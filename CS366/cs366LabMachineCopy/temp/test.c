#include<stdio.h>
#include<unistd.h>

int
main() {	
	FILE *fp;
	fp = fopen("file.txt", "w");
	int fd = fileno(fp);
	printf("%d\n", fd);
	sleep(500);

	return 0;
}
