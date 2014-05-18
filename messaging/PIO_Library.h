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
#ifndef PIO_HEADER
#define PIO_HEADER
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
/* ********************************** */
int Write_PIO(int, int);
char *Read_PIO(int);
int Close_PIO();
int Initialise_PIO(int, int, int);
/* ********************************** */
#endif
#ifdef __cplusplus
}
#endif
/* ************************************************************************************************************ */
/* ************************************************************************************************************ */
