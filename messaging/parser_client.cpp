/*********************************************** -HTIGCP- ******************************************************
*
*Author: Mark Randall & Sami Giacaman.
*Date Created: 12/11/2013.
*Date Amended: 26/11/2013
*Purpose: Solution for Hyper Text iGep Control Protocol
*
*Notes: 
*
*Program Description: 
*
*How to Compile: g++ -I -Wall -o server Server.c -lpthread
*		 g++ -I -Wall -o client Client.c
*OR
*
*How to Compile: gcc -I -Wall -o server Server.c -lpthread
*		 gcc -I -Wall -o client Client.c
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
bool checkMessages(StatusMessage aStatus)
{
	StatusMessage statusMsg;
	bool messageFalse = false;
	string title = aStatus.getTitle();
	string protocol = aStatus.getProtocol();
	int code = aStatus.getCode();
	string message = aStatus.getMessage();

	//check protocol
	if(protocol != "HTTP/1.1")
	{
		//invalid status protocol
		return messageFalse;
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
				return messageFalse;
			}
			//go to next state of FSM
			return true;
		}
		break;
		
		case 202:
		{
			/*
		  	if(title != "ACCEPTED")
			{
				//invalid status title
				return messageFalse;
			}
			*/
			//go to next state of FSM
			return messageFalse;
		}
		break;
		
		case 400:
		{
			/*
		  	if(title != "BAD REQUEST")
			{
				//invalid status title
				return messageFalse;
			}
			*/
			//go to next state of FSM
			return messageFalse;
		}
		break;
		
		case 402:
		{
			/*
		  	if(title != "PAYMENT REQUIRED")
			{
				//invalid status title
				return messageFalse;
			}*/
			//go to next state of FSM
			return messageFalse;
		}
		break;
		
		case 403:
		{
			/*
		  	if(title != "FORBIDDEN")
			{
				//invalid status title
				return messageFalse;
			}
			*/
			//go to next state of FSM
			return messageFalse;
		}
		break;
		
		case 500:
		{
			/*
		  	if(title != "INTERNAL SERVER ERROR")
			{
				//invalid status title
				return messageFalse;
			}
			*/
			//go to next state of FSM
			return messageFalse;
		}
		break;
		
		case 503:
		{
			/*
		  	if(title != "SERVICE UNAVAILABLE")
			{
				//invalid status title
				return messageFalse;
			}*/
			//go to next state of FSM
			return messageFalse;
		}
		break;
		
		default:
		{
			//invalid status code
			return messageFalse;
		}
		break;
	}

	return messageFalse;
}

/* **********************Main Fucntion*************************** *
*
*Function Name: int main(int argc, char *argv[])
*Purpose: This Function Starts and Initialize the Socket of Client.
*Send Message to Server.
*Date Created: 12/11/2013. Sami Giacaman
*
/* ************************************************************** */

int main(int argc, char *argv[])
{

    int numchar, portnum, socketfd, x = 0;
    char buffer[MAXPATH], message[200];
    char command[MAXPATH+1];
    int statusCode = 0;
    bool startStreaming = false;
    string returnMsg;

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
    if (ptrh == NULL) 
    {
        perror("Error: Invalid Host");
        exit(0);
    }

    /* *************** */
       portnum = 9000;
       ptrh = gethostbyname("localhost");
    /* *************** */

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
 
    sprintf(message,"POST HTTP/1.1\r\n\r\n#5412");
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
    cout << "What is testMessage:" << testMessage << endl;
    StatusMessage statusMsg;
    statusMsg = parseResponseMsg(testMessage);
    cout << "Message parsed, protocol is: \"" << statusMsg.getProtocol() <<
		"\"" << endl;
    cout << "Code is: \"" << statusMsg.getCode() << "\"" << endl;
    cout << "Title is: \"" << statusMsg.getTitle() << "\"" << endl;
    cout << "Message is: \"" << statusMsg.getMessage() << "\"" << endl;
    startStreaming = checkMessages(statusMsg);
	
    if(startStreaming)
    {
	//start gstreamer
	cout << "Starting GStreamer..." << endl;
    }
    else
    {
	//attempt to resend request message
    }
    /* Close Socket. */
    close(socketfd);
    /* Terminate Client Program. */
    return 0;
}

/* ************************************************************** */
/* ************************************************************** */
