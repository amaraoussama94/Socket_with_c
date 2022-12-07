
#################################  Linux  ###########################################################################
#Listing network adapters 
unix_list: unix_list.c  
	gcc unix_list.c -o unix_list


#Pong.o:Pong.cpp
#	g++ -c Pong.cpp

clean:
#cleanup all object file
	  -rm *.o $(objects) 


#################################  Windows  ###########################################################################
#Listing network adapters 
win_init:win_init.c
	gcc win_init.c -o win_init.exe -lws2_32

win_list:win_list.c
	gcc win_list.c -o win_list.exe -liphlpapi -lws2_32 