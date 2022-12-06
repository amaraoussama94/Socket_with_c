
#################################  Linux  ###########################################################################
#main app
#Pong:Pong.o  
#	g++ Pong.o -o Pong 


#Pong.o:Pong.cpp
#	g++ -c Pong.cpp

clean:
#cleanup all object file
	  -rm *.o $(objects) 


#################################  Windows  ###########################################################################

win_init:win_init.c
	gcc win_init.c -o win_init.exe -lws2_32