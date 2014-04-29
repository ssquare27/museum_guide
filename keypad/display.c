#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

enum KeypadButton{KEY_1=1, KEY_2, KEY_3, KEY_F,
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
enum segChar getHexRepresentation(enum KeypadButton);
int buttonIsNumeric(enum KeypadButton);
int getRealNumber(enum KeypadButton button);
void selectCol(int col);

int fd;

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
//	char* input = NULL;
	int i;

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
	char buf [100];
	int n = read (fd, buf, sizeof buf);
	if (!n)
		fprintf(stderr, "Read failed\n");

	write (fd, "@00D000\r", 8);
	n = read (fd, buf, sizeof buf);
	if (!n)
		fprintf(stderr, "Read failed\n");

//	char pints[10];
	enum KeypadButton pincode[3];
	int count=0;
	int pressed = 0;

	int x;
	int col;
	for (x = 0; x < 1000000; x++)
	{
		col = x % 4;
		printf("col: %d", col);
		selectCol(col);





		write (fd, "@00P1?\r", 7);
		n = read (fd, buf, sizeof buf);
		if (!n)
			fprintf(stderr, "Read failed\n");
		usleep(1000);

		if (strcmp(buf,"!0000\r") != 0)
		{
			if (pressed)
			{
				// ignore button held down
				continue;
			}
			// something was pressed
			int row = 0;
			int col = 0;
			enum KeypadButton button = KEY_1;
			
			row = getRowNumber(&buf[4]);

			int j;
			for (j=1; j<5; j++)
			{
				if (checkColumnForPressed(j))
				{
					col = j;
					break;
				}
			}
			
			printf("col: %d\n", col);
			button = getButton(row,col);
			printf("button: %d\n", button);

		/*	if (!buttonIsNumeric(button))
				continue;

			showButton(button);

			pincode[count] = getRealNumber(button);
			pressed = 1;
			count++;
			if (count > 3)
				break;*/
		}
		else
		{
			printf("nothing\n");
			pressed = 0;
		}		
		usleep(1000);
	}
	int disp;
	for (disp = 0; disp < count; disp++)
	{
		printf("%d", pincode[disp]);
	}
	printf("\n");
	return 0;
}

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
		usleep(1000);

	// write a 1
	write (fd, "@00P206\r", 8);
	if (!n)
		fprintf(stderr, "Read failed\n");
	n = read (fd, buf, sizeof buf);
	usleep(1000);
	}
	else if (col == 1)
	{
		write (fd, "@00P002\r", 8);
		n = read (fd, buf, sizeof buf);
		if (!n)
			fprintf(stderr, "Read failed\n");
		usleep(1000);

	// write a 1
	write (fd, "@00P25B\r", 8);
	if (!n)
		fprintf(stderr, "Read failed\n");
	n = read (fd, buf, sizeof buf);
	usleep(1000);
	}
	else if (col == 2)
	{
		write (fd, "@00P004\r", 8);
		n = read (fd, buf, sizeof buf);
		if (!n)
			fprintf(stderr, "Read failed\n");
		usleep(1000);

	// write a 1
	write (fd, "@00P24F\r", 8);
	if (!n)
		fprintf(stderr, "Read failed\n");
	n = read (fd, buf, sizeof buf);
	usleep(1000);
	}
	else if (col == 3)
	{
		write (fd, "@00P008\r", 8);
		n = read (fd, buf, sizeof buf);
		if (!n)
			fprintf(stderr, "Read failed\n");
		usleep(1000);

	// write a 1
	write (fd, "@00P266\r", 8);
	if (!n)
		fprintf(stderr, "Read failed\n");
	n = read (fd, buf, sizeof buf);
	usleep(1000);
	}

	write (fd, "@00P200\r", 8);
	if (!n)
		fprintf(stderr, "Read failed\n");
	n = read (fd, buf, sizeof buf);
	usleep(50);
}

int getRowNumber(char* ch)
{
	int row = 0;
	long int buttonValueRead;
	//last digit will be 1,2,4 or 8
	/*switch (buf[4])
	{
	case '1': // number could be 1 2 3 F
		row = 1; break;
	case '2': // number could be 4 5 6 E
		row = 2; break;
	case '4': // number could be 7 8 9 D
		row = 3; break;
	default: // number could be A 0 B C	
		row = 4; break;
	}*/
	// printf("buf4 %c\trow: %d\n", ch], row);

	buttonValueRead = strtol(ch, NULL, 16);
//	printf("hex: %x, %li\n", buttonValueRead, buttonValueRead);
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
	else /*(buttonValueRead & 0x8)*/
	{
		row = 4;
	}
	return row;
}

// return 1 if a button pressed else 0
int checkColumnForPressed(int column)
{
	char pints[10];
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
	// printf("column: %d\n", column);

	// set port0 to only read 1 column
	sprintf (pints, "@00P00%1x\r", colHex);
	write (fd, pints, 8);
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

enum KeypadButton getButton(int row, int col)
{
	return ((row - 1) * 4) + col;
}



/*void showButton(enum KeypadButton button)
{
	char buf [100];
	char pints[10];
//	char buttonChar = buttonStrings[button][0];
	enum segChar hexToPrint = getHexRepresentation(button);
	sprintf (pints, "@00P2%02x\r", hexToPrint);
	printf("Displaying: %02x\n", hexToPrint);
	printf("PINTS %s\n", pints);
	write (fd, pints, 8);
	read (fd, buf, sizeof buf);
	usleep(1000);
}*/


