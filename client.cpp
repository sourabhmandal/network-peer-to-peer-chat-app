#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

using namespace std;
/*
 * socket()
 * connet()
 * send()
 * recv()
 * close()
 */
// TCP echo client program
int main () {
    int s_socket_id;
    sockaddr_in serverAddr;  //host server IP address stored
    int servPort = 5100;


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //host-to-network-lock (IP address)
    serverAddr.sin_port = htons (servPort); // Server port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); //Convert from presentation format of an Network number

    // Create socket
    // creates a socket of specified DOMAIN AND PROTOCOL
    s_socket_id = socket (PF_INET, SOCK_STREAM, 0);
    if (s_socket_id < 0){
        cout << "Error: socket creation failed!\n";
        exit (1);
    }else{
        cout << "Success: socket created!\n";
    }
    // Connect to the server
    //connect server_socket_file_descriptor to peer server_address
    if (connect(s_socket_id, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        std::cerr << "Error: connection failed!\n";
        exit (1);
    }else{
        std::cout << "Error: connection failed!\n";
    }

    // Data transfer section
    while(true){
        //------------------------------------------SEND----------------------------------------------------------------
        string input;
        cout << "CLIENT> ";
        getline(cin, input);
        //send n bytes of buffer and server_socket_file descriptor
        int state = send (s_socket_id, input.c_str(), input.length() +1, 0);
        if(state < 0) {
            std::cout << "Error Sending Data.\n";
            break;
        }else{
            std::cout << "Message sent...\n\n";
        }
        //----------------------------------------RECEIVE---------------------------------------------------------------

        char buf[1000];
        memset(buf, 0, 1000);
        std::cout << "SERVER> ";
        //receives n bytes into buffer from server_socket_file descriptor
        int bytesRecv = recv(s_socket_id, buf, 1000, 0);;
        std::cout << string(buf, bytesRecv) << std::endl;
        std::cout << "Message Received from Server...\n\n";
    }
    close(s_socket_id);
    exit (0);
}
