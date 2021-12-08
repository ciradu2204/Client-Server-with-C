#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"rdwrn.h"
#include<string.h> //strlen
#include<stdlib.h>
#include<unistd.h> 
#include<pthread.h> //threading


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
 while(1)
 {
  void *client_handler(void *);
  printf("Waiting for a client to connect...\n");
 
  new_socket=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

   if(new_socket<0)
   {
     perror("Accept failed");
     exit(EXIT_FAILURE);
   }else{
     printf("Connection accepted...\n");
   }

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

 exit(EXIT_SUCCESS);
  

}

/*
 * This will handle the connection for each client 
 * */

void *client_handler(void *socket_desc){
  //get the desciptor
  int sock = *(int *)socket_desc;
  while(1){
    char  clientMessage[30];
    size_t payload_length;
    int read_size;

    readn(sock, (unsigned char *) &payload_length, sizeof(size_t)); 
    read_size=readn(sock, (unsigned char *) clientMessage, payload_length);
    
    if(read_size == 0){
     puts("client disconected\n");
     fflush(stdout); 
    }else if(read_size == -1){
     perror("recieved error\n");
    }
   
    writen(sock, (unsigned char *) &payload_length, sizeof(size_t)); 
    writen(sock, (unsigned char *) clientMessage, payload_length); 
  }

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
