Basically, the server has to keep track of the state of the queue, and clients
can issue commands to the server to change the state of the queue. In an effort
to minimize network IO, requests are identified in websocket messages as
specific binary opcodes:

       OPERATION   |   OPCODE
    ---------------|------------
       play        |   0x00
       pause       |   0x01
       add         |   0x02
       remove      |   0x03
       seek        |   0x04

an idea: javascript RPCs
These opcodes are essentially function calls and the protocol can maybe have some
way of serializing the necessary arguments. The return value lets the javascript
client know if the operation succeeded or if it needs to reissue the request
after a timeout.
