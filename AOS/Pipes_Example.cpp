#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using namespace std;

int main() {
	pid_t pid;
	int fd[2];
	int ret = pipe(fd);
	char message[] = "";

	char buf[20];

	for (;;) {
		cout << "what to send?" << endl;
		cin >> message;
		if (ret == -1) {
			perror("pipe");
			exit(1);
		}
		pid = fork();
		if (pid == 0) {
			// this is the child process...
			cout << "Child:" << endl;
			write(fd[1], message, 20);

		}
		else {
			cout << "Parent" << endl;
			//  this is the parent process....
			read(fd[0], buf, 20);
			cout << " Buf: ";
			cout << buf << endl;
		}
	}

	return 0;
}