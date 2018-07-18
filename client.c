#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define BUFSIZE 80

int main(){
	int server_socket;
	struct sockaddr_in addr;

	server_socket = socket(PF_INET,SOCK_STREAM,0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9898);
	
	inet_aton("127.0.0.1",&addr.sin_addr); //convert string to int

	connect(server_socket,&addr,sizeof(addr));
	
	char buf[BUFSIZE];
	while(1){
		
		memset(buf,0,BUFSIZE);
		fgets(buf,BUFSIZE,stdin);
		unsigned len = strlen(buf);
		buf[len]='\r';
		send(server_socket,buf,BUFSIZE,0);
	}

	close(server_socket);
	
	return 0;
}
