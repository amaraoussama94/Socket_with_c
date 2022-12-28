# Socket programing  with c
This  is  a collection of  server  client code. Here we use  socket with some best  practise. If you want to use the  code please install make first read the make file and then enjoy .</br>
 Here some tips to understand our code:</br>
win_ : file for windows system</br>
unix_: file for Linux system</br>
Else it is a cross platform code</br>

## Listing network adapters on Windows :
list of code  :</br>
win_init.c :Winsock initialization</br>
win_List.c :list network adapters on Windows  </br>

## Listing network adapters on Linux and macOS:
 To run this code, first install make. Then in shell got to repertory where the code exist(cd ...... Socket_with_c) </br>run:" make unix_list"  </br>
Then  ". /unix_list"</br>
list of code  :</br>
unix_List.c : list network adapters on Linux.  for more info : [getnameinfo_man](https://man7.org/linux/man-pages/man3/getnameinfo.3.html )</br>

##  Linux and macOS:
To run this code, first install make. Then in shell got to repertory where the code exist(cd ...... Socket_with_c) </br>run:" make tcpserverfork"  </br>
Then  ". /tcpserverfork"</br>
list of code  :</br>
### tcpserverfork.c:
Using IPv4. Using  fork we  create a child process to handle multiple client with no delay. This code is  microservers that make any string just all upper case simple and make you learn a lot. </br>
## cross-platform:
PS : to run all this code please read the make file !</br>
list of code  :</br>
### sock_init.c :
Silmple code  to inti socket work for both windows and unix like system </br>
### time_server.c: 
This is a simple time  server that sends the time date to simple HTTPS  web page .Here you will learn how to make  a cross platform code and how to make it accept IP6 and IP4 at the same time.</br> 
For a test  run http://127.0.0.1:8080, simply 127.0.0.1 is the local host address and 8080 is the port</br> 
### tcpclient.c :
Using IPv4. Follow instructions in the code first line if you want to test it. Using some mystery technique this code can create some chile that can send a simple string to the server and wait for his response   </br>
<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/TCP%20client.png" width="350" height="600"  >  </p>

### tcpserver.c:
Simple server that just translates a string sending by client to  all uppercase  string. This serves serve any number of  client  simultaneously with no blocking or  delay ,each client will get only his string that he send to server all uper .</br>Port fixed to 8080 , accept both ip4 and ip6. </br>
<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/TCp%20Server.png" width="350" height="600" >  </p>

### tcpserverchat.c:
Simple server that just translates a string sending by client to  all uppercase  string. This serves serve any number of  client  simultaneously with no blocking or  delay ,each client will all string that he send to server all uper and the other string send by the other client  .</br>Port fixed to 8080 , accept both ip4 and ip6</br>

### udp_recvfrom.c :
Simple UDP server.It will accept any incoming msg  from UDP client.</br>
<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/udpserver.png" width="350" height="600" >  </p>

### udpclient.c :
The simple UPD client handles 2 cm argument  @IP and port number. Then you start sending msg .PS there's no test if the msg reaches the server or no in this code .</br>
<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/Udpclient.png" width="350" height="600" >  </p>

### udp_serve_toupper.c :
Its  same like TCP. We just  use  UDP instead of TCP. We  also use  fork style  to handle multiclient request </br>
<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/udpserverupper.png" width="350" height="600" >  </p>

###  udp_serve_toupper_simple.c :
Same like the other  server using simple methods  . </br>

### udp_client_toupper.c :
UDP client, accept 2 argument from cmd @IP and Port number  of the server to connect  with, then we  make any  child  process to make  several  clients  and send some  msg to the server we then get them from serve and print  them </br>

### lookup.c :
This code takes a name or IP address for its only argument.It then uses getaddrinfo() to resolve that name or that IP address into an address structure, and the program prints that IP address** using getnameinfo() for the text conversion. If multiple addresses are associated with a name, it prints each of them. </br>  

for linux  user  :./lookup   domain (example.com  ,google.com .... as example )</br>  
windows user via powershell  : lookup.exe   domain (example.com  ,google.com .... as example )</br> 

### dns_query.c :
We will now implement a utility to send DNS queries to a DNS server and receive the DNS response. This should not normally be needed in the field. It is, however, a good opportunity to better understand the DNS protocol and to get experience of sending binary UDP packets.</br>  


for lunix user ,how to use :./dns_query example.com a   (exemple)</br> 
windows user via powershell : dns_query.exe example.com a   (exemple)</br> 

### Parsing_URL.c , parsing_url.h :
This is simple lib with one function . The function takes as input a URL, and it returns as output the hostname, the port number, and the document path .</br> 

### webclientget.c :
client takes as input a URL. It thenattempts to connect to the host and retrieve the resource given by the URL. The program displays the HTTP headers that are sent and received, and it attempts to parse out the requested resource content from the HTTP response.</br> 

<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/HTTpserver_client_GET_request.png" width="600" height="400" >  </p>

for lunix user ,how to use : ./ webclientget http://example.com/  (exemple)</br> 
windows user via powershell :  webclientget.exe http://example.com/    (exemple)</br> 

## webserver.c
-HTTP web server that can serve static files from a local directory .Our server only supports GET typerequests .Our server only supports hosting one site. Our server expects all hosted files to be in a subdirectory called public . </br> 

how to use :  after make it (mean compile it using make file) . simply run the server ./webserver (webserver.exe for windows user  ) then go to your web browser and tape http://127.0.0.1:8080/
then yo got it yey . </br> 
PS: You can also drop different files into the public folder and play around with creating more complicated websites.</br> 

## webserver2.c
-It behaves exactly like the code we developed, but it avoids having global state(at the expense of a little added verbosity). This may make web_server2.c more suitable for integration into more significant projects and continued development. </br> 
PS: make 2webserver  </br> 

## smtp_send.c :
The program we develop  is used to deliver mail directly to the recipient's email provider. It is not useful to deliver mail to a relay server because we are not going to implement authentication techniques. Generally, SMTP servers do not relay mail without authenticating that the sender has an account with them.</br> 
how to use :  after make it (mean compile it using make file) . simply run  ./smtp_send (smtp_send.exe for windows user  )</br>  
then,Connecting to host: gmail-smtp-in.k.google.com </br> 
then fill all other needed info .</br> 
to close mail that you want send just send . </br> 
 
PS: The SMTP protocol we describe in this code  is unsecured and not encrypted. This is convenient for explanation and learning purposes, but in the real world, you may want to secure your email transfer.</br> 

##  openssl_version.c :
test OpenSSL library installed or no</br> 

## https_simple.c :
Initiation to sll  . simple code </br> 
how to use : </br> 
Linux user :./https_simple example.org 443</br> 
Windows user :https_simple example.org 443</br> 

## tls_client.c: 
This is tcpclient.c , An In-Depth Overview of TCP Connections, but it has been modified to make TLS connections.
Linux user :./tls_client example.org 443</br> 
Windows user :tls_client example.org 443</br> 

## https_get.c: 
This is the webclientget.c program from Chapter 6, Building a Simple Web Client, but it has been modified for HTTPS. You can think of it as the extended version of https_simple.c.

for lunix user ,how to use : ./ https_get https://example.com/  (exemple)</br> 
windows user via powershell :  https_get.exe https://example.com/    (exemple)</br> 

## tls_get_cert.c:
This is like https_simple.c, but it simply prints the connected server's certificate and exits.

Linux user :./tls_get_cert example.org 443</br> 
Windows user :tls_get_cert example.org 443</br> 

## Self-signed certificates with OpenSSL : 
OpenSSL asks questions about what to put on the certificate, including the subject, your name, company, location, and so on. You can use the defaults on all of these as this doesn't matter for our testing purposes.</br> 

The basic command to self-sign a certificate is as follows:</br> 
openssl req -x509 -newkey rsa:2048 -nodes -sha256 -keyout key.pem \-out cert.pem -days 365 </br> 

You can also use OpenSSL to view a certificate. The following command does this:</br> 
openssl x509 -text -noout -in cert.pem</br> 

Windows using MSYS, you may get garbled line endings from the previous command. If so, try using unix2dos to fix it, as shown by the following command:</br> 
openssl x509 -text -noout -in cert.pem | unix2dos </br> 

##  tls_time_server.c :
simple time server that displays the time to an HTTPS client. This program is an adaptation of time_server.c .Our server uses a while loop to accept multiple connections.</br> 
How to use :</br> 
Generate your certification use section "Self-signed certificates with OpenSSL" </br> 
Fisrt run the server .</br> 
You can connect to the time server by navigating your web browser to  https://127.0.0.1:8080 </br> 
To access the time server, you need to add an exception in the browser. The method for this is different in each browser, but generally, there is an Advanced button that leads to an option to either add a certificate exception or otherwise proceed with the insecure connection.</br> 

## https_server.c :
This program is a modification of web_server.c , Building a Simple Web Server. It can be usedto serve a simple static website over HTTPS. In the https_server.c program, the basic TLS/SSL connection is set up and established the same way as shown in tls_time_server.c. Once the secure connection is established, the connection is simply treated as HTTP</br> 
How to use :</br> 
Generate your certification use section "Self-signed certificates with OpenSSL" </br> 
Fisrt run the server .</br> 
You can connect to the time server by navigating your web browser to  https://127.0.0.1:8080 </br> 
To access the time server, you need to add an exception in the browser. The method for this is different in each browser, but generally, there is an Advanced button that leads to an option to either add a certificate exception or otherwise proceed with the insecure connection.</br> 

## libssh_test.c:

Simple code to see the libssh version </br>

## ssh_connect.c :
Simple code tha allow you to connect  to ssh server . Note please that you need to instal open ssh to try tis project . 
How to use :How to use :</br> 
-./ss_connect 127.0.0.1(@IP of the server ) 22 (Port number if  you change it or it s optional )</br> 
-./ss_connect.exe 127.0.0.1(@IP of the server ) 22 (Port number if  you change it or it s optional )</br> 

## ssh_auth.c :
Simple code tha allow you to connect  to ssh server and authentify  . Note please that you need to instal open ssh to try tis project . 
how to use :</br> 
-./ssh_auth 127.0.0.1(@IP of the server ) 22 (Port number   ) alice</br> 
-ssh_auth.exe 127.0.0.1(@IP of the server ) 22 (Port number   ) alice</br> 
ssh_auth was used to successfully authenticate with the  locally running SSH server. The ssh_auth program used password authentication with the username alice and the password password123. Needless to say, you need to change the username and password as appropriate for your SSH server</br> 

## ssh_command.c :
Simple code tha allow you to connect  to ssh server ,authentify  and send command to remote machine . Note please that you need to instal open ssh to try tis project . 
how to use :</br> 
-./ssh_command 127.0.0.1(@IP of the server ) 22 (Port number   ) alice</br> 
-ssh_command.exe 127.0.0.1(@IP of the server ) 22 (Port number   )alice</br> 
ssh_auth was used to successfully authenticate with the  locally running SSH server. The ssh_auth program used password authentication with the username alice and the password password123. Needless to say, you need to change the username and password as appropriate for your SSH server</br> 

## ssh_download.c :
Simple code tha allow you to connect  to ssh server ,authentify  and send file to remote machine . Note please that you need to instal open ssh to try tis project . 
how to use :</br> 
-./ssh_download127.0.0.1(@IP of the server ) 22 (Port number   ) alice</br> 
-ssh_download.exe 127.0.0.1(@IP of the server ) 22 (Port number   ) alice</br> 
 ssh_auth was used to successfully authenticate with the  locally running SSH server. The ssh_auth program used password authentication with the username alice and the password password123. Needless to say, you need to change the username and password as appropriate for your SSH server</br> 

## error_text.c :
 display a text-based error message in addition tothe error code .</br>

##  connect_blocking.c  connect_timeout.c :
connect_timeout.c in is a working example of the connect() timeout method using select(). An additional example, connect_blocking.c, is also included for comparison .</br>
how to use :</br> 
-./ssh_download   127.0.0.1(@IP of the server ) 22 (Port number   ) </br> 
-ssh_download.exe 127.0.0.1(@IP of the server ) 22 (Port number   )  </br> 

<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/TCP%20three-way%20handshake.png" width="350" height="400" >  </p>

## big_send.c server_ignore.c :
server_ignore.c, implements a simple TCP server that accepts connections and then ignores them. The second file, big_send.c, initiates a TCP connection and then attempts to send lots of data. By using the big_send program to connect to the server_ignore program, you can investigate the blocking behavior of send() for yourself.</br>
how to use :</br> 
- ./server_ignore </br> ./big_send @IP 8080 </br>
- server_ignore.exe </br>  big_send.exe @IP 8080 </br>
## Please attention  :
for windows user please read the make file </br>
