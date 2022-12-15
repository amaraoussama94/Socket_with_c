
#################################  Linux  ###########################################################################
#Listing network adapters 
unix_list: unix_list.c  
	gcc unix_list.c -o unix_list 


tcpserverfork:tcpserverfork.c
	gcc tcpserverfork.c -o tcpserverfork

clean:
#cleanup all object file
#	  -rm *.o $(objects) 
	  -rm sock_init tcpclient tcpserver time_server unix_list tcpserverchat  tcpserverfork  udpclient  udp_recvfrom udp_serve_toupper udp_client_toupper webclientget webserver webserver2  smtp_send                                                                                                         


#################################  Windows  ###########################################################################
#Listing network adapters 
win_init:win_init.c
	gcc win_init.c -o win_init.exe -lws2_32

win_list:win_list.c
	gcc win_list.c -o win_list.exe -liphlpapi -lws2_32 

################################  cross-platform   ##################################################################
sock_init:sock_init.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc sock_init.c -o sock_init 
#	gcc sock_init.c -o sock_init-lws2_32  

time_server:time_server.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc time_server.c -o time_server
#	gcc time_server.c -o time_server.exe -lws2_32


tcpclient:tcpclient.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc tcpclient.c -o tcpclient
#	gcc tcpclient.c -o tcpclient.exe -lws2_32

tcpserver:tcpserver.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc tcpserver.c -o tcpserver
#	gcc tcpserver.c -o tcpserver.exe -lws2_32

tcpserverchat:tcpserverchat.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc tcpserverchat.c -o tcpserverchat
#	gcc tcpserverchat.c -o tcpserverchat.exe -lws2_32

udpclient :udpclient.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc udpclient.c -o udpclient 
#	gcc udpclient.c -o udpclient .exe -lws2_32

udp_recvfrom:udp_recvfrom.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc udp_recvfrom.c -o udp_recvfrom
#	gcc udp_recvfrom.c -o udp_recvfrom.exe -lws2_32

udp_serve_toupper:udp_serve_toupper.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc udp_serve_toupper.c -o udp_serve_toupper
#	gcc udp_serve_toupper.c -o udp_serve_toupper.exe -lws2_32

udp_client_toupper:udp_client_toupper.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc udp_client_toupper.c -o udp_client_toupper
#	gcc udp_client_toupper.c -o udp_client_toupper.exe -lws2_32

udp_serve_toupper_simple:udp_serve_toupper_simple.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc udp_serve_toupper_simple.c -o udp_serve_toupper_simple
#	gcc udp_serve_toupper_simple.c -o udp_serve_toupper_simple.exe -lws2_32

lookup:lookup.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc lookup.c -o lookupwebclientget
#	gcc lookup.c -o lookup.exe -lws2_32

dns_query:dns_query.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc dns_query.c -o dns_query
#	gcc dns_query.c -o dns_query.exe -lws2_32
 

webclientget:Parsing_URL.c webclientget.c
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc Parsing_URL.c webclientget.c   -o webclientget
#	gcc Parsing_URL.c webclientgets.c -o webclientget.exe -lws2_32

webserver:webserver.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc webserver.c   -o webserver
#	gcc webserver.c  -o webserver.exe -lws2_32

2webserver:webserver2.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc webserver2.c   -o webserver2
#	gcc webserver2.c  -o webserver2.exe -lws2_32

smtp_send:smtp_send.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc smtp_send.c   -o smtp_send
#	gcc smtp_send.c  -o smtp_send.exe -lws2_32

