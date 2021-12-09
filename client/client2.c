#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet.addr
#include<string.h> //strlen
#include"rdwrn.h" //readn() and written()
#include<unistd.h>
#include <stdlib.h>

typedef struct {
  char name[50]; 
  char release[50]; 
  char version[50]; 
  char nodeName[50];
  char hardwareName[25]; 
  char hardwarePlatform[25];
  char processorType[50]; 
  char operatingSystem[50]; 
} uname; 

int main(void)
{
   //declaration of functions to use in the while loop
   void getStudentId(int);
   void getRandomNumbers(int);
   void getUname(int);
   void getFileNames(int);	
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
    
    int user_option = 1;
    char server_reply[100];
     
    size_t payload_length;

    //send the option 

    writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
    writen(socketDesc, (unsigned char *) &user_option, sizeof(int)); 

    //receive the userId 
    
    readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
    readn(socketDesc, (unsigned char *) server_reply, payload_length); 
    
    printf("==============Start of Result==========\n");
    printf("The Student is prefixed with ip address: %s", server_reply);
    printf("===============End of Result==========\n"); 
}

 void getRandomNumbers(int socketDesc){
  
 int user_option = 2; 
 int server_reply[5]; 
 int i;
 size_t payload_length; 

 //send the option 
 writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
 writen(socketDesc, (unsigned char *) &user_option, sizeof(int));

 //recieve the random numbers
 readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
 readn(socketDesc, (unsigned char *) server_reply, payload_length);

 printf("==============Start of Result==========\n");
 printf("The five random numbers are ");
 for(i=0; i<5; i++){
  printf("%d ", server_reply[i]); 
  
  if(i==4){
   printf("\n");
  }
 }
 printf("===============End of Result==========\n");
 }

 void getUname(int socketDesc)
 {
   int user_option = 3; 
   uname *uname1; 
   size_t payload_length;

   //send the option 
   writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
   writen(socketDesc, (unsigned char *) &user_option, sizeof(int)); 

   //receive a struct
   readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
   readn(socketDesc, (unsigned char *) uname1, payload_length);
 }

 void getFileNames(int socketDesc)
 {

 }


