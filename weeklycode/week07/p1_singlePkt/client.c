/* Sending a TCP connect request to the server and sending a single packet */

#include <unistd.h> //for forking
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //manipulating strings (arrays of characters)

#include <errno.h> //defines macros for reporting and retrieving error conditions...
                   // errno acts like an integer variable... errno starts
                   // off as 0, and the library functions only store
                   //values freater than 0 (when they detect an error)

#include <sys/types.h> //includes necessary header files
#include <sys/socket.h> //supports any type of socket
#include <netinet/in.h> //contains definitions for the internet protocol family
#include <netdb.h> //contains definitions for network database operations

void checkError(int status) {
    if(status < 0) {
        printf("Socket error (%d): [%s]\n", getpid(), strerror(errno));
        exit(-1);
    }
}

int main(int argc, char* argv[]) {
    // Create a TCP socket
    int sid = socket(PF_INET, SOCK_STREAM, 0);  // creates a TCP socket for communication

    // Set the address of the server - address family, IP address and port number
    struct sockaddr_in srv; //*** sockaddr_in struct is parallel to sockaddr
                            //*** (_in for internet)... 
    struct hostent *server = gethostbyname("localhost"); // mapping hostname to IP address using DNS
    srv.sin_family = AF_INET;
    srv.sin_port = htons(8080); // because we have set the port # in server as 8080
    memcpy(&srv.sin_addr.s_addr, server->h_addr, server->h_length); //initializing srv with info from server (addr and length)

    // Send the connect request
    int status = connect(sid, (struct sockaddr*)&srv, sizeof(srv)); //clients always used the connect function
    checkError(status);

    // Send a message to the server
    char buf[] = "Hello server!";
    status = send(sid, buf, sizeof(buf), 0); /* can we send the entire payload in a single packet? */
    checkError(status);

    // Receive a message from the server
    char asw[256];
    status = recv(sid, asw, sizeof(asw), 0);
    checkError(status);
    printf("Response was: [%s]\n", asw);

    return 0;
}