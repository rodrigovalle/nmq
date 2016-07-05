import re
import os, sys, atexit
import concurrent.futures

from urllib.parse import parse_qs
from urllib.request import urlopen
from jsinterp import JSInterpreter


cipher_cache = {}

def warm_cache(thread_pool):
    # open a file with a bunch of player URLs and calculate cipher functions
    try:
        home = os.path.expanduser('~')
        f = open(home + "/.cache/nmq/youtube-clients", mode='r')
        cipher_to_player = {thread_pool.submit(get_cipher_function, player): player for player in f}
    except FileNotFoundError:
        return

    for cipher_future in concurrent.futures.as_completed(cipher_to_player):
        player_url = cipher_to_player[cipher_future]
        try:
            cache[player_url] = cipher_future.result()
        except:
            continue

    f.close()


def write_cache_to_disk():
    home = os.path.expanduser('~')
    if not os.path.exists(home + "/.cache/nmq/"):
        os.makedirs(home + "/.cache/nmq/")

    f = open(home + "/.cache/nmq/youtube-clients", mode='w+')
    for player_url in cipher_cache:
        f.write(player_url + '\n')


def get_info(video_id):
    '''
    returns youtube video's meta-data as a dict
    '''
    base = "http://www.youtube.com/get_video_info"
    # try: &el=info, &el=vevo, &el=embedded, &el=detailpage, ''
    query = "{}?video_id={}&el=detailpage&ps=default&eurl=&gl=US&hl=en"
    info_url = query.format(base, video_id)

    with urlopen(info_url) as resp:
        qs = resp.read()

    return parse_qs(qs.decode())


def get_hq_stream(stream_map_str):
    '''
    returns the audio stream with the highest bitrate as a dict
    '''
    if not stream_map_str:
        raise ValueError("stream map is empty")

    hq_stream = {}
    max_rate = 0

    for stream_data_str in stream_map_str.split(','):
        stream_data = parse_qs(stream_data_str)
        if 'audio' in stream_data['type'][0]:
            current_rate = int(stream_data['bitrate'][0])

            if current_rate > max_rate:
                max_rate = current_rate
                hq_stream = stream_data

    if not hq_stream:
        raise RuntimeError("no audio-only streams")

    return hq_stream


def get_cipher_function(video_id):
    '''
    This call makes two extra network requests, one for the video webpage
    and the other for the video's javascript client (base.js)
    That means this function call is expensive af, and the requests must
    be made synchronously.

    Optimization:
    - Cache the js client file's signature function.
    '''
    # get the youtube player so we can extract the encrypted signature
    url = "https://www.youtube.com/watch?v={}".format(video_id)
    with urlopen(url) as resp:
        video_webpage = resp.read().decode()

    ASSETS_RE = r'"assets":.+?"js":\s*("[^"]+")'
    player_url = re.search(ASSETS_RE, video_webpage).group(1)
    player_url = 'http:' + player_url.strip('"').replace('\\', '')

    # before continuing, check the cache
    if player_url in cipher_cache:
        return cipher_cache[player_url]

    with urlopen(player_url) as resp:
        jscode = resp.read().decode()

    ENCRYPTFUN_RE = r'\.sig\|\|([a-zA-Z0-9$]+)\('
    funcname = re.search(ENCRYPTFUN_RE, jscode).group(1)
    jsi = JSInterpreter(jscode)
    fun = jsi.extract_function(funcname)

    cipher_cache[player_url] = lambda s: fun([s])

    return cipher_cache[player_url]


def get_url(video_id, thread_pool):
    ''' returns a url to the highest quality audio stream '''
    info_future = thread_pool.submit(get_info, video_id)
    cipher_future = thread_pool.submit(get_cipher_function, video_id)

    info = info_future.result()
    adaptive_stream_map = info['adaptive_fmts'][0]
    #fmt_stream_map = info['url_encoded_fmt_stream_map'][0]
    hq_stream_data = get_hq_stream(adaptive_stream_map)

    format_id = hq_stream_data['itag'][0]
    url = hq_stream_data['url'][0]

    if 'sig' in hq_stream_data:
        cipher_future.cancel()
        url += '&signature=' + url_data['sig'][0]
    elif 's' in hq_stream_data:
        encrypted_sig = hq_stream_data['s'][0]
        decipher = cipher_future.result()
        url += '&signature=' + decipher(encrypted_sig)

    if 'ratebypass' not in hq_stream_data:
        url += '&ratebypass=yes'

    return url


def main():
    # step - vampire weekend
    #v = "_mDxcDjg9P4"

    tpool = concurrent.futures.ThreadPoolExecutor(max_workers=3)
    warm_cache(tpool)
    atexit.register(write_cache_to_disk)
    
    #print(get_url(v, tpool)) # used to test
    for line in sys.stdin:
        print(get_url(line.strip(), tpool), flush=True)

if __name__ == "__main__":
    main()
