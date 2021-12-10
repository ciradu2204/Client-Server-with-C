#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"rdwrn.h"
#include<string.h> //strlen
#include<stdlib.h>
#include<unistd.h> 
#include<pthread.h> //threading
#include<sys/utsname.h> //uname
#include<dirent.h>
#include<errno.h>
#include<sys/stat.h>

typedef struct {
  char sysname[50];
  char release[50];
  char version[50];
  char nodename[50];
  char machine[25];
} systemInfo;


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
  void handleSystemInfo(int, systemInfo *uname1);
  void handleReadFromFile(int);
  void fileTransfer(int); 
  
  systemInfo *uname1; 
  uname1 = (systemInfo *) malloc(sizeof(systemInfo)); 

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
		 handleSystemInfo(sock, uname1); 
		 break; 
	    case 4: 
		 handleReadFromFile(sock); 
	    case 5: 
		fileTransfer(sock);  

    }
   
  }

  free(uname1);

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

void handleSystemInfo(int sock, systemInfo *uname1){
  struct utsname uname_pointer; 
  size_t payload_length = sizeof(systemInfo); 
  
  //Get the system information
  uname(&uname_pointer);

  //Edit the uname structure
  strcpy(uname1->sysname,uname_pointer.sysname); 
  strcpy(uname1->release,uname_pointer.release); 
  strcpy(uname1->version,uname_pointer.version); 
  strcpy(uname1->machine,uname_pointer.machine); 
  strcpy(uname1->nodename,uname_pointer.nodename); 
  
  //send the uname structure to the client
  writen(sock, (unsigned char *) &payload_length, sizeof(size_t)); 
  writen(sock, (unsigned char *) uname1, payload_length);

}

void handleReadFromFile(int sock){
  DIR * dir = opendir("upload");
  struct dirent **namelist;  
  char fileNamesList[2000]; 
  char errorMessage[100];
  int n;

  if(dir){
   //directory exist 
     //Read all the files in the upload directory in alphabetic order
      n= scandir("upload", &namelist, NULL, alphasort); 
      if(n==-1){
	  strcpy(errorMessage,"Can't read the directory");
	  perror("can't read the directory");
	  exit(EXIT_FAILURE);
      }
      //read the filenames
      struct stat *stats;
      stats =  malloc(sizeof(stat));

      while(n--){
	  char path[50] = "/upload/";
	  //get the file pathname
	  //strcat(path, namelist[n]->d_name);
	  if(stat(path, stats)<0){
	   //int err = errno;  
	  }
          
           //check if the filename has read access
	   if((stats->st_mode && S_IROTH) && S_ISREG(stats->st_mode)){
                  strcat(fileNamesList, namelist[n]->d_name);
		  strcat(fileNamesList, "*");
	    }


	     free(namelist[n]);
	   
       }
      free(stats);
      free(namelist); 
     
   
   closedir(dir);
  }else if(ENOENT == errno){
  strcpy(errorMessage,"The directory does not exist");
  perror("The directory does not exist");
  }else if(EACCES == errno){
  strcpy(errorMessage, "You don't have access to the directory");
  perror("You don't have access to the directory");
  }else{
  strcpy(errorMessage,"Unknown error while trying to open the directory"); 
  perror("Unknown error  while trying to open the directory");
  }

  

  //send the name of files to the user and error messages

  if(strlen(fileNamesList) == 0){
  
  size_t payload_length= strlen(errorMessage) + 1; 
  writen(sock, (unsigned char *) &payload_length, sizeof(size_t));
  writen(sock, (unsigned char *) errorMessage, payload_length);

  }else{
  size_t payload_length = strlen(fileNamesList) + 1;
  writen(sock, (unsigned char *) &payload_length, sizeof(size_t));
  writen(sock, (unsigned char *) fileNamesList, payload_length);
  }
 
}

void fileTransfer(int sock){

 char fileName[100];
 size_t payload_length; 
 char errorMessage[100];
 char path[100]; 
 char fileBuffer[2000];
 FILE *fp; 

 //read the user file
 readn(sock, (unsigned char *) &payload_length, sizeof(size_t)); 
 readn(sock, (unsigned char *) fileName, payload_length); 

 //create a path 
 strcat(path, "upload/");
 strcat(path, fileName);
 struct stat filestate;
 
 //check the status of the file
 if(stat(path, &filestate) <0){
   strcpy(errorMessage, "The file does not exit");
   perror("The file does not exit");   
 }else{
   fp = fopen(path, "r"); 
   //can't open the filename
   if(fp == NULL){
   strcpy(errorMessage, "the file can not be open"); 
   perror("The file can not be open");
   }else{
   fscanf(fp, "%s", fileBuffer);

   payload_length = strlen(fileBuffer) + 1; 
   
   }
   //send the filename
   payload_length = strlen(fileBuffer) + 1;
   writen(sock , (unsigned char *)&payload_length, sizeof(size_t));
   writen (sock, (unsigned char *)fileBuffer, payload_length);

   payload_length = strlen(errorMessage) + 1;
   writen(sock, (unsigned char *)&payload_length, sizeof(size_t));
   writen(sock, (unsigned char *)errorMessage, payload_length);


   fclose(fp); 
 }


}
