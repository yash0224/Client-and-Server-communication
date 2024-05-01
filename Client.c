// client code wiht interactive menu.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pthread.h>


#define SHM_SIZE 1024 // Shared memory size
#define SHM_KEY 12345


struct ConnectChannel{
    int connected;
    char request[50];
    char response[50];
    int server_write;
};


struct CommChannel
{
    int request[5];
    int response;
    int rw_lock;
};

char Names[10][50];
int num_clients = 0;
	
int Name_check(char client_name[50]){
	int size = sizeof(Names)/sizeof(Names[0]);
	for (int i = 0 ; i < size ; i++){
	   if(strcmp(client_name,Names[i])==0)
	   	return 0;}
	   	
	return 1;}
	
int main() {
        
		
	struct ConnectChannel *Connect;
	
	// The same shared memory key used by the server
	key_t connect_key;
	connect_key = 1024;
	
	// Get the shared memory segment created by the server
	int shmid = shmget(connect_key,1024,0666);
		    if (shmid == -1) {
			perror("Shmget");
			exit(1); }

	// Attach shared memory segment to process address space
	Connect = (struct ConnectChannel*) shmat(shmid,0,0);
		
	while(true){
			
		printf( "CHOOSE: \n 1. New Client (Sign up) \n 2. Existing Client (Sign In)\n 3. Delete Account \n.[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Sign In , Line number - 72 ]\n\n", getpid(), pthread_self());
		
		int Choose_a;
		scanf("%d", &Choose_a);
		
		if(Choose_a == 1){
		
		    key_t connect_key;
		    connect_key = 1024;
			
			// Get the shared memory segment created by the server
			int shmid;
			shmid = shmget(connect_key,1024,0666);
				    if (shmid == -1) {
					perror("Shmget");
					exit(1); }

			// Attach shared memory segment to process address space
			Connect = (struct ConnectChannel*) shmat(shmid,0,0);
		            
		    
		    printf("Please Enter client name : \n [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Input name, Line number - 99 ]\n\n", getpid(), pthread_self());
		
		    char client_name[50];
		    scanf("%s", client_name);
		    	
		    
		    if (Name_check(client_name) == 0){
		    	printf("\nClient name should be unique . \n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Error, Line number - 110 ]\n\n", getpid(), pthread_self());
		    	continue;}
		    	
		   for (int i = 0; i < 40 ; i++){
		   	Names[num_clients][i] = client_name[i];
			Connect->request[i] = client_name[i];
			}
		   num_clients = num_clients +1 ;

		    Connect->connected = 1;
		    
	            while( Connect->server_write == 0 );
			    
	            printf("\nClient: Response from server: %s    . \n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 124 ]\n\n",Connect->response, getpid(), pthread_self());
	            
	            
	            printf("Client connects to the server on the comm channel.\n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Comm channel connection , Line number - 132 ]\n\n", getpid(), pthread_self());
	            
                    Connect->connected = 0;
                    Connect->server_write = 0;
	            continue;} 
	//	--------	     	    
		if (Choose_a == 2) {

			int client_key;
			char name[50];
			printf("\nEnter client key:       \n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Input, Line number - 142]\n\n", getpid(), pthread_self());
			scanf("%d", &client_key);
			
	                
		     	shmid = shmget(client_key,1024,0666);
			    if (shmid == -1) {
				perror("\nThe shared memory isnt present, client not registered to server.");
				continue;
			    }
			
			
			struct CommChannel *comm;  
			comm = (struct CommChannel*) shmat(shmid,0,0);
		     	comm->request[0] = 1;
		     	comm->rw_lock = 0;
		    
		    int Choose_af;
		    
		    printf(" Functions: \n 1.Arithmetic \n 2.EvenOrOdd \n 3.IsPrime \n 4.IsNegative  \n [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Interactive menu, Line number - 166 ]\n\n", getpid(), pthread_self());
		    
		    scanf("%d", &Choose_af);
		    comm->request[1] = Choose_af;
			   
	         if (Choose_af == 1){
		 
			    int num1, num2, operator;
			    printf("You have chosen Arithmetic. Please enter two integers followed by an arithmetic operator (Type:number + = 0 , - = 1 , * = 2, / = 3)\n [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Input , Line number - 174 ]\n\n", getpid(), pthread_self());
		    	
			    scanf("%d", &num1);
			    comm->request[2] = num1;
			    scanf("%d", &num2);
			    comm->request[3] = num2;
			    scanf("%d", &operator);
			    comm->request[4] = operator;
			    comm->rw_lock = 1;
			   
			    while(comm->rw_lock == 1);
			    
			    printf("\n\nThe server side output is %d    .\n ",comm->response);
			    printf("[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 187 ]\n\n",getpid(), pthread_self());
			    
			    }
			    
			    
		 else if (Choose_af == 2){
		        int num1;		        
		    	printf("You have chosen EvenOrOdd. Please enter one integer\n [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Input , Line number - 194 ]\n\n", getpid(), pthread_self());
		        scanf("%d", &num1);
		        
		        comm->request[2]= num1;
		        comm->rw_lock = 1;
		       

		        while(comm->rw_lock == 1);
		        
		        if (comm->response){
		        	printf("The server side output is : the number is odd \n  [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 204 ]\n\n", getpid(), pthread_self());}
		        else 
		        {printf("The server side output is : the number is even \n  [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 206 ]\n\n", getpid(), pthread_self());}}
			

		  else if (Choose_af == 3){

			int num1;
		    	printf("You have chosen IsPrime. Please enter one integer. \n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Input , Line number - 212]\n\n", getpid(), pthread_self());
		        scanf("%d", &num1);
		        comm->request[2] = num1;
		        comm->rw_lock = 1;
		    
			while(comm->rw_lock == 1);
			if (comm->response){
		        	printf("The server side output is : the number is not a prime number \n  [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 218 ]\n\n", getpid(), pthread_self());}
		        else 
		        {printf("The server side output is : the number is a prime number \n  [ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 221 ]\n\n", getpid(), pthread_self());}}
	
		   if (Choose_af == 4){printf("Not supported \n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Response from server, Line number - 223 ]\n\n", getpid(), pthread_self());}
		   
	           
	           }
		    
		  
	        if (Choose_a == 3){	           	 
			    int client_key;
			    
			    // attaching the shared memory segment
			    printf("Enter client key:       \n[ PID - %d , ThreadID - %lu , SourceFile - client.c , Function - Input, Line number - 238]\n\n", getpid(), pthread_self());
			    scanf("%d", &client_key);
			    // fetch client's communication channel
			    	struct CommChannel *comm;
			     	shmid=shmget(client_key,1024,0666);
			     	comm = (struct CommChannel*) shmat(shmid,0,0);
			     	
			     	
			    if (shmdt(Connect) == -1) {
				perror("shmdt\n");
				exit(1);
			    }
	           	    
			    comm->request[0] = 0;
			    comm->rw_lock = 1;
			    
			    while(comm->rw_lock == 1);
			    
			     // Detach shared memory segment
			    int shmid = shmget(client_key, 1024, 0666);

			    // Remove shared memory segment
			   int result = shmctl(shmid, IPC_RMID, NULL);
			    if ( result == -1) {
				perror("Shmctl\n");
				exit(1);}}}
	
	return 0;
	}
