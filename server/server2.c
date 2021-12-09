#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"rdwrn.h"
#include<string.h> //strlen
#include<stdlib.h>
#include<unistd.h> 
#include<pthread.h> //threading

int main(void)
{
 //a variable to hold the socket descriptor
 int socket_desc= 0, new_socket = 0;  

 struct sockaddr_in server, client;
 socklen_t socksize = sizeof(struct sockaddr_in); 
 //create a socket
 socket_desc = socket(AF_INET, SOCK_STREAM, 0);
 
 if(socket_desc == -1){
  printf("Could not create a socket");
 }

 memset(&server, '0', sizeof(server));

 //prepare the sockaddr_in structure 
 
 server.sin_family = AF_INET; 
 server.sin_addr.s_addr=htonl(INADDR_ANY);
 server.sin_port= htons(50031);

 //Bind 
 if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server))< 0){
  
   puts("bind failed");
   return 1; 
 }

 puts("bind done");

 //listen 
 
 listen(socket_desc, 10);

 //Accept and incoming connection 
 
 puts("Waiting for incoming connections..");
 while(1)
 {
  void *client_handler(void *);
  printf("Waiting for a client to connect...\n");
  
  new_socket=accept(socket_desc, (struct sockaddr *)&client, &socksize); 

  if(new_socket< 0)
  {
    perror("accept failed"); 
    return 1;
  }else{
   printf("connection accepted..\n"); 
  }

  pthread_t sniffer_thread;  

   if(pthread_create(&sniffer_thread, NULL, client_handler, (void*) &new_socket) <0)
   {
     perror("Could not create thread"); 
     exit(EXIT_FAILURE); 
   }

    //Join the thread, so that we don't terminate before the thread
    //pthread_join(sniffer_thread, NULL); 
    puts("Handler assigned\n");
 }

 if(new_socket<0)
   {
     perror("Accept failed");
     exit(EXIT_FAILURE);
   }else{
     printf("Connection accepted...\n");
   }


 exit(EXIT_SUCCESS);
  

}

/*
 * This will handle the connection for each client 
 * */

void *client_handler(void *socket_desc){
  //get the desciptor
  int sock = *(int *)socket_desc;
  void handleStudentId(int);
  void handleRandomNumber(int);
  void handleSystemInfo(int);
  void handleReadFromFile(int);
  while(1){
    int user_option;
    size_t payload_length;
    int read_size;
    
    readn(sock, (unsigned char *) &payload_length, sizeof(size_t)); 
    read_size=readn(sock, (unsigned char *) &user_option, sizeof(int));
    
    if(read_size == 0){
     puts("client disconected\n");
     fflush(stdout); 
    }else if(read_size == -1){
     perror("recieved error\n");
    }

    switch(user_option){
          
	    case 1:
		 handleStudentId(sock); 
	         break; 
	    case 2: 
		 handleRandomNumber(sock); 
		 break; 
	    case 3: 
		 handleSystemInfo(sock); 
		 break; 
	    case 4: 
		 handleReadFromFile(sock); 
	   

    }
   
  }

  //cleanup the socket
  shutdown(sock, SHUT_RDWR);  
  close(sock); 
  return 0;
}

void handleStudentId(int sock){
 FILE *ip;
 char ipaddress[50];  
 char studentId[100] = "S1906581/";

 //get the ip address 
 ip=popen("hostname -I", "r");
 if(ip == NULL){
   perror("The command does not exist");
 }

 //read the data from the stream 
 fgets(ipaddress, 50, ip); 
 //concatenate with the studentId
 strcat(studentId, (char *) ipaddress);

 size_t n = strlen(studentId) + 1; 

 //write to the socket
 writen(sock, (unsigned char *) &n, sizeof(size_t));
 writen(sock, (unsigned char *) studentId, n);

 //close the stream
 pclose(ip);
}

void handleRandomNumber(int sock){

int randomNumbers[5]; 
int i;
size_t n = sizeof(randomNumbers); 

srand(time(0)); 

for(i= 0; i< 5; i++){
 int num = (rand() % (1000 - 0 + 1)) + 0;
 randomNumbers[i] = num;  
}

//write to the socket 
writen(sock, (unsigned char *)&n, sizeof(size_t)); 
writen(sock, (unsigned char *)randomNumbers, n);



}

void handleSystemInfo(int sock){

}

void handleReadFromFile(int sock){
}
