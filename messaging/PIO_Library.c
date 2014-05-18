/************************************************ USB-PIO ******************************************************
*
*Author: Sami Giacaman & Mark Randall
*Date Created: 23/4/2014.
*Date Amended: 02/5/2014.
*Purpose: Solution for USB-PIO Keypad.
*
*Notes:
*
*Program Description: 
*
*How to Compile: gcc -o keypad KeypadDriverV1.c PIO_Library.c -lm 
*        
*And to Run it: ./keypad
* 
*
************************************************************************************************************* */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <string.h>
extern "C" {
#include "PIO_Library.h"
}
/* ********************************** */
#define DELAY 1000
/* ********************************** */
char *usbPIO = "/dev/ttyACM0";
int fd = 0;

void Port_Setup(int, int);
char *Convert_Int(int);

/* ********************************** *
 *  Write Function
/* ********************************** */
int Write_PIO(int portNum, int portVal)
{
	char* portString = Convert_Int(portVal);
	char writingPort[10];
	char check[10];

	sprintf(writingPort, "@00P%d%s\r", portNum, portString);
	write(fd, writingPort, 8);
	usleep(DELAY);
	read(fd, check, 6);
	usleep(DELAY);

	return 1;
}
/* ********************************** */

/* ********************************** *
 *  Read Function
/* ********************************** */
char *Read_PIO(int portNum)
{
	char readingPort[10];
	char* reply;
	reply = (char *) malloc(10 * sizeof(char));

	sprintf(readingPort, "@00P%d?\r", portNum);
	write(fd, readingPort, 8);
	usleep(DELAY);
	read(fd, reply, 6);
	usleep(DELAY);

	return reply;
}
/* ********************************** */

/* ********************************** *
 *  Close Function
/* ********************************** */
int Close_PIO()
{
	if (close(fd) == -1);
	{
		printf("ERROR: %s\n", strerror(errno));
		return 0;
	}

	return 1;
}
/* ********************************** */

/* ********************************** *
 *  Initialise Function
/* ********************************** */
int Initialise_PIO(int port0, int port1, int port2)
{
	int i = 0;
	int failure = 0;

	fd = open(usbPIO, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(fd <= 0)
	{
		printf("Failed to Open Device: %s\n", strerror(errno));
		return 0;
	}
        for (i = 0; i < 3; i++)
	{
		if(i==0)
		{
			/*failure = */Port_Setup(i,port0);
		}
		else if(i==1)
		{
			/*failure = */Port_Setup(i,port1);
		}
		else if(i==2)
		{
			/*failure = */Port_Setup(i,port2);
		}

		if(failure)
		{
			printf("Trapped in a fail\n");
			return 0;
		}	
	}

	return 1;
}
/* ********************************** */

/* ********************************** *
 *  Port Setup Function
/* ********************************** */
void Port_Setup(int portNum, int portVal)
{
	char* portString;
	int failure = 1;
	char settingPort[10];
	char check[10];

	if(portVal == 0)
	{
		portString = Convert_Int(portVal);
	}
	else if(portVal == 255)
	{
		portString = Convert_Int(portVal);
	}
	else
	{
		printf("Invalid Direction Value for Port%d\n", portNum);
		return;
	}

	sprintf(settingPort, "@00D%d%s\r", portNum, portString);

	if(write(fd, settingPort, 8) == -1)
	{
		printf("ERROR: %s\n", strerror(errno));
		return;
	}

	usleep(DELAY);
	if(read(fd, check, 4) == -1)
	{
		printf("ERROR: %s\n", strerror(errno));
		return;
	}
	usleep(DELAY);

}
/* ********************************** */

/* ********************************** *
 *  Convert Int Function
/* ********************************** */
char *Convert_Int(int portVal)
{
	char* portString;
	portString = (char *) malloc(10 * sizeof(char));

	if(portVal < 10)
	{	
		sprintf(portString, "0%x", portVal);
	}
	else
	{
		sprintf(portString, "%x", portVal);
	}

	return portString;
}
/* ********************************** */
#ifdef __cplusplus
}
#endif
/* ************************************************************************************************************ */
/* ************************************************************************************************************ */
