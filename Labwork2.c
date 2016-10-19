#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100

typedef struct timeval timeval;

void inline getTime(timeval *t); 
long int inline getTimeDiff(timeval *before, timeval *after); // in microseconds

int main() 
{ 
	timeval beforeFork, afterFork;
	int fd;
	char buf[BUF_SIZE]; 
	ssize_t numBytes; // that have been read or written from file
	size_t bufOffset = 0;

	if ((fd = open("./File", O_RDWR | O_CREAT, S_IRWXU)) == -1)
	{
		printf("Can't open file\n");
		return -1;
	}

	getTime(&beforeFork); 
	
	int st;
	if (fork() > 0)
	{
		wait(&st);

		lseek(fd, 0, SEEK_SET);

		while ((numBytes = read(fd, buf + bufOffset, BUF_SIZE)) > 0)
		{
			if (numBytes == -1)
			{
				printf("Read error\n");
				return -1;
			}
			
			bufOffset += numBytes;
		}

		printf("%s\n", buf);

		close(fd);
	}
	else
	{
		getTime(&afterFork);
	
		sprintf(buf, "Child process launch time: %ld microseconds",
									getTimeDiff(&beforeFork, &afterFork));
	
		size_t bufSize = strlen(buf);
		do
		{
			numBytes = write(fd, buf + bufOffset, bufSize);

			if (numBytes == -1)
			{
				printf("Write error\n");
				return -1;
			}

			bufOffset += numBytes;

		}
		while (bufOffset != bufSize);
	}
	return 0;
}

void getTime(timeval *t)
{
	static struct timezone tz;

	if (gettimeofday(t, &tz) == -1)
	{
		printf("Can't get time\n");
		exit(EXIT_FAILURE);
	}
}

long int getTimeDiff(timeval *before, timeval *after)
{
	return (after->tv_sec - before->tv_sec)/1000000 + 
								 after->tv_usec - before->tv_usec;
}
