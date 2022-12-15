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

<p align="center"> <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/HTTpserver_client_GET_request.png" width="350" height="600" >  </p>

for lunix user ,how to use : ./ webclientget http://example.com/  (exemple)</br> 
windows user via powershell :  webclientget.exe http://example.com/    (exemple)</br> 

## webserver.c
-HTTP web server that can serve static files from a local directory .Our server only supports GET typerequests .Our server only supports hosting one site. Our server expects all hosted files to be in a subdirectory called public . </br> 

## Please attention  :
for windows user please read the make file </br>
