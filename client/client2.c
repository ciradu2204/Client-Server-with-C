#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet.addr
#include<string.h> //strlen
#include"rdwrn.h" //readn() and written()
#include<unistd.h>
#include<stdlib.h> 

typedef struct {
  char sysname[50];
  char release[50];
  char version[50];
  char nodename[50];
  char machine[25];
} systemInfo;


int main(void)
{
  //declaration of functions to use in the while loop
  void getStudentId(int);
  void getRandomNumbers(int);
  void getUname(int, systemInfo *uname1);
  void getFileNames(int);
  void fileTransfer(int);  
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
   systemInfo *uname1;
   uname1 = (systemInfo *) malloc(sizeof(systemInfo));
   printf("1. Get student ID\n"); 
   printf("2. Get 5 random numbers\n"); 
   printf("3. The server uname\n"); 
   printf("4. Obtain file names in the current working directory\n");
   printf("5. Transfer a file from server to client\n"); 
   printf("6. Exits the program\n");
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
	      getUname(socket_desc, uname1);
	      break; 
      case 4: 
	      getFileNames(socket_desc);
	      break;
      case 5: fileTransfer(socket_desc); 
	      break; 
      case 6:
	      printf("Terminating...\n");
	      return;
      default: 
	      printf("The choice does not exist");  
   }
  free(uname1); 
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

 void getUname(int socketDesc, systemInfo *uname1)
 {
   int user_option = 3;  
   size_t payload_length= sizeof(systemInfo);

   //send the option 
   writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
   writen(socketDesc, (unsigned char *) &user_option, sizeof(int)); 

   //receive a struct
   readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
   readn(socketDesc, (unsigned char *) uname1, payload_length);

   //print the details recieved
   printf("==========Start of the Result==========\n"); 
   printf("Kernel name: %s\n", uname1->sysname);
   printf("Kernel version: %s\n",uname1->version);
   printf("Node name: %s\n", uname1->nodename);
   printf("Realease:  %s\n", uname1->release); 
   printf("Hardware Name: %s\n", uname1->machine); 
   printf("=======End of Result=================\n"); 
 }

 void getFileNames(int socketDesc)
 {
  int user_option = 4; 
  size_t payload_length;
  const char delimeter[2] = "*";
  char server_reply[2000]; 
  char *fileName; 

  //send the option 
  writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
  writen(socketDesc, (unsigned char *) &user_option, sizeof(int)); 

  //read the filenames string 
  readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
  readn(socketDesc, (unsigned char *) server_reply, payload_length); 

  printf("================Start of Result ==============\n");
  //parse the string 
  
    //get the first name of the file 
     fileName= strtok(server_reply, delimeter); 
     
     //When it's returning an error message
      if(fileName == NULL){
	  printf("%s\n", server_reply); 
       }

    //loop through the remaining filenames 
      while(fileName != NULL){
      printf("%s\n", fileName);

      fileName= strtok(NULL, delimeter);
      }
   printf("==========End of result=====================\n");
 }

 void fileTransfer(int socketDesc)
 {
    int user_option = 5; 
    char user_fileName[100];
    size_t payload_length; 
    FILE *fp;
    char buffer[2000]; 
    char errorMessage[100];

    //send the option 
    writen(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
    writen(socketDesc, (unsigned char *)& user_option, sizeof(int)); 

    //ask user for the file name
    printf("Please enter the file name\n");
    scanf("%s", user_fileName); 

    payload_length = strlen(user_fileName) + 1; 
    
    //send the user file name to the server
    writen(socketDesc, (unsigned char*) &payload_length, sizeof(size_t)); 
    writen(socketDesc, (unsigned char*)user_fileName, payload_length);
    
    //read the user response.
    readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t));
    readn(socketDesc, (unsigned char *) buffer, sizeof(FILE));
   
    //read the error if any
    readn(socketDesc, (unsigned char *) &payload_length, sizeof(size_t)); 
    readn(socketDesc, (unsigned char *) errorMessage, payload_length); 

    if(strlen(errorMessage) > 0){
    printf("=============The start of the result====================\n");
    printf("%s", errorMessage);
    printf("============The end of the result=======================\n");

    }else{
    //create a file name
    strcat(user_fileName, ".txt");
    //open the filename to write
    fp = fopen(user_fileName, "w");
    fprintf(fp, "%s", buffer);

    printf("=============The start of the result====================\n");
    printf("File transferred successfully");
    printf("============The end of the result=======================\n");
    }
 }

