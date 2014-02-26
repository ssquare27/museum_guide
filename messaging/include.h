/*********************************************** -HTIGCP- ******************************************************
*
*Author: Sami Giacaman & Mark Randall.
*Date Created: 12/11/2013.
*Date Amended: 26/11/2013
*Purpose: Solution for Hyper Text iGep Control Protocol
*
*Notes: 
*
*Program Description: 
*
*How to Compile: g++ -I -Wall -o server Server.cpp -lpthread
*		 g++ -I -Wall -o client Client.cpp
*
*And to Run it: ./server <Port Number>
*		./client 
*OR
*
*How to Compile: make all
*		 
*And to Run it: ./server <Port Number>
*		./client 
*
************************************************************************************************************* */
/* ********************************* */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <limits.h> 
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h> 
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
/* ********************************* */
/* ********************************* */
#define MAXPATH 4096 	/* Max Length. */
#define QLEN 6 	        /* Size of Request Queue. */
#define GOOD 1
#define BAD 0
/* ************************************************************************************* */
#define SERVICEUN "HTTP/1.1 - 503 - Service Unavailble\r\n"
/* ************************************************************************************* */
/* ********************************* */
typedef struct 
{
    pthread_t ptid;
    int busy;
    int num;
    int socket;  
} T_vars;
T_vars avThreads[100];
/* ********************************* */
/* ******************************************************************************************************* */
/* ******************************************************************************************************* */
static void *readmessage(void *buptr);	                                       /* Server Read Function. */
static void *writemessage(void *buptr ,char *retbuffer);		       /* Server Write Function. */
/* ******************************************************************************************************* */
/* ******************************************************************************************************* */
