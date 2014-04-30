/*keypad.c
 * Reads keypad key presses and puts corresponding
 * number on the seven segment display.
 * There is currently a bug with this code not working
 * on the IGEPv2 due to the software thinking a 
 * key 13 is being held down.
 * Future versions will fix this problem.
 * Note this code is being shared across
 * 2 modules at UWE
 * 1. Embedded Systems Development
 * 2. Designing and Writing Device Drivers
 *
 * Authors: Nathan Toft (10018903), Ben Holden, Thomas West
 * and Gregory Masters
 */
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

enum KeypadButton{KEY_NONE, 
		  KEY_1, KEY_2, KEY_3, KEY_F,
                  KEY_4, KEY_5, KEY_6, KEY_E,
                  KEY_7, KEY_8, KEY_9, KEY_D,
                  KEY_A, KEY_0, KEY_B, KEY_C};

enum segChar {ZERO=0x3F, ONE=0x06, TWO=0x5B, THREE=0x4F, FOUR=0x66, FIVE=0x6D,
	      SIX=0x7D, SEVEN=0x07, EIGHT=0x7F, NINE=0x6F, A=0x77, B=0x7C,
	      C=0x39, D=0x5E, E=0x79, F=0x71};

int getRowNumber(char*);
int checkColumnForPressed(int);
enum KeypadButton getButton(int, int);
void showButton(enum KeypadButton);
enum segChar getHexRepresentation(int button);
int buttonIsNumeric(enum KeypadButton);
int getRealNumber(enum KeypadButton button);
void selectCol(int col);
void writeNum(int number);
enum KeypadButton buttonPressed(int col);

int fd;

/*Set up interface attributes for serial port.
 * Original source http://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
 */
int set_interface_attribs (int fd, int speed, int parity)
{
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0)
    {
      fprintf (stderr, "error %d from tcgetattr\n", errno);
      return -1;
    }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // ignore break signal
  tty.c_lflag = 0;                // no signaling chars, no echo,
  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
      fprintf (stderr, "error %d from tcsetattr\n", errno);
      return -1;
    }
  return 0;
}

/*Serial port attributes for non-blocking read and writes
 * Original source: http://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
 */
void set_blocking (int fd, int should_block)
{
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0)
    {
      fprintf (stderr, "error %d from tggetattr\n", errno);
      return;
    }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
    fprintf (stderr, "error %d setting term attributes\n", errno);
}

int main()
{
  char *portname = "/dev/ttyACM0";
  int i;
  enum KeypadButton pincode[3];
  int count=0;
  int pressed = 0;
  int lastCol = -1;
  char buf [100];
  int col;
  int n;
  enum KeypadButton button;

  fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0)
    {
      fprintf (stderr, "error %d opening %s: %s\n", errno, portname, strerror (errno));
      return 1;
    }

  set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  set_blocking (fd, 0);                // set no blocking
  printf("set up\n");
  write (fd, "@00D1FF\r", 8);  
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  write (fd, "@00D000\r", 8);
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  for (i = 0; i < 4; i++)
    pincode[i] = -1; 
  for (col = 0; col < 5; col++, col = col % 4)
    {
      selectCol(col);
      writeNum(pincode[col]);
      if (count > 3)
	continue;
      button = buttonPressed(col);
      if (button == KEY_NONE)
	{
	  if (lastCol == col)
	    {
	      pressed = 0;
	    }
	  printf("nothing\n");
	}
      else
	{
	  if (pressed)
	    {
	      // ignore button held down
	      printf("held\n");
	      continue;
	    }
	  // a new button was pressed
	  if (!buttonIsNumeric(button))
	    continue;
	  pincode[count] = getRealNumber(button);
	  pressed = 1;
	  lastCol = col;
	  count++;
	  printf("count is %d ", count);
	}
	
      usleep(1000);
    }

  return 0;
}

/*Selects which column to read on the keypad*/
void selectCol(int col)
{
  char buf [100];
  int n;
  if (col == 0)
    {
      write (fd, "@00P001\r", 8);
      n = read (fd, buf, sizeof buf);
      if (!n)
	fprintf(stderr, "Read failed\n");
    }
  else if (col == 1)
    {
      write (fd, "@00P002\r", 8);
      n = read (fd, buf, sizeof buf);
      if (!n)
	fprintf(stderr, "Read failed\n");
    }
  else if (col == 2)
    {
      write (fd, "@00P004\r", 8);
      n = read (fd, buf, sizeof buf);
      if (!n)
	fprintf(stderr, "Read failed\n");
    }
  else if (col == 3)
    {
      write (fd, "@00P008\r", 8);
      n = read (fd, buf, sizeof buf);
      if (!n)
	fprintf(stderr, "Read failed\n");
    }

  usleep(1000);
}

void writeNum(int number)
{
  enum segChar writeChar;
  char buf [100];
  char data[10];
  int n;
  printf("num %d\n", number);
  if (number < 0)
    {
      writeChar = 0x00;
    }
  else
    {
      writeChar = getHexRepresentation(number); 
    }

  sprintf (data, "@00P2%02x\r", writeChar);
  write (fd, data, 8);
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  usleep(1000);
  // write blank so it doesnt interfere with the next
  write (fd, "@00P200\r", 8);
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  usleep(50);
}

/*Returns which keypad button was pressed*/
enum KeypadButton buttonPressed(int col)
{
  char buf [100];
  int n;

  write (fd, "@00P1?\r", 7);
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  usleep(100);
  if (strcmp(buf,"!0000\r") != 0)
    {
      // something was pressed
      int row = 0;
      enum KeypadButton button = KEY_1;		
      row = getRowNumber(&buf[4]);
      return getButton(row,col);
    }
  return KEY_NONE;
}

/*Scans the row and returns the number of that row*/
int getRowNumber(char* ch)
{
  int row = 0;
  long int buttonValueRead;
 //last digit will be 1,2,4 or 8
  /*
    row 1 number could be 1 2 3 F
    row 2 number could be 4 5 6 E
    row 3 number could be 7 8 9 D
    row 4 number could be A 0 B C
    }*/

  buttonValueRead = strtol(ch, NULL, 16);
  printf("hex: %x, %li\n", buttonValueRead, buttonValueRead);
  if (buttonValueRead & 0x1)
    {
      row = 1;
    }
  else if (buttonValueRead & 0x2)
    {
      row = 2;
    }			
  else if (buttonValueRead & 0x4)
    {
      row = 3;
    }			
  else
    {
      row = 4;
    }
  return row;
}

// return 1 if a button pressed else 0
int checkColumnForPressed(int column)
{
  char data[10];
  int colHex;
  int n=0;
  char buf [100];

  switch (column)
    {
    case 1:
      colHex = 1; break;
    case 2:
      colHex = 2; break;
    case 3:
      colHex = 4; break;
    default:
      colHex = 8; break;
    }
  printf("column: %d\n", column);
  // set port0 to only read 1 column
  sprintf (data, "@00P00%1x\r", colHex);
  write (fd, data, 8);
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  usleep(10);
  // now do the actual read on port1
  write (fd, "@00P1?\r", 7);
  n = read (fd, buf, sizeof buf);
  if (!n)
    fprintf(stderr, "Read failed\n");
  usleep(100);
  if (strcmp(buf,"!0000\r") != 0)
    {
      return 1;
    }
  return 0;
}

/*Returns which keypad button was pressed*/
enum KeypadButton getButton(int row, int col)
{
  return ((row - 1) * 4) + (col + 1);
}

/*Checks if the button pressed on the keypad was numeric*/
int buttonIsNumeric(enum KeypadButton button)
{
  int isNumeric;

  switch (button)
    {
    case KEY_A:
    case KEY_B:
    case KEY_C:
    case KEY_D:
    case KEY_E:
    case KEY_F:
      isNumeric = 0; break;
    default /*Numeric*/:
      isNumeric = 1; break;
    }
  return isNumeric;
}

/*Maps the number off the pressed button on the keypad onto the real number*/
int getRealNumber(enum KeypadButton button)
{
  int number;
  switch (button)
    {
    case KEY_1:
      number = 1; break;
    case KEY_2:
      number = 2; break;
    case KEY_3:
      number = 3; break;
    case KEY_4:
      number = 4; break;
    case KEY_5:
      number = 5; break;
    case KEY_6:
      number = 6; break;
    case KEY_7:
      number = 7; break;
    case KEY_8:
      number = 8; break;
    case KEY_9:
      number = 9; break;
    case KEY_0:
      number = 0; break;
    default /*Letter*/:
      number = -1; break;
    }
  return number;
}
/*Write to the serial port to return which button was read*/
void showButton(enum KeypadButton button)
{
  char buf [100];
  char data[10];
  enum segChar hexToPrint = getHexRepresentation(button);
  sprintf (data, "@00P2%02x\r", hexToPrint);
  printf("Displaying: %02x\n", hexToPrint);
  printf("data %s\n", data);
  write (fd, data, 8);
  read (fd, buf, sizeof buf);
  usleep(1000);
}

/*Returns the correct hex number to write to the seven segment displays
 *NOTE: Letter keys are commented out until future work
 * is done to improve this*/
enum segChar getHexRepresentation(int button)
{
  enum segChar seg;
  switch (button)
    {
    case 1:
      seg = ONE; break;
    case 2:
      seg = TWO; break;
    case 3:
      seg = THREE; break;
      /*	case KEY_F:
		seg = F; break;*/
    case 4:
      seg = FOUR; break;
    case 5:
      seg = FIVE; break;
    case 6:
      seg = SIX; break;
      /*	case KEY_E:
		seg = E; break;*/
    case 7:
      seg = SEVEN; break;
    case 8:
      seg = EIGHT; break;
    case 9:
      seg = NINE; break;
      /*	case KEY_D:
		seg = D; break;
		case KEY_A:
		seg = A; break;*/
    case 0:
      seg = ZERO; break;
      /*	case KEY_B:
		seg = B; break;
		default:
		seg = C; break;*/
    }
  return seg;
}

