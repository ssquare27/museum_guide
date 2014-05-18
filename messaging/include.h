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
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
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
#include <net/if.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <netdb.h> 
#include <errno.h>
#include <fcntl.h>
#include <time.h>
extern "C" {
#include "PIO_Library.h"
}
/* ********************************* */
/* ********************************* */
#define MAXPATH 4096 	/* Max Length. */
#define QLEN 6 	        /* Size of Request Queue. */
#define GOOD 1
#define BAD 0
/* ************************************************************************************* */
#define SERVICEUN "HTTP/1.1 - 503 - Service Unavailble\r\n"
/* ************************************************************************************* */
#define LED_0 0x3F
#define LED_1 0x06
#define LED_2 0x5B
#define LED_3 0x4F
#define LED_4 0x66
#define LED_5 0x6D
#define LED_6 0x7D
#define LED_7 0x07
#define LED_8 0x7F
#define LED_9 0x6F
#define LED_A 0x77
#define LED_B 0x7C
#define LED_C 0x39
#define LED_D 0x5E
#define LED_E 0x79
#define LED_F 0x71
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
