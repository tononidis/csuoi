# Assignment 1

In this assignment we where given an already implemented server-client source code.
Where the client sended requests to the server to either put or get key-value pairs, that represented a station.
The format that was used is like this "<operation>:<key>:<value>".

As an example of a put request is "PUT:station.44:-4>",
and to get (retrieve) the value "GET:station.44>".

In order to store or retrieve values the open source library KISSDB was used (https://github.com/adamierymenko/kissdb).

For this exercise we where tasked to modify the server source code in order to support multiple threads, and client requests concurrently.
However we where not expected to support PUT and GET requests concurrently, only the PUT requests.
The client requests are handled by one thread (producer thread), that pushed the connections info into a queue.
Then the consumer threads popped this connection info and handled to request.
If the queue was either empty or full the appropriate thread blocks and unblocks where placed.
For this implementation the pthread library was used.

Another task was to print collected server statistics when the process was terminated by a SIGTSTP (ctrl-z) signal.
These statistics include: the total requests completed, the average time a request remained idle in the queue waiting for a consumer thread to handle it and the
average time for handling the request.

Finally we where tasked to modify the client source code to support multiple threads, in order to send requests concurrently.

Since I am not the original author for some parts of the server and client code. I can not share the original code with out the teachers permission.
So instead I went the extra step to reimplement all that original source code using my current knowledge and understanding of the C programming language.
However some implementation parts are still shared with my collogue at the time Panagiotis Drivas.

These changes include:

- The parsing of the users message is now the clients responsibility.
- Lowering of the memory usage by reusing the same buffer for both reading and writing on the network socket.
- The server can also supports GET and PUT requests simultaneously.
- Originally when multiple put or get requests where send, new connection was established and destroyed for each request. This however has been refactored in order to keep the connection session alive until all the requests have been deployed.

To build the assignment make sure you have installed the premake5 tool (https://premake.github.io).
Then run:
>>> premake5 gmake && make config=<debug/release/dist> all
Find the bin folder that contains the executables for the server and client.
