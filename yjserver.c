#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>  /* needed by fork() */
#define BUFSIZE 80

void* Receiver(void *arg){
	int server_socket =*(int*)arg;
	int numbytes;
	char buf[BUFSIZE];
	do{
		memset(buf,0,BUFSIZE);
		numbytes = recv(server_socket,buf,BUFSIZE,0);
		printf("%s",buf);
		if(strstr(buf,"quit") != NULL){
			printf("..quitting..\n");
			exit(0);
		}
		else if(strstr(buf,"ls | wc -l") != NULL){
			printf("..counting..\n");
			system("ls | wc -l");
			printf("..completed..\n\n");
		}

		else if(strstr(buf,"ls") != NULL){
			printf("..listing directory..\n");
			system("ls");
			printf("..completed..\n\n");
		}
		}while(numbytes>0);
	return 0;
}

void* Sender(void *arg){
	int server_socket =*(int*)arg;
	char buf[BUFSIZE];
	while(1){
		memset(buf,0,BUFSIZE);
		fgets(buf,BUFSIZE,stdin);
		unsigned len = strlen(buf);
		buf[len]='\r';
		send(server_socket,buf,BUFSIZE,0);
	}
}


void main()
{	
	pthread_t tidReceiver[256], tidSender[256];

	int server_socket, client_socket[256], addr_len, PID;
	struct sockaddr_in addr;
	char buf[BUFSIZE];
	
	/* creatung server socket */
	server_socket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&addr,0,sizeof(addr));

	int one = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one));

	/*defining server address */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9898);
	addr.sin_addr.s_addr = INADDR_ANY;

	/* bing the socket to specified IP and port */
	bind(server_socket, (struct sockaddr *)&addr, sizeof(addr) );
	
	/* listen on socket, and can queue 20 connect requests */
	listen(server_socket, 20);


	int i = 0;
	while(1){	
		addr_len = sizeof(addr);
		client_socket[i] = accept(server_socket, &addr, &addr_len);

		printf("Client %d is connecting...\n",getpid(),buf);
		
		if (pthread_create(&tidReceiver[i], NULL, &Receiver, &client_socket[i])){
			perror("Fail create Receiver thread");
		}
		if (pthread_create(&tidSender[i], NULL, &Sender, &client_socket[i])){
			perror("Fail create Sender thread");
		}

		i++;
	
	}
	

	pthread_join(tidReceiver, NULL);
	pthread_join(tidSender, NULL);

	close(client_socket); 
	close(server_socket);
	
	return 0;	
}
