
#################################  Linux  ###########################################################################
#Listing network adapters 
unix_list: unix_list.c  
	gcc unix_list.c -o unix_list


clean:
#cleanup all object file
	  -rm *.o $(objects) 


#################################  Windows  ###########################################################################
#Listing network adapters 
win_init:win_init.c
	gcc win_init.c -o win_init.exe -lws2_32

win_list:win_list.c
	gcc win_list.c -o win_list.exe -liphlpapi -lws2_32 

################################  cross-platform   ##################################################################
sock_init:sock_init.c
# for both windows and linux  user if you use mingwing comp and windows please comment this and uncomment the 2nd line
	gcc sock_init.c -o sock_init 
#	gcc sock_init.c -o sock_init-lws2_32 for mingwing  user , windows system