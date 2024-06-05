## Operating Systems Project
# Client and Server Communication using Shared Memory & Mutex Locks

- Implemented the following steps :
1. The server initiates and creates the connect channel.
2. The server receives a register request on the connect channel.
 a. On successful creation of a comm channel for a client
 b. On successful response made to the client’s register request.
4. The server receives an unregister request
 a. On successful cleanup of the comm channel.
5. The server receives a service request on the comm channel.
6. The server responds to the client on the comm channel.
7. The server should maintain and print the summary info.
 a. The list of registered clients
 b. The count of requests serviced for the client. Note that failed requests which are
   responded to are also counted in.
c. The total count of requests serviced for all clients.
8. The client makes register request to the server on the connect channel.
9. The client connects to the server on the comm channel.
10. The client sends a request to the server on the comm channel
11. The client receives a response on the comm channel.
12. All error states, and their mitigations/counter actions
