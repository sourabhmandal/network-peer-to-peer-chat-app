#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>

/*
 * socket() --
 * bind() --
 * listen() --
 * accept() --
 * recv()
 * send()
 * close
 */
int queue_limit = 2;

int main(){
    char host[NI_MAXHOST], client[NI_MAXSERV];
    int s_socket_id, c_socket_id;
    int servPort = 5100;

    //--------------------------------------------------------creating a socket-------------------------------------------------------------
    
    //PF_INET = protocol family,internet, identifies using protocol(TCP/IP or UDP/IP)
    //SOCK_STREAM = establishes 2-way connection based service
    //IPPROTO_TCP = uses TCP/IP as protocol
    s_socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(s_socket_id < 0){
        std::cout << "NO SOCKET CREATED... OPERATION FAILED !!!\n";
        return 0;
    }else{
        std::cout << "SOCKET CREATED...\n";
    }

    //structure used for TCP/IP addressing
    struct sockaddr_in server_addrs, client_addrs;  //provides internet address
    socklen_t client_len = sizeof(client_addrs);
    server_addrs.sin_family = AF_INET;
    server_addrs.sin_addr.s_addr = htonl(INADDR_ANY); //host-to-network-lock
    server_addrs.sin_port = htons(5100);  //host-to-network-short
    inet_pton(AF_INET, "0.0.0.0", &server_addrs.sin_addr); //Convert from presentation format of an Network number

    //---------------------------------------------------BIND - to assign address to socket-----------------------------------------------
    //bind(sockid, &addrport, size)
    // Gives socket file descriptor local address
    int status = bind(s_socket_id, (struct sockaddr *) &server_addrs, sizeof(server_addrs));
    if(status < 0){
        std::cout << "Error: SOCKET BIND FAILED !!!\n\n";
        return 0;
    }else{
        std::cout << "Success: SOCKET BIND\n"
    }
    std::cout << "Success: Connected to the port : " << servPort << std::endl;

    //--------------------------------------------------LISTEN - to listen for connection------------------------------------------------
    //listen(sockid, queueLimit)
    //sockid = integer, socket descriptor
    //queue_limit = max active paticipant can wait
    //used by server only - to get new sockets
    status = listen(s_socket_id, queue_limit);
    if(status < 0){
        std::cout << "Error: LISTENER FAILED\n";
        return 0;
    }else{
        std::cout << "Success: LISTENING STARTED\n";
    }

    //--------------------------------------------ACCEPT - accept client socket connection ----------------------------------------------
    //returns new socket value
    //sockid : origional socket id(being listened on)
    //clientAddr : address of client pc
    //addrLen : size of address
    //blocking

    c_socket_id = accept(s_socket_id, (struct sockaddr *) &client_addrs, &client_len);
    if(c_socket_id < 0){
        std::cerr << "Error: Cant connect to Client...\n";
    }else{
        std::cout << "Success: Connected To the Client..\n";
    }

    //memset()
    //reset the host and client char arrays to specific value, here=0
    memset(host, 0, NI_MAXHOST);
    memset(client, 0, NI_MAXSERV);

    //Translate a socket address to a location and service name
    int result = getnameinfo((sockaddr *) &client_addrs, sizeof(client_addrs), host, NI_MAXHOST, client, NI_MAXSERV, 0);
    if(result){
        std::cout << host << " ---> Connected on ---> " << client << std::endl;
    }else{
        inet_ntop(AF_INET, &client_addrs.sin_addr, host, 1000);
        std::cout << host << " ---> Connected no ---> " << ntohs(client_addrs.sin_port) << std::endl;
    }

    char buf[1000];
    for(;;){
        memset(buf, 0, 1000);
        //receives n bytes into buffer from server_socket_file descriptor
        int byteRecv = recv(c_socket_id, buf, 1000, 0);
        if(byteRecv == -1){
            std::cerr << "Error: There was a connection issue\n";
            break;
        }else{
            std::string input;
            std::cout << "CLIENT> " << std::string(buf) << std::endl;
            std::cout << "Message Received form client..\n\n";
            memset(buf, 0, 1000);
            std::cout << "SERVER> ";
            std::cin.getline(buf, 1000);
            //send n bytes of buffer and server_socket_file descriptor
            send(c_socket_id, buf, sizeof(buf) / sizeof(buf[0]), 0);    //blocking process
            std::cout << "Message sent...\n\n";
        }
    }
    //Close the file descriptor
    close(c_socket_id);
    exit(0);
}
