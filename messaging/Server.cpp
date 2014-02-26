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
 * Class: ParsedMessage, Created by: Mark Randall	*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: To store the individual components of the	*
 * incoming client message, that can then be handled	*
 * by checkMessages()					*
 ********************************************************/
class ParsedMessage
{
public:
	ParsedMessage();
	~ParsedMessage();

	void setCommand(string command);
	void setProtocol(string protocol);
	void setCode(int code);
	void setStatusObject(StatusMessage aSM);

	string getCommand();
	string getProtocol();
	int getCode();
	StatusMessage getStatusObject();
private:
	//Parsed message variables
	string command;
	string protocol;
	int code;

	/* Error message object, to be used in: parseRequestMsg() *
	 * if we fail to parse the incoming client message	  */
	StatusMessage aStatusMessage;
};

ParsedMessage::ParsedMessage()
{
	command = "";
	protocol = "";
	code = 0;
}
ParsedMessage::~ParsedMessage()
{	}

void ParsedMessage::setCommand(string c)
{
	command = c;
}
void ParsedMessage::setProtocol(string p)
{
        protocol = p;
}
void ParsedMessage::setCode(int c)
{
        code = c;
}
void ParsedMessage::setStatusObject(StatusMessage aSM)
{
	aStatusMessage = aSM;
}

string ParsedMessage::getCommand()
{
	return command;
}
string ParsedMessage::getProtocol()
{
        return protocol;
}
int ParsedMessage::getCode()
{
        return code;
}
StatusMessage ParsedMessage::getStatusObject()
{
	return aStatusMessage;
}
//End of ParsedMessage class

/********************************************************
 * Function: splitMessage(), Created by: Mark Randall	*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: Takes a string, start position (int) and a	*
 * end position (int) and copies the characters within	*
 * those positions, and returns the new string.		*
 * e.g. message = "POST HTTP/1.1 #4000"			*
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
 * Function: parseRequestMsg()				*
 * Created by: Mark Randall				*
 * Date: 20th September 2013				*
 * 							*

 ********************************************************/
ParsedMessage parseRequestMsg(string message)
{
	ParsedMessage parsedMessage;
	StatusMessage statMsg;
	statMsg.setCode(400);
	statMsg.setTitle("BAD REQUEST");
	statMsg.setProtocol("HTTP/1.1");
	
	int i = 0;
	int foundCRLF = 0,prevCRLFPos = 0;
	int foundSpace = 0,prevSpacePos = 0;
	int converted;
	string seperateLines[2];

	for(i=message.find("\r\n",0); i!=string::npos; i=message.find("\r\n",i))
	{
		if(foundSpace < 2)
		{
			seperateLines[foundCRLF] =
			  splitMessage(message,prevCRLFPos,(i-prevCRLFPos));
		}
		else if(foundSpace >= 2)
		{
			statMsg.setMessage("Request had too many CRLF's");
			cout << statMsg.getMessage() << endl;
			parsedMessage.setStatusObject(statMsg);
			return parsedMessage;
		}

		i = i + 2;
		foundCRLF++;
		prevCRLFPos = i;
	}
	if(foundCRLF == 0)
	{
		statMsg.setMessage("Request had no CRLF's");
		cout << statMsg.getMessage() << endl;
		parsedMessage.setStatusObject(statMsg);
		return parsedMessage;
	}
	
	i=message.find("#",0);
	if(i==string::npos)
	{
		statMsg.setMessage("Request contained no \"#\"");
		cout << statMsg.getMessage() << endl;
		parsedMessage.setStatusObject(statMsg);
		return parsedMessage;
	}
	if(!(stringstream(message.substr((++i),message.length())) >> converted))
	{
		statMsg.setMessage("Request contained invalid code");
		cout << statMsg.getMessage() << endl;
		parsedMessage.setStatusObject(statMsg);
		return parsedMessage;
	}
	parsedMessage.setCode(converted);
	
	i=0;
	for(i=seperateLines[0].find(" ",0); i!=string::npos; i=seperateLines[0].find(" ",i))
	{
		if(foundSpace == 0)
		{
			parsedMessage.setCommand(splitMessage(
			  seperateLines[0],prevSpacePos,(i-prevSpacePos)));
		}
		else if(foundSpace >= 1)
		{
			statMsg.setMessage("Request contained too many \" \"'s");
			cout << statMsg.getMessage() << endl;
			parsedMessage.setStatusObject(statMsg);
			return parsedMessage;
		}

		i++;
		foundSpace++;
		prevSpacePos = i;
	}
	if(foundSpace == 0)
	{
		statMsg.setMessage("Request contained no \" \"'s");
		cout << statMsg.getMessage() << endl;
		parsedMessage.setStatusObject(statMsg);
		return parsedMessage;
	}
	parsedMessage.setProtocol(splitMessage(
	  seperateLines[0],prevSpacePos,seperateLines[0].length()));
	
	statMsg.setCode(200);
	statMsg.setTitle("OK");
	statMsg.setMessage("Starting Streaming...");
	parsedMessage.setStatusObject(statMsg);

	return parsedMessage;
}

/********************************************************
 * Function: checkMessage(), Created by: Mark Randall	*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: checkMessage() takes the ParsedMessage	*
 * object variables (e.g. aParsed.getCommand(),		*
 * aParsed.getProtocol(), aParsed.getCode()), and	*
 * checks the content of these are valid.		*
 * Once command and protocol are checked, and the code	*
 * is 4 digits, we can send an appropriate SQL script	*
 * to the SQL database to check that code is authorised	*
 * or, if the code links to a audio file to stream.	*
 * e.g.	aParsed.getCommand() = "POST"			*
 * 	aParsed.getProtocol() = "HTTP/1.1"		*
 * 	aParsed.getCode() = 4000			*
 * 	statusMsg.getCode() = 200			*
 * 	statusMsg.getTitle() = "OK"			*
 * 	statusMsg.getMessage() = "Purchased code " +	*
 * 				 "accepted"		*
 ********************************************************/
StatusMessage checkMessages(ParsedMessage aParsed)
{
	StatusMessage statusMsg;
	statusMsg = aParsed.getStatusObject();
	
	string command = aParsed.getCommand();
	string protocol = aParsed.getProtocol();
	int code = aParsed.getCode();

	//check protocol
	if(protocol != "HTTP/1.1")
	{
		statusMsg.setCode(400);
		statusMsg.setTitle("BAD REQUEST");
		statusMsg.setMessage("Invalid protocol");
		return statusMsg;
	}

	//check that code is between 0 - 9,000, i.e cant have over 4 digits
	if((code < 0) || (code > 9999))
	{
		statusMsg.setCode(400);
		statusMsg.setTitle("BAD REQUEST");
		statusMsg.setMessage("Invalid code");
		return statusMsg;
	}
	
	//check command
	if(command == "POST")
	{
		//send SQL script, to find data for the code, on the purchased codes table
	}
	else if(command == "GET")
	{
		//send SQL script, to find the audio file via the code, on the playback table
	}
	else if(command == "DELETE")
	{
		//send SQL script, to remove authorised code, from the purchased codes table
	}
	else
	{
		statusMsg.setCode(400);
		statusMsg.setTitle("BAD REQUEST");
		statusMsg.setMessage("Invalid command");
		return statusMsg;
	}

	return statusMsg;
}

/********************************************************
 * Function: createReturnMessage()			*
 * Created by: Mark Randall				*
 * Date: 20th September 2013				*
 * 							*
 * Purpose: Takes the StatusMessage variables, and	*
 * concatenates them together into a string to send	*
 * back to the client.					*
 * e.g.	statusMsg.getCode() = 200			*
 * 	statusMsg.getTitle() = "OK"			*
 * 	statusMsg.getMessage() = "Purchased code " +	*
 * 				 "accepted"		*
 * 	returnStr = "200 OK Purchased code accepted"	*
 ********************************************************/
string createReturnMessage(StatusMessage statusMsg)
{
	string returnStr;
	stringstream convert;

	if(!(convert << statusMsg.getCode()))
	{
		convert << 500;
		statusMsg.setTitle("INTERNAL SERVER ERROR");
		statusMsg.setMessage("Failed to create response message");
	}
		
	returnStr = returnStr + "HTTP/1.1 ";
	returnStr = returnStr + convert.str();
	returnStr = returnStr + " " + statusMsg.getTitle();
	returnStr = returnStr + "\r\n\r\n" + statusMsg.getMessage();

	return returnStr;
}

/* **********************Main Fucntion*************************** *
*
*Function Name: int main(int argc, char *argv[]);
*Purpose: This Function Starts and Initialize the Socket of Server.
*Listen to Socket From Clients.
*Date Created: 12/11/2013. Sami Giacaman
*
/* ************************************************************** */
int main(int argc, char *argv[])
{
    char buffer[MAXPATH];
    int i, portnum, socketfd, socketfd2, calen, activeThread = 0;
    pid_t pid;
    int statusCode = 0;
	
    struct sockaddr_in caddr; /* Struct to hold client's address. */
    struct sockaddr_in saddr; /* Struct to hold server's address. */
    
    /* ********************* */
    /* If Execution is Wrong */
    /* ********************* */
    if (argc != 2) 
    {
       fprintf(stderr,"Execute As Follow: %s <Port Number>\n", argv[0]);
       exit(0);
    }

    /* Allocate argument with variable. */
    portnum = atoi(argv[1]);

    /* ************** */
    /* Create Socket. */
    /* ************** */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    { 
        perror("Error: Socket Creation");
        exit(0);
    }

    bzero(&saddr, sizeof(saddr)); /* Clear sockaddr Structure. */
    saddr.sin_family = AF_INET;   /* Set Family to Internet. */

    /* All Connections on Same Port Number to be Accepted. */
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(portnum); /* Use Specified Port Number. */
    /* Bind a local address to the socket. */
    if (bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr)) != 0) 
    {
	perror("ERROR: Bind Failed");
	exit(0);
    }	
    /* Specify Size of Request Queue. */
    if (listen(socketfd, QLEN) < 0) 
    {
    	perror("Error: Listen Failed");
	exit(0);
    }
    calen = sizeof(caddr);
    printf("\n\n\tWelcome to iGep Server\n");
	
    /* Main Loop - Accept and Handle Requests. */
    while (1) 
    {
	/* Thread is Busy. Move On. */
  	while(avThreads[activeThread].busy == GOOD && activeThread < 200)
	{
 	    /* Next Thread.*/
  	    activeThread++; 
  	}
  	/* Reach End of Available Threads. */	
  	if(activeThread == 200)
        {
	    /* Accept Last Thread. */
            /* gcc Compiler */
  	    /* socketfd2 = accept(socketfd, (struct sockaddr *) &caddr, &calen);*/
	    /* g++ Compiler */
	    /* According to the accept manual page, the third parameter needs to be a 
             * socklen_t pointer; so I cast the int pointer Iam passing in 
             * to a socklen_t pointer */
            socketfd2 = accept(socketfd, (struct sockaddr *)&caddr, (socklen_t*)&calen);

	    if(socketfd2 < 0)
	    {
		exit(0);
            }
            strcpy(buffer,SERVICEUN);
  	    if (write(socketfd2, buffer, strlen((char*)buffer)) <= 0) 
	    {
            	perror("Error: Writing to Socket");
        	exit(0);
	    }
            printf("\nIAM DEAD. TOO Many Threads.");
   	    exit(0);	
  	} 
	else 
	{   /* Accept Request. */
            /* g++ Compiler */
	    avThreads[activeThread].socket = accept(socketfd, (struct sockaddr *)&caddr, (socklen_t*)&calen);
	    if (avThreads[activeThread].socket < 0) 
	    {
            	perror("Error: Accept Failed");
		exit(0);
	    }
	    /* Change Attributes of activeThread. */
	    avThreads[activeThread].num = activeThread;
	    avThreads[activeThread].busy = GOOD;
	    /* Create a New Thread Using pthread_create() Function. */
	    if(pthread_create(&avThreads[activeThread].ptid, NULL, &readmessage, (void *) &avThreads[activeThread]) !=0 ) 
            {
	    	perror("Error: Thread Creation");
		exit(0);
	    }	
	}
    }
}
/* ************************************************************** */
/* **********************Read Fucntion************************** *
*
*Function Name: static void *readmessage(void *buptr);
*Purpose: This Function Reads The Client Request.
*then sends it to parsing, and then to socket write.
*Date Created: 26/11/2013. Sami Giacaman
*
/* ************************************************************** */
static void *readmessage(void *buptr) 
{
    char buffer[MAXPATH],command[50],audio[20],notneeded[20];
    int req,audioCode, j=0, i=0, k=0;
    T_vars *ins;
    ins = (T_vars *) buptr;
    string returnMsg;
    ParsedMessage parsedMsg;
    StatusMessage statusMsg;

    /* Get Client Request, Put Into Buffer. */
    if ((req = read((int)ins->socket, buffer, MAXPATH - 1)) <= 0) 
    {
    	perror("Error: Reading from Socket");
        exit(0);
    }
    printf("\n-----------------------------------------------");
    printf("\nMessage Recieved:");
    printf("\n-----------------------------------------------");
    printf("\n%s",buffer);
    printf("\n-----------------------------------------------\n\n");
    /* Parsing */
    parsedMsg = parseRequestMsg(buffer);
    statusMsg = parsedMsg.getStatusObject();
    if(statusMsg.getCode() != 0)
    {
	returnMsg = createReturnMessage(statusMsg);
	/*** Start: Added response functionality to error check. (Mark - 27th Nov 2013) ***/
	char *retbuffer=new char[returnMsg.size()+1];
    retbuffer[returnMsg.size()]=0;
    memcpy(retbuffer,returnMsg.c_str(),returnMsg.size());
    /* Writing */
    writemessage(buptr, retbuffer); 
	return 0;
	/*** End ***/
    }
    statusMsg = checkMessages(parsedMsg);
    returnMsg = createReturnMessage(statusMsg);
    char *retbuffer=new char[returnMsg.size()+1];
    retbuffer[returnMsg.size()]=0;
    memcpy(retbuffer,returnMsg.c_str(),returnMsg.size());
    /* Writing */
    writemessage(buptr, retbuffer);  
}
/* ************************************************************** */
/* **********************Write Fucntion************************** *
*
*Function Name: static void *writemessage(void *buptr ,char *retbuffer);
*Purpose: This Function Writes to Sockets.
*Date Created: 26/11/2013. Sami Giacaman
*
/* ************************************************************** */
static void *writemessage(void *buptr ,char *retbuffer)
{
    T_vars *ins;
    ins = (T_vars *) buptr;

    if (write((int)ins->socket, retbuffer, strlen((char*)retbuffer)) <= 0) 
    {
        perror("Error: Writing to Socket");
        exit(0);
    }

    /* Close Socket. */
    shutdown((int)ins->socket, 1);
    close((int)ins->socket);
    ins->busy = BAD;
}
/* ************************************************************** */
/* ************************************************************** */
