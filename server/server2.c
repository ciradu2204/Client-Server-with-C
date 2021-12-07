#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"rdwrn.h"
#include<string.h> //strlen
#include<stdlib.h>
#include<unistd.h> 
#include<pthread.h> //threading

void *client_handler(void *); 
void handleStudentId(int); 
void handleRandomNumber(int); 
void handleSystemInfo(int); 
void handleReadFromFile(int); 

int main(void)
{
 //a variable to hold the socket descriptor
 int socket_desc, new_socket, c;   

 struct sockaddr_in server, client;
 //create a socket
 socket_desc = socket(AF_INET, SOCK_STREAM, 0);

 if(socket_desc == -1){
  printf("Could not create a socket");
 }


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
 c=sizeof(struct sockaddr_in);
 while((new_socket=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) >= 0)
 {

 printf("Waiting for a client to connect...\n");
 
 printf("Connection accepted....\n");

 pthread_t sniffer_thread;  

 if(pthread_create(&sniffer_thread, NULL, client_handler, (void*) &new_socket) <0)
 {
   perror("Could not create thread"); 
   exit(EXIT_FAILURE); 
 }


 //Join the thread, so that we don't terminate before the thread
 pthread_join(sniffer_thread, NULL); 
 puts("Handler assigned\n");
 }

 if(new_socket<0)
 {
  perror("Accept failed");
  return 1;

 }

 exit(EXIT_SUCCESS);
  

}

/*
 * This will handle the connection for each client 
 * */

void *client_handler(void *socket_desc){
 
  //Get the socket descriptor 
  int sock = *(int *)socket_desc;	
  char  client_option[2000];
  int read_size;

  //read the option from client 
  while((read_size=readn(sock, client_option, 2000) )> 0){
     write(sock, client_option, strlen(client_option));
     //switch(atoi(client_option)){
	  //case 1: 
	      //handleStudentId(sock);
	      //break;
	  //case 2: 
	      //handleRandomNumber(sock);
	     // break;
	  //case 3: 
	      //handleSystemInfo(sock);
	     // break;
	  //case 4: 
	      //handleReadFromFile(sock);
      //}

 } 


 if(read_size == 0)
  {
    puts("client disconected");
  }else if (read_size == -1)
  {
     perror("receive failed");
  }
  
  //Free the socket pointer
  free(socket_desc);
  
  //cleanup the socket
  shutdown(sock, SHUT_RDWR);  
  close(sock); 
  return 0;
}

void handleStudentId(int sock){
 
 char studentId_message[100] = "S1906581";
 printf("studentId_message : %s", studentId_message);
 //strcat(studentId_message, (char *) &server.sin_addr.s_addr);

 size_t n = strlen(studentId_message) + 1; 

 writen(sock, (unsigned char *) &n, sizeof(n)); 
 writen(sock, (unsigned char *) studentId_message, n);

}

void handleRandomNumber(int sock){

}

void handleSystemInfo(int sock){

}

void handleReadFromFile(int sock){
}
