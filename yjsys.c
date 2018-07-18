#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(){
	int file[2];
	char bufA[13] = "\nHello World\n";
	char bufB[13];

	printf("Now listing directories...\n");
	system("ls");

	file[0] = open("yjtest.txt", O_RDWR | O_CREAT);
	file[1] = open("yjtest.txt", O_RDWR);

	write(file[0], bufA, strlen(bufA));
	write(1, bufB, read(file[1],bufB,13));

	close(file[0]);
	close(file[1]);
	
	sleep(1);
	
	int pid = fork();
	if (pid!=0){
		printf("\nI am parent\n");
	}
	else{
		printf("\nI am child. PID = %d\n", getpid());
	}

	return 0;
}

