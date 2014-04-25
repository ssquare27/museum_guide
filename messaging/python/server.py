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
        message = self.request.recv(1024)
        print message
        response = message_parser(message)
        #response = "{}".format(message)
        print response
        self.request.sendall(response)
        
        
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
 
    def shutdown_request(self,request):
        request.shutdown()

def sql_query(query):
        conn = MySQLdb.connect (host = "localhost",
        user = "<Database User>",
        passwd = "<user Password>",
        db = "<database>")
    
        cursor = conn.cursor()
        output = ""
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

def message_parser(message):
    #Define output variable
    response = ""
    if message == "QUERYALL":
        response = "{}".format(sql_query("SELECT * FROM tag"))
    elif message.find("GET TAGS NAME") >= 0:
        response = "{}".format(sql_query("SELECT name FROM tag"))
    elif message.find("UPDATE LOCATION") >= 0:
        message = message.replace("UPDATE LOCATION ", "")
        opts = message.split(' ')
        update_query = "UPDATE tag SET location=\""+opts[1]+"\" WHERE address=\""+opts[0] + "\""
        response = "{}".format(sql_query(update_query))
    else:
        response = "404 ERROR"
    return response
    
        

if __name__ == "__main__":
    
    HOST, PORT = "0.0.0.0", 25999

    server = SslTCPServer("/server.pem",(HOST, PORT), ServerTCPReqHandler)
    ip, port = server.server_address

    server_thread = threading.Thread(target=server.serve_forever)
    
    server_thread.daemon = True
    server_thread.start()

    server.serve_forever()

