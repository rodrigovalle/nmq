var ws;
var count = 0;
var id;

window.onload = function () {
    ws = new WebSocket("ws://localhost:8001");
    ws.onopen = function (open) {
        console.log("connected!");
        console.log("sending a test message...");
        id = window.setInterval(sendPing, 1000);
    }

    ws.onerror = function (error) {
        console.log("couldn't find the server :(");
        console.log(error.data);
    }

    ws.onmessage = function (message) {
        console.log("got a new message from the server: " + message.data);
        //ws.close();
    }

    ws.onclose = function () {
        console.log("connection closed.");
    }
};

window.beforeunload = function() {
    ws.onclose() = function() {};
    ws.close();
};

function sendPing() {
    if (count == 5) {
        window.clearInterval(id);
        ws.close();
    } else {
        ws.send("ping!");
    }
    count++;
}

/*function read_response() {
    port_frm = document.getElementById("getport");
    port = port_frm.elements['port'].value;
    address = "ws://localhost:" + port;
    console.log(address);
    ws = new WebSocket(address);
    bind_ws_event_handlers();
}*/
