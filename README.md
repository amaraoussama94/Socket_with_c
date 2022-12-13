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

### udp_serve_toupper.c :

###  udp_serve_toupper_simple.c :

### udpclient.c :

## Please attention  :
for windows user please read the make file </br>
