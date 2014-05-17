#!/usr/bin/python
import socket
import threading
import SocketServer
import MySQLdb

from OpenSSL import SSL

mutex = threading.Semaphore()

#Handler for the client requests.
class ServerTCPReqHandler(SocketServer.BaseRequestHandler):
    #Handler code
    def handle(self):
		#Recieve request
        message = self.request.recv(1024)
		#debug
        print message
		#Parse request
        response = message_parser(message)
        #response = "{}".format(message)
        print response
		#send request.
        self.request.sendall(response)
        
#Doing some clever stuff mainly for SSL, best not to look at this as it 
#is often not the reason for failure.        
class SslTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer): 
    #Override class initialisation to include ssl certificate.
    def __init__(self, sslfile, server_address, RequestHandlerClass,
                    bind_and_activate=True):
        
        self.daemon_threads = True 
        self.client_lock = threading.Lock()
 
        # if cert-file is present then use ssl
        if sslfile is None:
            self.use_ssl = False
            SocketServer.TCPServer.__init__(self, server_address,
                                        RequestHandlerClass, bind_and_activate)
        else:
            self.use_ssl = True

            SocketServer.BaseServer.__init__(self, server_address,
                                                RequestHandlerClass)
            ctx = SSL.Context(SSL.SSLv23_METHOD)
            # cert.pem-file containing the server private key and certificate
            ctx.use_privatekey_file(sslfile)
            ctx.use_certificate_file(sslfile)
            self.socket = SSL.Connection(ctx, socket.socket(self.address_family, self.socket_type))
            
            if bind_and_activate:
                self.server_bind()
                self.server_activate()
 
    #def shutdown_request(self,request):
        #request.shutdown()

#Handles the database communication
def sql_query(query):
		#Database login details
        conn = MySQLdb.connect (host = "localhost",
        user = "igep",
        passwd = "letmein",
        db = "test")
		#select cursor to enable database commands.
        cursor = conn.cursor()
        output = ""
		#Select query
        if query.find("SELECT") >= 0:
            cursor.execute(query)
            rows = cursor.fetchall()
            
            for row in rows:
                output += "{} \n".format(row)
            output = output.translate(None, "()\'")
            output = output.replace("datetime.datetime", "")
        elif query.find("UPDATE") >= 0:
            print query
            cursor.execute(query)
            conn.commit()
            output = "OK"
        return output

#Handle request from client.
def message_parser(message):
    #Define output variable
    response = ""
    if message.find("POST") >= 0:
        lines = message.split('\n')
        #response = "{}".decode_request(message)
        response = "{}".format(decode_request(message))
    else:
        response = "404 ERROR"
    return response
    
def decode_request(message):
    lines = message.split('\n')
    if lines[1] != "\r":
        return "HTTP/1.1 400 BAD REQUEST\r\n\r\n Invalid Request"
    
    if lines[2].find("Host:") >= 0:
        mac = lines[2].replace("Host: ", "")    
        mac = mac.replace("\r\n", "")
        
        auth_code = lines[3]
        auth_code = auth_code.replace("\r\n", "")
        auth_code = auth_code.replace("#", "")
        print auth_code
        sql_response = sql_query("SELECT valid FROM IGEP WHERE authCode=\""+auth_code+"\"")
        sql_response = sql_response.replace(",","")
        if sql_response.find("Y") >= 0:
            return "HTTP/1.1 403 FORBIDDEN \r\n\r\n Blah"
        else:
            sql_response = sql_query("UPDATE IGEP SET ip=\""+mac+"\" WHERE authCode=\""+auth_code+"\"")
            return "HTTP/1.1 200 OK \r\n\r\n yay"
        
    
        

if __name__ == "__main__":
    #Host and port
    HOST, PORT = "0.0.0.0", 25999
    #Takes an optional argument for enabling SSL, .pem should be used.
    #To enable plain text communication the first argument should be null.
    server = SslTCPServer(None,(HOST, PORT), ServerTCPReqHandler)
    ip, port = server.server_address
	#Server start up and threading.
    server_thread = threading.Thread(target=server.serve_forever)
    
    server_thread.daemon = True
    server_thread.start()

    server.serve_forever()

