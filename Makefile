#main app
#Pong:Pong.o  
#	g++ Pong.o -o Pong 


#Pong.o:Pong.cpp
#	g++ -c Pong.cpp

clean:
#cleanup all object file
	  -rm *.o $(objects) 