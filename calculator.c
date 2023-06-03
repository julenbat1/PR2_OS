#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{			
	int number;
	int pid = getpid();

	char greet[40];
	sprintf(greet, "Process with pid %d spawned \n", pid);
	write(2, greet, strlen(greet));


		

	while(read(0, &number, sizeof(number)) > 0 )
	{
		char strNumber[40];
		int result = (int) number * pid;
		write(1, &result, sizeof(int));
		sprintf(strNumber, "%d", result);
		write(2, strNumber, strlen(strNumber));
		write(2, "\n", 1);
	}
}
