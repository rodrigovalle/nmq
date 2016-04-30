#include <stdio.h>
#include <libwebsockets.h>

int
main()
{
	printf("%s\n", lws_get_library_version());
	return 0;
}
