# IRC Server  

An IRC server developed using the C++98 standard (any external library and Boost libraries are forbidden). 

## Overview  

- This project focuses solely on server-side development and does not include an IRC client.  
- Server-to-server communication is not implemented.  
- The executable is run as follows:  

  ```sh
  ./ircserv <port> <password>
### Port  
The port number on which the IRC server will listen for incoming connections.  

### Password  
The connection password required by any IRC client attempting to connect.  

## Requirements  

- The server should be able to handle multiple clients simultaneously without becoming unresponsive.  
- Forking is not allowed. All I/O operations must be non-blocking.  
- Only a single `poll()` (or an equivalent function) should be used to manage all operations, including reading, writing, and listening.  
- A reference IRC client should be chosen for compatibility testing. This client will be used during evaluation.  
- The selected reference client should be able to connect to the server without errors.  
- Communication between the client and server should be handled via TCP/IP (IPv4 or IPv6).  
- Using the reference client with this server should resemble using it with any official IRC server.  

## Features  

### User Capabilities  

- Authenticate, set a nickname, and set a username.  
- Join a channel, send, and receive private messages using the reference client.  
- Send messages to a channel, which should be forwarded to all other users in that channel.  
- Be categorized as regular users or operators.  

### Operator Commands  

- **KICK** – Remove a user from a channel.  
- **INVITE** – Invite a user to join a channel.  
- **TOPIC** – View or change the channel topic.  
- **MODE** – Modify channel settings:  
  - `i` – Enable/disable invite-only mode.  
  - `t` – Restrict `TOPIC` changes to operators.  
  - `k` – Set/remove a channel password.  
  - `o` – Grant/revoke operator privileges.  
  - `l` – Set/remove the user limit for a channel.  
