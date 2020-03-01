#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h> //inet_addr
#include <sstream>
#include <iostream>

#define MAX 80
#define DEFAULT_PORT 8089

// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        std::cout << "From client: " << buff << std::endl;

        // and send back that message to the client
        write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

int main(int argc, char** argv)
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    unsigned int port = DEFAULT_PORT;

    std::cout << "starting the echo socket server..." << std::endl;
    if(argc > 1)
	{
	    std::istringstream ss(argv[1]);
	    if (!(ss >> port))
		{
		    std::cerr << "Could not parse the port argument" << std::endl;
		    return -1;
		}
	}

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cerr << "Could not create socket";
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        std::cout << "bind failed, port may already be in use." << std::endl;
	return -1;
    }
    std::cout << "bind done on port " << port << std::endl;

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    std::cout << "Waiting for incoming connections " << std::endl;
    c = sizeof(struct sockaddr_in);
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (new_socket<0)
    {
        std::cerr << "accept failed" << std::endl;
    }

    std::cout << "Connection accepted" << std::endl;
    func(new_socket);

    return 0;
}
