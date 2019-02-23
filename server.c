/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Jiazheng Sun
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */
#include <ctype.h>
#include <time.h>
#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

struct User{
  char user_name[40];
  int user_balance;
  int withdraw_requests;
  time_t user_timer;
};

/* The main function */
int main(int argc, char *argv[])
{
    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort;		/* Server port */
    changeServPort = 8080;
    unsigned int clntLen;			/* Length of address data struct */

    char rcvBuf[RCVBUFSIZE]; /* Buff to store the whole received packet. Name + withdraw_amount*/
    char nameBuf[BUFSIZE];			/* Buff to store account name from client */
    char amountBuf[RCVBUFSIZE]; /*Buff to store withdraw_amount*/
    int  balance;				/* Place to record account balance result */
    int withdraw_amount; /*Place to record the amount the user try to withdraw*/


    struct User user1;
    strcpy(user1.user_name, "mySavings");
    user1.user_balance = 40000;
    user1.withdraw_requests = 0;
    struct User user2;
    strcpy(user2.user_name, "myChecking");
    user2.user_balance = 50000;
    user2.withdraw_requests = 0;
    struct User user3;
    strcpy(user3.user_name, "myRetirement");
    user3.user_balance = 60000;
    user3.withdraw_requests = 0;
    struct User user4;
    strcpy(user4.user_name, "myCollege");
    user4.user_balance = 70000;
    user4.withdraw_requests = 0;

    struct User users[4] = {user1, user2, user3, user4};

    /* Create new TCP Socket for incoming requests*/
    /*	    FILL IN	*/
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock < 0){
      perror("Socket failed");
      abort();
    }
    /* Construct local address structure*/
    /*	    FILL IN	*/
    memset(&changeServAddr, 0, sizeof(changeServAddr));
    changeServAddr.sin_addr.s_addr = INADDR_ANY;
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_port = htons(changeServPort);

    /* Bind to local address structure */
    /*	    FILL IN	*/
    if(bind(serverSock,(struct sockaddr *) &changeServAddr, sizeof(changeServAddr)) < 0){
      perror("bind failed");
      abort();
    }
    /* Listen for incoming connections */
    /*	    FILL IN	*/
    // back_log connection queue size? how to get it?
    if(listen(serverSock, 5) < 0){
      perror("listen failed");
      abort();
    }
    /* Loop server forever*/
    while(1)
    {

	/* Accept incoming connection */
	/*	FILL IN	    */
  clntLen = sizeof(changeClntAddr);
  clientSock = accept(serverSock,(struct sockaddr *)&changeClntAddr,&clntLen);
  if(clientSock < 0) {
    perror("accept failed");
    abort();
  }
	/* Extract the account name from the packet, store in nameBuf */
	/* Look up account balance, store in balance */
	/*	FILL IN	    */
  int total_receive = 0;
  while(total_receive < RCVBUFSIZE){
    int read_bytes = recv(clientSock,rcvBuf,RCVBUFSIZE,0);
    if (read_bytes < 0) {
      perror("receive failed");
      abort();
    } else if (read_bytes == 0) {
      close(serverSock);
    } else {
      total_receive += read_bytes;
    }
  }


  int j = 0;
  while (isdigit(rcvBuf[j])==0 && j < strlen(rcvBuf)) {
    j += 1;
  }
  if (rcvBuf[j] != '\0') {
    strcpy(amountBuf, rcvBuf + j);
  }
  strncpy(nameBuf, rcvBuf, j);
  nameBuf[j] = '\0';


  int temp;
  int record;
  for (temp = 0; temp < 4; temp++) {
    if (strcmp(users[temp].user_name, nameBuf) == 0){
      record = temp;
    }
  }
  balance = users[record].user_balance;
  char balanceArray[500];
  if (strlen(amountBuf) > 1) { //withdraw command
    if (users[record].withdraw_requests >= 3) {
      sprintf(balanceArray, "%s", "Error: Exceed 3 withdraw requests within a minute. Please try again later.");
    } else {
      withdraw_amount = atoi(amountBuf);
      if(withdraw_amount <= balance) {
        balance -= withdraw_amount;
        users[record].user_balance = balance;
        sprintf(balanceArray, "%1d", balance);
      } else {
        sprintf(balanceArray, "%s", "Error: Insufficient Funds!");
      }
    }
    if (users[record].withdraw_requests == 0) {
      users[record].user_timer = time(NULL);
    }
    users[record].withdraw_requests += 1;
    if (time(NULL) - users[record].user_timer > 59) {
      users[record].withdraw_requests = 0;
    }
  } else { //BAL command
    sprintf(balanceArray, "%1d", balance);
  }
	/* Return account balance to client */
	/*	FILL IN	    */
  int sent_bytes = send(clientSock, balanceArray, strlen(balanceArray), 0);
  if(sent_bytes < 0) {
    perror("sent failed");
  }
  //Resore arrays and values to make sure no old values are interfering with future stuff
  memset(rcvBuf, 0, 512);
  memset(nameBuf,0,40);
  memset(amountBuf,0,512);
  withdraw_amount = 0;
    }
    close(serverSock);
}
