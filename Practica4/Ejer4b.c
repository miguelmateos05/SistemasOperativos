#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/file.h>

int main(void)
{
	int fd1, fd2, i, pos;
	char c;
	char buffer[6];

	fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	write(fd1, "00000", 5);

	for (i = 1; i < 10; i++)
	{
    	pos = lseek(fd1, 0, SEEK_CUR);
    	if (fork() == 0)
    	{
        	/* Child */
        	sprintf(buffer, "%d", i * 11111);

        	// Lock the file
        	//flock(fd1, LOCK_EX);

        	lseek(fd1, pos, SEEK_SET);
        	write(fd1, buffer, 5);
    	        //fsync(fd1);
        	// Unlock the file
        	//flock(fd1, LOCK_UN);
    
        	close(fd1);
        	exit(0);
    	}
    	else
    	{
        	/* Parent */
	wait(NULL);
        lseek(fd1, 5, SEEK_CUR);
	write(fd1, "00000", 5);
    	//fsync(fd1);
    	}
	}

	// Wait for all children to finish
	while (wait(NULL) != -1);

	lseek(fd1, 0, SEEK_SET);
	printf("File contents are:\n");
	while (read(fd1, &c, 1) > 0)
    	printf("%c", (char)c);
	printf("\n");
	close(fd1);
	exit(0);
}




