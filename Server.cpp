#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <exception>

using namespace std;

class ServerException: public exception {
public:
    virtual const char* what() const throw(){
        return "Size exceeded!";
    }
};

int main()
{
    ServerException serverException;
// SERVER

cout << "==========THE SERVER===========" << endl;

    sockaddr_in serverSocket;
    sockaddr_in service;

    string address = "127.0.0.1";  // The IP address

    char host[NI_MAXHOST];
    //char service[NI_MAXSERV];


// CREATING the server socket below
    cout << "The SERVER Socket being created" << endl;

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1)
        cout << "There has been an error with creating the server socket" << endl;
    else
        cout << "Server Socket has been created!!" << endl;


// BINDING the server socket below
    cout << "The Server socket binding stage" << endl;

    service.sin_family = AF_INET;
    service.sin_port = htons(5400);
    inet_pton(AF_INET, address.c_str(), &service.sin_addr);


    if (::bind(serverSock, (sockaddr*)&service, sizeof(service)) == -1)
        cout << "The binding stage has been unsuccessful " << endl;
    else
        cout << "The binding stage has been successful!!" << endl;


// LISTENING for the client socket connection
    cout << "The Server Socket listening stage" << endl;

    if (listen(serverSock, SOMAXCONN) == -1)
        cout << "The listen function has failed" << endl;
    else
        cout << "The server socket is now listening....can you hear anything???" << endl;


// ACCEPTING the client socket request
    cout << "The Server Socket Accepting the Requestion Connection from Client" << endl;

    sockaddr_storage client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(serverSock, (sockaddr*)&client, &clientSize);

    if (clientSocket == -1)
        cout << "The connection request has not been accepted, there has been an error" << endl;
    else
        cout << "The connection has been accepted successfully! We are now connected to the client " << endl;






// RECEIVING AND SENDING data back and forth between the client and server socket
    cout << "Time for a little chat with the CLIENT" << endl;

    // Receive messages from client socket

    char receiveClientMessage[500];
    while(1) {
        try {
            int byteServerCount = recv(clientSocket, receiveClientMessage, 500, 0);

            if (byteServerCount < 0)
                cout << "No message has been received from the client :(" << endl;
            else
                cout << "Client has sent you a message: " << receiveClientMessage << " .Care to reply??" << endl;
            if (strcmp(receiveClientMessage, "QUIT")== 0) {
                break;
            }
            else if (strlen(receiveClientMessage) > 500) {
                throw serverException;
            }
        }
        catch(ServerException e) {
            cout << "Exception: " << e.what() << endl;
            break;
        }


        // Send messages back to the client socket

        char sendClientMessage[500];
        printf("Please type in a reply to the client: ");
        cin.getline(sendClientMessage, 500);
        try {
            int byteCount = send(clientSocket, sendClientMessage, 200, 0);
            if (byteCount == -1)
                cout << "There has been an error with the send function" << endl;
            else
                cout << "The message has been sent to the client successfully!" << endl;
            if (strcmp(sendClientMessage, "QUIT")== 0) {
                cout << "Server Socket if Closed" << endl;
                close(serverSock);
                break;
            }
            else if (strlen(receiveClientMessage) > 500) {
                throw serverException;
            }
        }
        catch(ServerException e) {
            cout << "Exception: " << e.what() << endl;
            break;
        }
    }

    return 0;
}