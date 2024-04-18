
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
#include <errno.h>

#include "sendReceive.h"
#include "pollLib.h"

#define MAX_BUF_LENGTH_PLUS_PDU 1026


/*

int safeSend(int socketNum, uint8_t * buffer, int bufferLen, int flag)
{
	int bytesSent = 0;
	if ((bytesSent = send(socketNum, buffer, bufferLen, flag)) < 0)
	{
        perror("recv call");
       exit(-1);
     }
	 
    return bytesSent;
}

*/

int sendPDU(int socketNum, unsigned short int *buffer, int bufferLen, int flag){

    int bytes_sent = 0;
    unsigned short int PDU_length = bufferLen+2;

    unsigned short int send_buffer[MAX_BUF_LENGTH_PLUS_PDU];

    memcpy(send_buffer,&PDU_length,2);
    memcpy(send_buffer+1,buffer,bufferLen * sizeof(unsigned short int));


    if((bytes_sent = send(socketNum,send_buffer,bufferLen+2,flag)) < 0){

        perror("recv call");
        exit(EXIT_FAILURE);

    }

    return bytes_sent;


}



int recvPDU(int socketNum, unsigned short int * buffer, int bufferLen, int flag){

    unsigned short int rec_buffer[MAX_BUF_LENGTH_PLUS_PDU];

    int bytes_received = safeRecv(socketNum,rec_buffer,bufferLen+2,flag);
    if (bytes_received < 0)
    {
        if (errno == ECONNRESET)
        {
            bytes_received = 0;
        }
        else
        {
            perror("recv call");
            exit(EXIT_FAILURE);
        }
    }
    else if(bytes_received == 0)
        return bytes_received;

    unsigned short int PDU_length;
    memcpy(&PDU_length,rec_buffer,2);
    memcpy(buffer,rec_buffer+1,PDU_length-2);

    return bytes_received;

}
