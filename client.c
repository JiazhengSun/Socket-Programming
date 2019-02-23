/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Jiazheng Sun
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[])
{
    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* server address structure */

    char *accountName;		    /* Account Name  */
    char *servIP;		    /* Server IP address  */
    unsigned short servPort;	    /* Server Port number */

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */

    int balance;		    /* Account balance */
    char *command;
    char *withdraw_amount;

    int check = 0;
    /* Get the Account Name from the command line */
    if (argc < 5)
    {
	     printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort operation (amount)");
	      exit(1);
    }
    command = argv[4];
    if (strcmp(command, "WITHDRAW") == 0) {
      if (argc != 6) {
        printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort WITHDRAW amount");
      	exit(1);
      }
      withdraw_amount = argv[5];
      check = 1;
    } else if (strcmp(command, "BAL") == 0) {
      if (argc != 5)
      {
  	     printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort BAL");
  	     exit(1);
      }
    } else {
      printf("Incorrect operation. Its either WITHDRAW or BAL");
      exit(1);
    }
    accountName = argv[1];
    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    /*	    FILL IN	*/
    servIP = argv[2];
    servPort = (short)strtol(argv[3], &argv[3], 10);
    command = argv[4];
    strcpy(sndBuf, accountName);
    if (check == 1) {
      strcat(sndBuf, withdraw_amount);
    }
    /* Create a new TCP socket*/
    /*	    FILL IN	*/
    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock < 0) {
      perror("socket() failed");
      abort();
    }
    /* Construct the server address structure */
    /*	    FILL IN	 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(servPort);

    /* Establish connecction to the server */
    /*	    FILL IN	 */
    if (connect(clientSock, (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
    {
        perror("connect error");
    }
    /* Send the string to the server */
    /*	    FILL IN	 */
    if (send(clientSock, sndBuf, SNDBUFSIZE, 0) < 0){
      perror("send failed");
    }


    /* Receive and print response from the server */
    /*	    FILL IN	 */
    if(recv(clientSock, rcvBuf, RCVBUFSIZE, 0) < 0) {
      perror("receive failed");
    }
    printf("%s\n", accountName);
    printf("Balance is: %s\n", rcvBuf);

    close(clientSock);
    return 0;
}
