/******************************************************************************
* myServer.c
* 
* Writen by Prof. Smith, updated Jan 2023
* Use at your own risk.  
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>

#include "networks.h"
#include "safeUtil.h"
#include "sendReceive.h"

#define MAXBUF 1024
#define DEBUG_FLAG 1

void recvFromClient(int clientSocket);
int checkArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	int mainServerSocket = 0;   //socket descriptor for the server socket
	//int clientSocket = 0;   //socket descriptor for the client socket
	int portNumber = 0;

	
	portNumber = checkArgs(argc, argv);
	
	//create the server socket
	setupPollSet();
	mainServerSocket = tcpServerSetup(portNumber);
	addToPollSet(mainServerSocket);

	serverControl(mainServerSocket);

	// wait for client to connect


	//clientSocket = tcpAccept(mainServerSocket, DEBUG_FLAG);
	
	

	//recvFromClient(clientSocket);
	
	/* close the sockets */
	//close(clientSocket);
	close(mainServerSocket);

	
	return 0;
}

void recvFromClient(int clientSocket)
{
	uint8_t dataBuffer[MAXBUF];
	int messageLen = 0;
	
	//now get the data from the client_socket
	if ((messageLen = recvPDU(clientSocket, dataBuffer, MAXBUF, 0)) < 0)
	{
		perror("recv call");
		exit(-1);
	}

	if (messageLen > 0)
	{
		printf("Message received, length: %d Data: %s\n", messageLen, dataBuffer);
	}
	else
	{
		printf("Connection closed by other side\n");
		close(clientSocket);
		removeFromPollSet(clientSocket);
	}
}

int checkArgs(int argc, char *argv[])
{
	// Checks args and returns port number
	int portNumber = 0;

	if (argc > 2)
	{
		fprintf(stderr, "Usage %s [optional port number]\n", argv[0]);
		exit(-1);
	}
	
	if (argc == 2)
	{
		portNumber = atoi(argv[1]);
	}
	
	return portNumber;
}

void serverControl(int main_socket){

	while(1){

		int socket = pollCall(10000);
		if(socket == main_socket)
			addNewSocket(socket);
		else
			processClient(socket);

	}

}

void addNewSocket(int main_socket){

	int clientSocket = tcpAccept(main_socket, DEBUG_FLAG);
	addToPollSet(clientSocket);

	addToPollSet(clientSocket);

}

void processClient(int socket){

	recvFromClient(socket);

}


