#!/usr/bin/python
import socket
import threading
import SocketServer
import MySQLdb
import pygst
pygst.require("0.10")
import gst
import pygtk
import gtk

from OpenSSL import SSL

mutex = threading.Semaphore()

class GstAudio:
    def __init__(self, file_link, client_addr):
        self.pipeline = gst.Pipeline("serverPipeline")

        self.audiotestsrc = gst.element_factory_make("souphttpsrc", "audio")
        self.audiotestsrc.set_property("location", file_link)
        self.pipeline.add(self.audiotestsrc)

        self.decode = gst.element_factory_make("mad", "decoder")
        self.pipeline.add(self.decode)

        self.converter = gst.element_factory_make("audioconvert", "converter")
        self.pipeline.add(self.converter)

        self.lame = gst.element_factory_make("lame", "lame")
        self.lame.set_property("bitrate", 320)
        self.pipeline.add(self.lame)

        self.sink = gst.element_factory_make("udpsink", "sink")
        self.sink.set_property("host", client_addr)
        self.sink.set_property("port", 7001)
        self.pipeline.add(self.sink)

        gst.element_link_many(self.audiotestsrc, self.decode, self.converter, self.lame,  self.sink)

        bus = self.pipeline.get_bus()
        bus.add_signal_watch()
        bus.enable_sync_message_emission()
        bus.connect("message", self.on_message)
        bus.connect("sync-message::element", self.on_sync_message)

    def stop(self):
        self.pipeline.set_state(gst.STATE_NULL)
        gobject.source_remove( self.timer )

    def start(self):
        self.pipeline.set_state(gst.STATE_PLAYING)

    def on_message(self, bus, message):
        t = message.type
        if t == gst.MESSAGE_EOS:
            self.pipeline.set_state(gst.STATE_NULL)
           
        elif t == gst.MESSAGE_ERROR:
            self.pipeline.set_state(gst.STATE_NULL)
            err, debug = message.parse_error()
            print "Error: %s" % err, debug

    def on_sync_message(self, bus, message):
        if message.structure is None:
            return
        message_name = message.structure.get_name()

#Handler for the client requests.
class ServerTCPReqHandler(SocketServer.BaseRequestHandler):
    #Handler code
    def handle(self):
		#Recieve request
        client = self.request.getpeername()
        
        client_ip = client[0]
        message = self.request.recv(1024)
		#debug
        print message
		#Parse request
        response = message_parser(message, client_ip)
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
        conn = MySQLdb.connect (host = "188.65.117.68",
        user = "edwardbc_server",
        passwd = "lHAGuwsGd72x",
        db = "edwardbc_esd_db")
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
        conn.close()
        return output

#Handle request from client.
def message_parser(message, client_ip):
    
    client = client_ip
    client = client.replace("[", "")
    client = client.replace("]", "")
    client = client.replace("'", "")
    client = client.replace(",", "")
   # client = client.split(' ')
    print client
    #Define output variable
    response = ""
    if message.find("POST") >= 0:
        lines = message.split('\n')
        response = "{}".format(decode_request(message, client))
    elif message.find("GET") >= 0:
        lines = message.split('\n')
        response = "{}".format(decode_request(message, client))
    else:
        response = "404 ERROR"
    return response
    
def decode_request(message, ip):
    lines = message.split('\n')
    if lines[1] != "\r":
        return "HTTP/1.1 400 BAD REQUEST\r\n\r\n Invalid Request"
    
    if lines[0].find("POST") >= 0:
        mac = lines[2].replace("Host: ", "")    
        mac = mac.replace("\r\n", "")
        mac = mac.replace(":", "-")
        print mac
        auth_code = lines[3]
        auth_code = auth_code.replace("\r\n", "")
        auth_code = auth_code.replace("#", "")
        print auth_code
        sql_response = sql_query("SELECT authCode FROM IGEP WHERE authCode=\""+auth_code+"\"")
      
        #sql_response = sql_response.replace(",","")
        #sql_response = sql_response.replace("\n","")

        print sql_response
        if sql_response.find(auth_code) != 0:
            return "HTTP/1.1 403 FORBIDDEN \r\n\r\nCode is unavailable"
        elif sql_response.find(auth_code)  >= 0:
            sql_response = sql_query("UPDATE IGEP SET ip=\""+ip+"\"  WHERE authCode=\""+auth_code+"\"")
            return "HTTP/1.1 200 OK \r\n\r\nCode valid"
    elif lines[0].find("GET") >= 0:
        mac = lines[2].replace("Host: ", "")    
        mac = mac.replace("\r\n", "")
        mac = mac.replace(":", "-")
        print mac
        audio_code = lines[3]
        audio_code = audio_code.replace("\r\n", "")
        audio_code = audio_code.replace("#", "")
        print audio_code
        mac_return = sql_query("SELECT mac FROM IGEP WHERE ip=\""+ip+"\"")
        mac = mac.replace(",", "")
        mac = mac.replace("\n", "")
        mac = mac.replace(" ", "")
        print mac_return
        query = "SELECT customerID FROM Bookings WHERE mac='%s' AND timeOUT IS NULL" % (mac)
        customerID = sql_query(query)
        customerID = customerID.replace(",","")
        customerID = customerID.replace("\n","")
        customerID = customerID.replace(" ","")

        sql_response = sql_query("SELECT expertise FROM Customers WHERE customerID="+str(customerID))
        expertise = sql_response[0] 
        print expertise
        #Get Language and format it.
        sql_response = sql_query("SELECT language FROM Customers WHERE customerID="+str(customerID))
        sql_response = sql_response.replace(",","")
        sql_response = sql_response.replace("\n","")
        sql_response = sql_response.replace(" ", "")
        language = sql_response
        print language
        query = "SELECT filePath FROM Audio WHERE audioCode=%d AND expertise=%d AND language='%s'" % (int(audio_code), int(expertise), language)
        #print query
        sql_response = sql_query(query)
        sql_response = sql_response.replace(",","")
        sql_response = sql_response.replace("\n","")
        sql_response = sql_response.replace(" ", "")
        print sql_response
        audio_file = sql_response

        #Decide if the link is valid, if not return error
        if sql_response.find("http") >= 0:
            print "Accepted\n"
            query = "INSERT INTO Log (customerID, audioCode, dateTime) VALUES ( '%s', '%s', (select now())" % (str(customerID), str(audio_code))
            sql_response = sql_query(query)
            print sql_response

            #Start the gstreamer server.
            player = GstAudio(audio_file, ip)
            player.start()
            #gtk.main()

            return "HTTP/1.1 200 OK \r\n\r\nCode valid"
        else:
            return "HTTP/1.1 403 FORBIDDEN \r\n\r\nCode is unavailable"
        
if __name__ == "__main__":
    #Host and port
    HOST, PORT = "0.0.0.0", 7000
    #Takes an optional argument for enabling SSL, .pem should be used.
    #To enable plain text communication the first argument should be null.
    server = SslTCPServer(None,(HOST, PORT), ServerTCPReqHandler)
    ip, port = server.server_address
    print str(ip)+":"+str(port)
	#Server start up and threading.
    server_thread = threading.Thread(target=server.serve_forever)
    
    server_thread.daemon = True
    server_thread.start()

    server.serve_forever()
    server.socket.getpeername()

