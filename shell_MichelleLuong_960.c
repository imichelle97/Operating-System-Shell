/*
 * shell_MichelleLuong_960.c
 *
 *  Created on: Sep 19, 2017
 *      Author: michelle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 80 /* The maximum length command */

/*
 * Function prototypes
 */
unsigned int split_cmd(char* a_str, char** a_buffer, const char a_delim);

/*
 * Main Function
 */
int main(void) {

	char *args[MAXLINE/2 + 1];	/* Command line with max 40 arguments */
	int should_run = 1;			/* Flag to determine when to exit program */
	int background = 0;			/* Flag to determine background tasks */
	char cmd[MAXLINE + 1];
	int status;
	printf("CS 149 Shell from Michelle Luong\n");

	while(should_run) {
		printf("Michelle-960> ");	/* prompt */
		fflush(stdout);

		fgets(cmd, MAXLINE, stdin);		/* Gets the command */
		cmd[strlen(cmd)-1]='\0';		/* Replace the '\n' at the end of string with '\0' */
		int num_of_args = split_cmd(cmd, args, ' ');

		/*
		 * If user types in the exit command, shell will exit the program
		 */
		if(strcmp(args[0], "exit") == 0){
			should_run = 0;
		}

		/*
		 * Check whether user included an & at the tail end of command line to determine whether or not the
		 * shell (parent) process is to wait for the child to exit
		 */
		if((num_of_args > 0) && (strcmp(args[num_of_args - 1], "&") == 0)) {
			background = 1;
			args[num_of_args - 1] = NULL;
			num_of_args--;
		}

		if((num_of_args > 0) && (should_run == 1)) {
			pid_t pid = fork();
			if(pid < 0) {
				printf("Fork Failed\n");
			}
			else if(pid == 0) {	/* CHILD PROCESS */
				execvp(args[0], args);
				printf("Exec Failed\n");
				exit(1);
			}
			else {	/* PARENT PROCESS */
				if(background == 0) {
					while(wait(&status) != pid);
				}
				else {
					background = 0;
				}
			}
		}
	}
}

/* Split command function takes in an array of char string (a_str), array of string of string pointers (aka string of
 * array, called a_buffer), and a_delim) *
 */
unsigned int split_cmd(char* a_str, char** a_buffer, const char a_delim) {

	char delim[2];
	char* token = NULL;
	char* saveptr;

	unsigned int num_token = 0;
	delim[0] = a_delim;
	delim[1] = 0;

	token = strtok_r(a_str, delim, &saveptr);

	/*
	 * Empty string
	 */
	if(token == NULL) {
		*a_buffer = NULL;
		return 0;
	}

	while(token) {
		*(a_buffer + num_token) = token;
		token = strtok_r(0, delim, &saveptr);
		num_token++;
	}

	/*
	 * Terminate the array of strings with a null string
	 */
	*(a_buffer + num_token) = '\0';
	return num_token;

}




