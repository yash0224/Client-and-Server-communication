// server code with interactive menu.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>


#define CONNECT_CH_KEY 1024
#define SHM_SIZE 1024 // Shared memory size
#define MAX_CLIENTS 10

struct ConnectChannel
{
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


int num_clients = 0;
int num_response = 0;
char clientsName[MAX_CLIENTS][50];
int requests_serviced[MAX_CLIENTS];
int shared_memory_key[MAX_CLIENTS];

pthread_t tid[MAX_CLIENTS];
pthread_t del;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void sigint_handler (int signo)
{
   printf("\n\n\n\nClient Names   -   Requests serviced  -   shared memory  \n");	   
  
  int sum = 0;
  for (int i = 0; i < num_clients; i++ ){
  	printf("client- %s  ;   No. of request- %d  \n", clientsName[i], requests_serviced[i]);
  	sum = sum + requests_serviced[i]; }
  	 
  printf("\n[ PID - %d , ThreadID - %lu , SourceFile - server.c , Function - Total Requests, Line number - 64 ]\n\n", getpid(), pthread_self()); 
    
  exit (EXIT_SUCCESS);
}

void *client_thread(void *arg)
{   
    int key = *(int *)arg; 
    
    int index;
    for ( index = 0; index <num_clients ; index++){
    	if (shared_memory_key[index] == key){
    		break;}}		
    requests_serviced[index] = 1;
    int shmid = shmget(key, 1024, 0666);

    
    if (shmid == -1)
    {   
    	printf("Shmget. \n[ PID - %d , ThreadID - %lu , SourceFile - server.c , Function - Error, Line number - 88 ]\n\n", getpid(), pthread_self());
    	
        exit(1);}
    
    struct CommChannel *Comm;
    Comm = (struct CommChannel *)shmat(shmid, 0, 0);
    
   while(1){

	    while( Comm->rw_lock == 0);
	    pthread_mutex_lock(&mutex);	
	     
	    num_response = num_response + 1 ;
	    requests_serviced[index] = requests_serviced[index] + 1;

	    
	    if (Comm->request[0] == 0)
	    {    
	           
	           printf("Server has received unregister request\n.[ PID - %d , ThreadID - %lu , SourceFile - server.c , Function - Server has recieved unregsiter , Line number - 111 ]\n\n", getpid(), pthread_self()); 
	           
	           Comm->rw_lock = 0;
		    }
		
		
	    else
	     { 
		   
		if (Comm->request[1] == 2)
		{
		    if (Comm->request[2] % 2 == 0)
		    {
		        Comm->response = 0;
		        Comm->rw_lock = 0;}
		        
		    else
		    {
		        Comm->response = 1;
		        Comm->rw_lock = 0;}}
		        
		else if (Comm->request[1] == 3)
		{
		    int output = 0;
		    
		    // Check from 2 to square root of n
		    for (int i = 2; i < Comm->request[2]; i++)
			{if (Comm->request[2] % i == 0)
			    {output = 1; break;}}
			    
		    Comm->response = output;
		    Comm->rw_lock = 0;}
	
		else if (Comm->request[1] == 4)
		{
		    Comm->response = 0;
		    Comm->rw_lock = 0;}
		    
		else
		{
		    int a = Comm->request[2];
		    int b = Comm->request[3];
		    int c = Comm->request[4];
		    
		    if (c == 0)
		    {   Comm->response = a + b; /*addition operation*/ }
		    
		    else if (c == 1)
		    {   Comm->response = a - b; /* subtraction operation */}
		    
		    else if (c == 2)
		    {   Comm->response = a * b; /*multiplication operation  */ }
		    
		    else
		    {   Comm->response = a / b; /* division */ }

		    Comm->rw_lock = 0;}}
		    
		   
		   printf("Request processes & Answer = %d .\n[ PID - %d , ThreadID - %lu , SourceFile - server.c , Function - Service request response, Line number - 184 ]\n\n", Comm->response, getpid(), pthread_self()); 
		      
    pthread_mutex_unlock(&mutex);
    }};


int main() {

    
    if (signal (SIGINT, sigint_handler) == SIG_ERR)
    {
         fprintf (stderr, "Error in SIGINT!");
         exit (EXIT_FAILURE);}
    
    int client_key = 0;
    
    // Create shared memory segment
    int shmid = shmget(CONNECT_CH_KEY, SHM_SIZE, IPC_CREAT | 0666);

    if (shmid == -1) {

        perror("Shmget");
        exit(1);}
    
    // Attach shared memory segment to process address space
    struct ConnectChannel *Connect;

    Connect = (struct ConnectChannel*) shmat(shmid, NULL, 0);

    if (Connect == (void*) -1){
        perror("Shmat");
        exit(1);}

    // Initialize connection info
    Connect->connected = 0;
    Connect->server_write = 0;
    printf("Server started. Waiting for connection requests...\n[ PID - %d , ThreadID - %lu , SourceFile - server.c , Function - Status update , Line number - 233]\n\n", getpid(), pthread_self());

    // Server loop
    while (1) {

        // Check for connection request
        if (*Connect->request== '\0') {
            sleep(1); // Sleep for 1 second before checking again
            continue;}
             
        else {
            // Connection request received, handle it
            
            client_key++;
            char client_commkey[50];
            sprintf(client_commkey, "%d", client_key);

            if (strlen(Connect->request)>0){
            num_response = num_response + 1 ;
            
            for (int i = 0; i < 50 ; i++){
		clientsName[num_clients][i] = Connect->request[i];}
		
            shmid = shmget(client_key, SHM_SIZE, IPC_CREAT | 0666);
            if (shmid == -1) { perror("Shmget");
			exit(1);}
			
	    printf("Server has created a communication channel : key : %d For User Name - %s.\n[ PID - %d , ThreadID - %lu , SourceFile - server.c , Function - Creation of communication channel , Line number - 266]\n\n", client_key, Connect->request, getpid(), pthread_self());
            
	    for (int i = 0; i < 50 ; i++){
		Connect->response[i] = client_commkey[i];}
	    shared_memory_key[num_clients] = client_key;
	    num_clients = num_clients + 1;
   
	    
	    Connect->server_write = 1;
            Connect->connected = 0;
            
            pthread_create(&tid[num_clients], NULL, client_thread, &client_key);
            
            //Reset connection info
            memset(Connect->request, 0, sizeof(Connect->request));

	}}}
    
    Connect->connected = 1;
    return 0;
}
