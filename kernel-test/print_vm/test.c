#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int n = 100;
	printf("pid = %d\n",getpid());
	while (n--) 
		sleep(3);
	return 0;
}
