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
list of code  :</br>
unix_List.c for more info : [getnameinfo_man](https://man7.org/linux/man-pages/man3/getnameinfo.3.html )</br>
##  Linux and macOS:
list of code  :</br>
*tcpserverfork.c</br>
## cross-platform:
list of code  :</br>
*sock_init.c for test  run http://127.0.0.1:8080 </br>
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
