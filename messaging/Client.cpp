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
*		 g++ -I -Wall -o client Client.cpp PIO_Library.c -lm
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
#include "include.h"
using namespace std;
/********************************************************
 * Class: StatusMessage, Created by: Mark Randall	*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: To store the outgoing message to send to	*
 * the client.						*
 ********************************************************/
class StatusMessage
{
public:
	StatusMessage();
	~StatusMessage();

	void setCode(int code);
	void setTitle(string title);
	void setMessage(string message);
	void setProtocol(string protocol);

	int getCode();
	string getTitle();
	string getMessage();
	string getProtocol();
private:
	int statusCode;
	string statusTitle;
	string statusMessage;
	string statusProtocol;
};

StatusMessage::StatusMessage()
{
	statusCode = 0;
	statusTitle = "";
	statusMessage = "";
	statusProtocol = "";
}
StatusMessage::~StatusMessage()
{	}

void StatusMessage::setCode(int c)
{
	statusCode = c;
}
void StatusMessage::setTitle(string t)
{
        statusTitle = t;
}
void StatusMessage::setMessage(string m)
{
        statusMessage = m;
}
void StatusMessage::setProtocol(string p)
{
	statusProtocol = p;
}

int StatusMessage::getCode()
{
	return statusCode;
}
string StatusMessage::getTitle()
{
        return statusTitle;
}
string StatusMessage::getMessage()
{
        return statusMessage;
}
string StatusMessage::getProtocol()
{
	return statusProtocol;
}
//End of StatusMessage class

/********************************************************
 * Function: splitMessage(), Created by: Mark Randall	*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: Takes a string, start position (int) and a	*
 * end position (int) and copies the characters within	*
 * those positions, and returns the new string.		*
 * e.g. message = "POST HTTP/1.1"			*
 * 	parseStartPos = 5				*
 * 	parseEndPos = 13				*
 * 	returnMsg = "HTTP/1.1"				*
 ********************************************************/
string splitMessage(string message, int parseStartPos, int parseEndPos)
{
	string returnMsg = message.substr(parseStartPos, parseEndPos);
	
	return returnMsg;
}

/********************************************************
 * Function: parseResponseMsg()				*
 * Created by: Mark Randall				*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: To seperate out the individual strings and	*
 * int from the response message. The message should	*
 * contain a string (protocol), followed by a space,	*
 * followed by a int (code), followed by a space, next	*
 * is the title string, which we are storing and	*
 * checking the contents in the checkMessages()		*
 * function.						*
 * We then need to check there is 2 \r\n's, followed	*
 * by the last string, the message.			*
 * 							*
 * The first for loop handles finding the \r\n (CRLF)	*
 * and has appropriate logic for finding more than 2,	*
 * or less than 2 (i.e response message is wrong).	*
 * We then take the message at the end of the string,	*
 * and add it into statusMessage->message.		*
 * The next for loop does the same, but for spaces,	*
 * we add the remaining protocol, code and title into	*
 * statusMessage, for checking in checkMessages()	*
 * e.g. message = "HTTP/1.1 200 OK\r\n\r\nPurchased " +	*
 * 		  "code accepted"			*
 * 	statusMessage.getMessage() = "Purchased " +	*
 * 				     "code accepted"	*
 * 	statusMessage.getProtocol() = "HTTP/1.1"	*
 * 	statusMessage.getCode() = 200			*
 * 	statusMessage.getTitle() = "OK"			*
 ********************************************************/
StatusMessage parseResponseMsg(string message)
{
	StatusMessage statusMessage;
	int i = 0;
	int foundCRLF = 0,prevCRLFPos = 0;
	int foundSpace = 0,prevSpacePos = 0;
	int converted = 0;
	string seperateLines[2];

	for(i=message.find("\r\n",0); i!=string::npos; i=message.find("\r\n",i))
	{
		if(foundCRLF < 2)
		{
			seperateLines[foundCRLF] =
			  splitMessage(message,prevCRLFPos,(i-prevCRLFPos));
		}
		else if(foundCRLF >= 2)
		{
			//appropriate error message - 400
			cout << "Too many CRLF's in message" << endl;
		}

		i = i + 2;
		foundCRLF++;
		prevCRLFPos = i;
	}
	if(foundCRLF < 2)
	{
		//response message wrong
		cout << "Message doesn't contain enough \"\\r\\n\"" << endl;
	}
	statusMessage.setMessage(splitMessage(message,prevCRLFPos,
	  message.length()));
	
	i=0;
	for(i=seperateLines[0].find(" ",0);
	    ((i!=string::npos) || (foundSpace == 2));
	    i=seperateLines[0].find(" ",i))
	{
		if(foundSpace == 0)
		{
			statusMessage.setProtocol(splitMessage(
			  seperateLines[0],prevSpacePos,(i-prevSpacePos)));
		}
		else if(foundSpace == 1)
		{
			if(!(stringstream(splitMessage(seperateLines[0],prevSpacePos,
			  (i-prevSpacePos))) >> converted))
			{
				cout << "Failed to convert code to int" << endl;
			}
			statusMessage.setCode(converted);
		}

		i++;
		foundSpace++;
		prevSpacePos = i;
	}
	if(foundSpace == 0)
	{
		//appropriate error message - 400
		cout << "Message doesn't contain any \" \"" << endl;
	}
	statusMessage.setTitle(splitMessage(
	  seperateLines[0],prevSpacePos,seperateLines[0].length()));
	
	return statusMessage;
}

/********************************************************
 * Function: checkMessages(), Created by: Mark Randall	*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: To check the protocol, code and title are	*
 * correct and valid, before deciding what further	*
 * action to take. If they are valid, the FSM will move	*
 * to the next state.					*
 * Currently the function has been condensed for the	*
 * prototype stage, and therefore only has logic for	*
 * if the message back is a "OK" - true, or if it is	*
 * not what we want "INTERNAL SERVER ERROR" - false.	*
 ********************************************************/
StatusMessage checkMessages(StatusMessage aStatus)
{
	StatusMessage invalidResponse;
	string title = aStatus.getTitle();
	string protocol = aStatus.getProtocol();
	int code = aStatus.getCode();
	string message = aStatus.getMessage();	

	invalidResponse.setTitle("INTERNAL SERVER ERROR");
	invalidResponse.setProtocol("HTTP/1.1");
	invalidResponse.setCode(500);
	invalidResponse.setMessage("Server response was invalid format - Title incorrect");

	//check protocol
	if(protocol != "HTTP/1.1")
	{
		//invalid status protocol
		invalidResponse.setMessage("Server response was invalid format - Protocol incorrect");
		return invalidResponse;
	}
	
	//check status code
	/* This switch case has been designed to handle only *
	 * a "OK" message back (i.e. for prototype stage),   *
	 * the return type, and contents will need modifying *
	 * for the full version.			     */
	switch(code)
	{
		case 200:
		{
			if(title != "OK")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		case 202:
		{
		  	if(title != "ACCEPTED")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		case 400:
		{
		  	if(title != "BAD REQUEST")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		case 402:
		{
		  	if(title != "PAYMENT REQUIRED")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		case 403:
		{
		  	if(title != "FORBIDDEN")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		case 500:
		{
		  	if(title != "INTERNAL SERVER ERROR")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		case 503:
		{
		  	if(title != "SERVICE UNAVAILABLE")
			{
				//invalid status title
				return invalidResponse;
			}
			//go to next state of FSM
			return aStatus;
		}
		break;
		
		default:
		{
			//invalid status code
			invalidResponse.setMessage("Server response was invalid format - Code Invalid");
			return invalidResponse;
		}
		break;
	}

	return invalidResponse;
}
/* **********************Main Function*************************** *
*
*Function Name: int main(int argc, char *argv[])
*Purpose: This Function Starts and Initialize the Socket of Client.
*Send Message to Server.
*Date Created: 12/11/2013. Sami Giacaman
*
/* ************************************************************** */

int main(int argc, char *argv[])
{


//char *usbPIO = "/dev/ttyACM0"; included in PIO_library.c
char reply[10];
char check[10];

int fdKey, flag1 = 0, flag2 = 1; 
int delayTime = 1000;
char number[10];

//fdKey = open(usbPIO, O_RDWR | O_NOCTTY | O_NONBLOCK);

/*if(fdKey>0)
{
	printf("Open successfully\n");

	write(fdKey, "@00D000\r", 8);
	usleep(delayTime);
	read(fdKey, check, 4);
	usleep(delayTime);
	printf("\n%c%c%c",check[0],check[1],check[2]);

	write(fdKey, "@00D1FF\r", 8);
	usleep(delayTime);
	read(fdKey, check, 4);
	usleep(delayTime);
	printf("\n%c%c%c",check[0],check[1],check[2]);

	write(fdKey, "@00D200\r", 8);
	usleep(delayTime);
	read(fdKey, check, 4);
	usleep(delayTime);
	printf("\n%c%c%c",check[0],check[1],check[2]);
}
else
{
	perror(usbPIO);
	printf("Failed to Open Device\n");
}*/
/*UNCOMMENT!!!
    if(!Initialise_PIO(0x00,0xFF,0x00))
    {
    	printf("Failed to Initialise Device\n");
    	return 0;
    }
*/
    int numchar, portnum, socketfd, x = 0, fd, fdm;
    char buffer[MAXPATH], message[200];
    char command[MAXPATH+1];
    int statusCode = 0;
    StatusMessage responseMsg;
    string returnMsg;
    struct ifreq ifr;
    struct ifreq macAdd;
    struct sockaddr_in saddr; /* Struct to hold server's address. */
    struct hostent *ptrh;     /* Pointer to a host table entry. */

    	if (x==MAXPATH)
	{
		perror("\nToo Many Arguments"); /* Request Line is BIG.*/
		exit(0);
	}
	command[x] = '\0';
	command[x+1] = '\0';
    /* **************** */
    /* FOR TESTING ONLY */
    /* **************** */
    //sprintf(command,"propfind://localhost:9000/1"); 
    //sprintf(command,"connect://localhost:9000/3500"); 
    /* ************************************* */
    /* ************************************* */
    /* Allocate each argument with variable. */
    /* ************************************* */

    /* ************** */
    /* Create Socket. */
    /* ************** */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    { 
        perror("Error: Socket Creation");
        exit(0);
    }
/*
    if (ptrh == NULL) 
    {
        perror("Error: Invalid Host");
        exit(0);
    }
*/

    /* *************** */
       portnum = 7000;
       ptrh = gethostbyname("localhost");
    /* *************** */

    if (ptrh == NULL) 
    {
        perror("Error: Invalid Host");
        exit(0);
    }

    bzero((char *) &saddr, sizeof(saddr)); /* Clear sockaddr Structure. */
    saddr.sin_family = AF_INET;            /* Set Family to Internet. */
    /* Copy Host Name to Equivalent IP address and copy to saddr. */
    bcopy((char *)ptrh->h_addr, (char *)&saddr.sin_addr.s_addr, ptrh->h_length);
    saddr.sin_port = htons(portnum);       /* Use Specified Port Number. */
    /* Connect Socket to the Specified Server.*/
    if (connect(socketfd,(struct sockaddr *) &saddr,sizeof(saddr)) < 0) 
    {
        perror("Error");
        exit(0);
    }
    bzero(buffer,MAXPATH); /* Clear. */   
    /* Get IP Address */
    /******************/
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
    ioctl(fd, SIOCGIFADDR, &ifr);
    /******************/
    /******************/
    /* Get MAC Address */
    /*******************/
    fdm =socket(PF_INET,SOCK_DGRAM, 0);
    memset(&macAdd, 0x00, sizeof(macAdd));
    strcpy(macAdd.ifr_name, "eth0");
    ioctl(fdm, SIOCGIFHWADDR, &macAdd);
    close(fdm);
    ioctl(fd, SIOCGIFHWADDR, &macAdd);

	/* Keypad Routine */


	/*if(!Initialise_PIO(0x00,0xFF,0x00))
	{
		printf("Failed to Initialise Device\n");
		return 0;
	}*/

	//printf("Open successfully..\n");
/*UNCOMMENT!!!
 while(flag1 < 4) 
 {
	while(1)
	{
		Write_PIO(0,1);
		strcpy(reply,Read_PIO(1));

		switch(reply[4])
		{
			case '1':
				//printf("\nFirst Column First Button [1]");
				Write_PIO(2,LED_1);
				flag1 + 1;
				std::cout << number << '1' ;
				
				
				break;
			break;

			case '2':
				//printf("\nFirst Column Second Button [4]");
				Write_PIO(2,LED_4);
				flag1 + 1;
				std::cout << number << '4' ;
			break;
		
			case '4':
				//printf("\nFirst Column Third Button [7]");
				Write_PIO(2,LED_7);
				flag1 + 1;
				std::cout << number << '7' ;
			break;

			case '8':
				//printf("\nFirst Column Fourth Button [A]");
				Write_PIO(2,LED_A);
			break;
		}	

		Write_PIO(0,2);
		strcpy(reply,Read_PIO(1));
	
		switch(reply[4])
		{
			case '1':
				//printf("\nSecond Column First Button [2]");
				Write_PIO(2,LED_2);
				flag1 + 1;
				std::cout << number << '2' ;
			break;

			case '2':
				//printf("\nSecond Column Second Button [5]");
				Write_PIO(2,LED_5);
				flag1 + 1;
				std::cout << number << '5' ;
			break;
		
			case '4':
				//printf("\nSecond Column Third Button [8]");
				Write_PIO(2,LED_8);
				flag1 + 1;
				std::cout << number << '8' ;
			break;

			case '8':
				//printf("\nSecond Column Fourth Button [0]");
				Write_PIO(2,LED_0);
				flag1 + 1;
				std::cout << number << '0' ;
			break;
		}

		Write_PIO(0,4);
		strcpy(reply,Read_PIO(1));
	
		switch(reply[4])
		{
			case '1':
				//printf("\nThird Column First Button [3]");
				Write_PIO(2,LED_3);
				flag1 + 1;
				std::cout << number << '3' ;

			break;

			case '2':
				//printf("\nThird Column Second Button [6]");
				Write_PIO(2,LED_6);
				flag1 + 1;
				std::cout << number << '6' ;
			break;
		
			case '4':
				//printf("\nThird Column Third Button [9]");
				Write_PIO(2,LED_9);
				flag1 + 1;
				std::cout << number << '9' ;
			break;

			case '8':
				//printf("\nThird Column Fourth Button [B]");
				Write_PIO(2,LED_B);
			break;
		}

		Write_PIO(0,8);
		strcpy(reply,Read_PIO(1));
	
		switch(reply[4])
		{
			case '1':
				//printf("\nFourth Column First Button [F]");
				Write_PIO(2,LED_F);
			break;

			case '2':
				//printf("\nFourth Column Second Button [E]");
				Write_PIO(2,LED_E);
			break;
		
			case '4':
				//printf("\nFourth Column Third Button [D]");
				Write_PIO(2,LED_D);
			break;

			case '8':
				//printf("\nFourth Column Fourth Button [C]");
				Write_PIO(2,LED_C);
			break;
		}
	}
 }
 flag1 = 0;
*/
strcpy(number,"4242");
    sprintf(message,"POST HTTP/1.1\r\n\r\nHost: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\r\n#%s",(unsigned char)macAdd.ifr_hwaddr.sa_data[0],(unsigned char)macAdd.ifr_hwaddr.sa_data[1],(unsigned char)macAdd.ifr_hwaddr.sa_data[2],(unsigned char)macAdd.ifr_hwaddr.sa_data[3],(unsigned char)macAdd.ifr_hwaddr.sa_data[4],(unsigned char)macAdd.ifr_hwaddr.sa_data[5],number);
    close(fd);
/*,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)*/
    /*******************/
    /*******************/
    sprintf(buffer,"%s",message);
    printf("\n-----------------------------------------------");
    printf("\nRequest Sent:");
    printf("\n-----------------------------------------------");
    printf("\n%s",buffer);
    printf("\n-----------------------------------------------\n\n");
    /* Write Data to Socket, Read Data From it and Display. */
    numchar = write(socketfd,buffer,strlen(buffer));
    if (numchar < 0)
    { 
        perror("Error: Writing to Socket");
        exit(0);
    }

    bzero(buffer,MAXPATH); /* Clear. */
    numchar = read(socketfd,buffer,MAXPATH);
    if (numchar < 0) 
    {
    	perror("Error: Reading from Socket");
        exit(0);
    }
    printf("\n-----------------------------------------------");
    printf("\nMessage Received:");
    printf("\n-----------------------------------------------");
    printf("\n%s",buffer);
    printf("\n-----------------------------------------------\n\n");

    string testMessage(buffer);
    StatusMessage statusMsg;
    statusMsg = parseResponseMsg(testMessage);
    responseMsg = checkMessages(statusMsg);

    if(responseMsg.getCode() == 200)
    {
	while(1)
	{
	    /* Close Socket. */
	    close(socketfd);
	    /* ************** */
	    /* Create Socket. */
	    /* ************** */
	    socketfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (socketfd < 0)
	    { 
		perror("Error: Socket Creation");
		exit(0);
	    }

	    /* *************** */
	       portnum = 7000;
	       ptrh = gethostbyname("localhost");
	    /* *************** */
		
	    if (ptrh == NULL) 
	    {
		perror("Error: Invalid Host");
		exit(0);
	    }
	    bzero((char *) &saddr, sizeof(saddr)); /* Clear sockaddr Structure. */
	    saddr.sin_family = AF_INET;            /* Set Family to Internet. */
	    /* Copy Host Name to Equivalent IP address and copy to saddr. */
	    bcopy((char *)ptrh->h_addr, (char *)&saddr.sin_addr.s_addr, ptrh->h_length);
	    saddr.sin_port = htons(portnum);       /* Use Specified Port Number. */
	    /* Connect Socket to the Specified Server.*/
	    if (connect(socketfd,(struct sockaddr *) &saddr,sizeof(saddr)) < 0) 
	    {
		perror("Error");
		exit(0);
	    }
	    bzero(buffer,MAXPATH); /* Clear. */   
	    /* Get IP Address */
	    /******************/
	    fd = socket(AF_INET, SOCK_DGRAM, 0);
	    ifr.ifr_addr.sa_family = AF_INET;
	    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
	    ioctl(fd, SIOCGIFADDR, &ifr);
	    /******************/
	    /******************/
	    /* Get MAC Address */
	    /*******************/
	    fdm =socket(PF_INET,SOCK_DGRAM, 0);
	    memset(&macAdd, 0x00, sizeof(macAdd));
	    strcpy(macAdd.ifr_name, "eth0");
	    ioctl(fdm, SIOCGIFHWADDR, &macAdd);
	    close(fdm);
	    ioctl(fd, SIOCGIFHWADDR, &macAdd);
	    /******************/
	    /******************/
	    /* Keypad Section */
	    /******************/
	    /******************/
/*UNCOMMENT OUT!!!
while(flag2 < 4) // increment with each succ press
 {
	while(1)
	{
		Write_PIO(0,1);
		strcpy(reply,Read_PIO(1));

		switch(reply[4])
		{
			case '1':
				//printf("\nFirst Column First Button [1]");
				Write_PIO(2,LED_1);
				flag1 + 1;
				std::cout << number << '1' ;
				
				
				break;
			break;

			case '2':
				//printf("\nFirst Column Second Button [4]");
				Write_PIO(2,LED_4);
				flag1 + 1;
				std::cout << number << '4' ;
			break;
		
			case '4':
				//printf("\nFirst Column Third Button [7]");
				Write_PIO(2,LED_7);
				flag1 + 1;
				std::cout << number << '7' ;
			break;

			case '8':
				//printf("\nFirst Column Fourth Button [A]");
				Write_PIO(2,LED_A);
			break;
		}	

		Write_PIO(0,2);
		strcpy(reply,Read_PIO(1));
	
		switch(reply[4])
		{
			case '1':
				//printf("\nSecond Column First Button [2]");
				Write_PIO(2,LED_2);
				flag1 + 1;
				std::cout << number << '2' ;
			break;

			case '2':
				//printf("\nSecond Column Second Button [5]");
				Write_PIO(2,LED_5);
				flag1 + 1;
				std::cout << number << '5' ;
			break;
		
			case '4':
				//printf("\nSecond Column Third Button [8]");
				Write_PIO(2,LED_8);
				flag1 + 1;
				std::cout << number << '8' ;
			break;

			case '8':
				//printf("\nSecond Column Fourth Button [0]");
				Write_PIO(2,LED_0);
				flag1 + 1;
				std::cout << number << '0' ;
			break;
		}

		Write_PIO(0,4);
		strcpy(reply,Read_PIO(1));
	
		switch(reply[4])
		{
			case '1':
				//printf("\nThird Column First Button [3]");
				Write_PIO(2,LED_3);
				flag1 + 1;
				std::cout << number << '3' ;

			break;

			case '2':
				//printf("\nThird Column Second Button [6]");
				Write_PIO(2,LED_6);
				flag1 + 1;
				std::cout << number << '6' ;
			break;
		
			case '4':
				//printf("\nThird Column Third Button [9]");
				Write_PIO(2,LED_9);
				flag1 + 1;
				std::cout << number << '9' ;
			break;

			case '8':
				//printf("\nThird Column Fourth Button [B]");
				Write_PIO(2,LED_B);
			break;
		}

		Write_PIO(0,8);
		strcpy(reply,Read_PIO(1));
	
		switch(reply[4])
		{
			case '1':
				//printf("\nFourth Column First Button [F]");
				Write_PIO(2,LED_F);
			break;

			case '2':
				//printf("\nFourth Column Second Button [E]");
				Write_PIO(2,LED_E);
			break;
		
			case '4':
				//printf("\nFourth Column Third Button [D]");
				Write_PIO(2,LED_D);
			break;

			case '8':
				//printf("\nFourth Column Fourth Button [C]");
				Write_PIO(2,LED_C);
			break;
		}
	}
 }
	    flag2 = 0; //reset
	    /******************/
	    /******************/
strcpy(number,"6969");
	    sprintf(message,"GET HTTP/1.1\r\n\r\nHost: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\r\n#%s",(unsigned char)macAdd.ifr_hwaddr.sa_data[0],(unsigned char)macAdd.ifr_hwaddr.sa_data[1],(unsigned char)macAdd.ifr_hwaddr.sa_data[2],(unsigned char)macAdd.ifr_hwaddr.sa_data[3],(unsigned char)macAdd.ifr_hwaddr.sa_data[4],(unsigned char)macAdd.ifr_hwaddr.sa_data[5],number);
	    close(fd);
	    /*******************/
	    /*******************/
	    sprintf(buffer,"%s",message);
	    printf("\n-----------------------------------------------");
	    printf("\nRequest Sent:");
	    printf("\n-----------------------------------------------");
	    printf("\n%s",buffer);
	    printf("\n-----------------------------------------------\n\n");
	    /* Write Data to Socket, Read Data From it and Display. */
	    numchar = write(socketfd,buffer,strlen(buffer));
	    if (numchar < 0)
	    { 
		perror("Error: Writing to Socket");
		exit(0);
	    }

	    bzero(buffer,MAXPATH); /* Clear. */
	    numchar = read(socketfd,buffer,MAXPATH);
	    if (numchar < 0) 
	    {
	    	perror("Error: Reading from Socket");
		exit(0);
	    }
	    printf("\n-----------------------------------------------");
	    printf("\nMessage Received:");
	    printf("\n-----------------------------------------------");
	    printf("\n%s",buffer);
	    printf("\n-----------------------------------------------\n\n");

	    string testMessage2(buffer);
	    statusMsg = parseResponseMsg(testMessage2);
	    responseMsg = checkMessages(statusMsg);

	    if(responseMsg.getCode() == 200)
	    {
		printf("Streaming...\n");
		//gstreamer goes here :)
		//ideally as a forked process (so we can wait for new code entered (go back through loop))
		//if we have a stream currently active, and get to here. Kill the child, and act as normal
	    }
	    else if(responseMsg.getCode() == 400)
	    {
		printf("Audio Code issue\n");
		//audio code does not exist (nothing assigned), alert user, and loop back round
		printf("\nThis audio code is unavailable/does not exist.\nPlease try another\n");
	    }
	    else if(responseMsg.getCode() == 500)
	    {
		/*Sleep for 5 Seconds, Check if the socket is still connected
		 *If not, recreate socket and then attempt a response resend.
		 *To be implemented. Not for prototype.*/

		// if we get 500 again, then serious server/client issue with messaging/
		// sockets persist
		printf("\nIAM DEAD. 500");
   	    	exit(0);
		//^To be implemented by Sami...
	    }
	}
    }
    else if(responseMsg.getCode() == 402)
    {
	//display message stating code has not been purchased
    }
    else if(responseMsg.getCode() == 403)
    {
	//display message stating code is currently being used/is unavailable
    }
    else if(responseMsg.getCode() == 500)
    {
	/*Sleep for 5 Seconds, Check if the socket is still connected
	 *If not, recreate socket and then attempt a response resend.
	 *To be implemented. Not for prototype.*/

	// if we get 500 again, then serious server/client issue with messaging/
	// sockets persist
	    printf("\nIAM DEAD. 500");
   	    exit(0);
	//^To be implemented by Sami...
    }
    /* Close Socket. */
    close(socketfd);
    /* Terminate Client Program. */
    return 0;
}

/* ************************************************************** */
/* ************************************************************** */
