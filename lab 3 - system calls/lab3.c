/*
Lab 3 Part 1
Olivia Nye and Jacob Waxman
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h> 
#include<string.h>
//Question #1 - Fork -- When both the parent and the child change the value of x,
	// x is first changed by the parent, then when the child process is entered,
	// x is reverted to it's original value and then changed by the child process.
int Fork(int val){
	int x = val;
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child (new process)
		//check the value of x in the child process
		printf("hello, I am child (pid:%d). The value of x is : %d\n", (int) getpid(), x);
		
		//have child change the value of x
		x = 80;
		//check the new value of x in the child process
		printf("hello, I am child (pid:%d). The NEW value of x is : %d\n", (int) getpid(), x);
		exit(0);
	} else {
		// parent goes down this path (main)
		printf("hello, I am parent of %d (pid:%d). The value of x is : %d\n",rc, (int) getpid(), x);
		//have parent change the value of x
		x = 90;
		//check the new value of x in the child process
		printf("hello, I am parent of %d (pid:%d). The NEW value of x is : %d\n",rc, (int) getpid(), x);
	}
	return 0;
}
//Question #2 - Fork and Open-- Yes, both the parent and the child can write to the same fd.
	// Writing concurrently does not seem to be a problem. 
int Fork_and_Open(char filename[]){
	int fd = open(filename, O_RDWR);
	int sz;
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child (new process)
		printf("hello, I am child (pid:%d)\n", (int) getpid());
		//child writes to file
		sz = write(fd, "I am the child and I am writing this in the file\n", strlen("I am the child and I am writing this in the file\n"));
		printf("the child has written something to the file\n");		
		exit(0);
	} else {
		// parent goes down this path (main)
		printf("hello, I am parent of %d (pid:%d).\n",rc, (int) getpid());
		//parent writes to file
		sz = write(fd, "I am the parent and I am writing this in the file\n", strlen("I am the parent and I am writing this in the file\n"));
		printf("the parent has written something to the file\n");	
	}
	close(fd);
	return 0;
}

//Question #3 - Yes, we can, but in order to ensure the specified order without calling wait, you need to scan for user input in the parent function, 
	//or block the parent function in another way.
int Fork_and_Print(){
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child prints "hello"
		printf("hello\n");
		exit(0);
	} else {
		char pointlessVar[100];
		//prompt for input
		printf("type any number or letter and press enter to continue\n");
		//delay parent process by taking io
		scanf("%s", &pointlessVar[0]);
		printf("goodbye\n");	
	}
	return 0;
}

//Question #4 - The different exec function calls allow for different arguments to be passed through the system. 
	//e,v,l,p can be added to the end of exec to specify arguments. This makes the exec function easier for varied use cases.
int Fork_and_Exec(){
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		printf("I'm the child\n");
		exit(0);
	} else {
		printf("I'm the parent and I'm about to call exec\n");
		char *args[2];
		args[0] = "/bin/ls";
		args[1] = NULL;
		execvp(args[0], args);
	}
	return 0;
}

//Question 5 - Wait returns the PID of the child. If you call wait the child 
	//instead of the parent, it will ensure that the parent finishes running before the child runs.
int Wait(){
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child prints "hello"
		printf("hello\n");
		
		int childPID = getpid();
		exit(0);
		//try using wait in the child
		//wait(NULL);
	} else {

		int y = wait(NULL);
		//shows us that wait returns the pid of the child
		printf("%d\n", y);
		
		printf("goodbye\n");	
	}
	return 0;
}

//Question 6 -- waitpid would be preferred over wait when there are multiple child processes
	// or when debugging and you're looking for where an error occurs.
int Waitpid(){
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child prints "hello"
		printf("hello\n");
		exit(0);
	} else {

		int y = waitpid(rc, NULL, 0);
		
		printf("goodbye\n");	
	}
	return 0;
}

//Question 7 -- If the child calls printf after closing the descriptor, the print will 
	//not occur because the child will have already terminated. 
int Standard_Out(){
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child prints "hello"
		close(STDOUT_FILENO);
		printf("hello\n");
		exit(0);
	} else {

		
		printf("goodbye\n");	
	}
	return 0;
}


int main(int argc, char *argv[]) {
	int x = 100;
	Fork(x);
	Fork_and_Open(argv[1]);
	Fork_and_Print();
	Fork_and_Exec();
	Wait();
	Waitpid();
	Standard_Out();
	return 0;
	
	
}