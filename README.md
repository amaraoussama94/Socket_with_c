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
*tcpserverfork.c: Using  fork we  create a child process to handle multiple client with no delay. This code is  microservers that make any string just all upper case simple and make you learn a lot. </br>
## cross-platform:
PS : to run all this code please read the make file !</br>
list of code  :</br>
*sock_init.c : Silmple code  to inti socket work for both windows and unix like system </br>
*time_server.c:for test  run http://127.0.0.1:8080 </br>
*tcpclient.c </br>
<p align="center">
    <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/TCP%20client.png"  >  
</p>
*tcpserver.c</br>
<p align="center">
    <img src="https://github.com/amaraoussama94/Socket_with_c/blob/main/TCp%20Server.png"  >  
</p>
*tcpserverchat.c</br>
## PS
for windows user please read the make file </br>
