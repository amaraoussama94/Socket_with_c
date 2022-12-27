
#################################  Linux  ###########################################################################
#Listing network adapters 
unix_list: unix_list.c  
	gcc unix_list.c -o unix_list 


tcpserverfork:tcpserverfork.c
	gcc tcpserverfork.c -o tcpserverfork

clean:
#cleanup all object file
#	  -rm *.o $(objects) 
	  -rm sock_init tcpclient tcpserver time_server unix_list tcpserverchat  tcpserverfork  udpclient  udp_recvfrom udp_serve_toupper udp_client_toupper webclientget webserver webserver2  smtp_send  openssl_version   https_simple   https_get tls_client  tls_get_cert   tls_time_server  https_server   libssh_test   ssh_connect   ssh_auth  ssh_command   ssh_download   error_text  connect_blocking                                                                         


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

openssl_version:openssl_version.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc openssl_version.c   -o openssl_version  -lcrypto
#	gcc openssl_version.c  -o openssl_version.exe   -lcrypto

https_simple:https_simple.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc https_simple.c   -o https_simple  -lssl -lcrypto
#	gcc https_simple.c  -o https_simple.exe   -lssl -lcrypto -lws2_32

https_get:https_get.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc https_get.c   -o https_get  -lssl -lcrypto
#	gcc https_get.c  -o https_get.exe   -lssl -lcrypto -lws2_32

tls_client:tls_client.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc tls_client.c   -o tls_client  -lssl -lcrypto
#	gcc tls_client.c  -o tls_client.exe   -lssl -lcrypto -lws2_32

tls_get_cert:tls_get_cert.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc tls_get_cert.c   -o tls_get_cert  -lssl -lcrypto
#	gcc tls_get_cert.c  -o tls_get_cert.exe   -lssl -lcrypto -lws2_32

tls_time_server:tls_time_server.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc tls_time_server.c   -o tls_time_server  -lssl -lcrypto
#	gcc tls_time_server.c  -o tls_time_server.exe   -lssl -lcrypto -lws2_32


https_server:https_server.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc https_server.c   -o https_server  -lssl -lcrypto
#	gcc https_server.c  -o https_server.exe   -lssl -lcrypto -lws2_32

libssh_test:libssh_test.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc libssh_test.c   -o libssh_test  -lssh
#	gcc libssh_test.c  -o libssh_test.exe   -lssh

ssh_connect:ssh_connect.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc ssh_connect.c   -o ssh_connect  -lssh
#	gcc ssh_connect.c  -o ssh_connect.exe   -lssh


ssh_auth:ssh_auth.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc ssh_auth.c   -o ssh_auth  -lssh
#	gcc ssh_auth.c  -o ssh_auth.exe   -lssh     

ssh_command:ssh_command.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc ssh_command.c   -o ssh_command  -lssh
#	gcc ssh_command.c  -o ssh_command.exe   -lssh    

ssh_download:ssh_download.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc ssh_download.c   -o ssh_download  -lssh
#	gcc ssh_download.c  -o ssh_download.exe   -lssh    

error_text:error_text.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc error_text.c   -o error_text   
#	gcc error_text.c  -o error_text.exe    

connect_blocking:connect_blocking.c  
# for both windows and linux  user if you use MinGW comp and windows please comment this and uncomment the 2nd line
	gcc connect_blocking.c   -o connect_blocking   
#	gcc connect_blocking.c  -o connect_blocking.exe   