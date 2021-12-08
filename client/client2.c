#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet.addr
#include<string.h> //strlen
#include"rdwrn.h" //readn() and written()
#include<unistd.h>
#include <stdlib.h>

int main(void)
{
  //a variable to take the descriptor returned by the socket
  int socket_desc; 
  struct sockaddr_in server;

  //Create a socket which takes the domain, type, protocol 
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  //incase the socket creation was not successful
  if(socket_desc == -1){

   perror("Error -could not create a socket");
   exit(EXIT_FAILURE);
  }

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(50031);
  int connection;
  connection=connect(socket_desc, (struct sockaddr *)&server, sizeof(server));

   if(connection < 0){
     perror("Error-connect failed");
      exit(1);
    }else{
     printf("Connected to server.....\n");
   }

  while(connection == 0)
  {

   void getStudentId(int); 
   void getRandomNumbers(int); 
   void getUname(int); 
   void getFileNames(int); 
    
   
   int choice; 
   printf("1. Get student ID\n"); 
   printf("2. Get 5 random numbers\n"); 
   printf("3. The server uname\n"); 
   printf("4. Obtain file names in the current working directory\n"); 
   printf("5. Exits the program\n");
   printf("Enter your choice: \n");
   scanf("%d", &choice);
   
   switch(choice)
   {
      case 1: 
	      getStudentId(socket_desc);
	      break;
      case 2: 
	      getRandomNumbers(socket_desc);
	      break; 
      case 3: 
	      getUname(socket_desc);
	      break; 
      case 4: 
	      getFileNames(socket_desc);
	      break;
      case 5:
	      printf("Terminating...\n");
	      exit(0); 
      default: 
	      printf("The choice does not exist");  
   }
   
  }
  close(socket_desc);
  return 0;
}

void getStudentId(int socketDesc){
    
    char users_option[] = "Option is 1";
    char server_reply[32];
     
    size_t payload_length = strlen(users_option) + 1; 

    //send the option 

    writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
    writen(socketDesc, (unsigned char *) users_option, payload_length); 

    //receive the userId 
    
    readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
    readn(socketDesc, (unsigned char *) server_reply, payload_length); 

    printf("The user id is: %s\n", server_reply); 
}

 void getRandomNumbers(int socketDesc){

 }

 void getUname(int socketDesc)
 {
 }

 void getFileNames(int socketDesc)
 {

 }


