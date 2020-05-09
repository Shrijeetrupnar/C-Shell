#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>

//limits
#define MAX_TOKENS 100
#define MAX_STRING_LEN 100

size_t MAX_LINE_LEN = 10000;


// builtin commands
#define EXIT_STR "exit"
#define EXIT_CMD 0
#define UNKNOWN_CMD 99

#define BUFFSIZE 1024

pid_t pid_list[100];
int index_pid = -1;


FILE *fp; // file struct for stdin
char **tokens;
char *line;

void initialize()
{

	// allocate space for the whole line
	assert( (line = malloc(sizeof(char) * MAX_STRING_LEN)) != NULL);

	// allocate space for individual tokens
	assert( (tokens = malloc(sizeof(char*)*MAX_TOKENS)) != NULL);

	// open stdin as a file pointer
	assert( (fp = fdopen(STDIN_FILENO, "r")) != NULL);

}

void tokenize (char * string)
{
	int token_count = 0;
	int size = MAX_TOKENS;
	char *this_token;

	while ( (this_token = strsep( &string, " \t\v\f\n\r")) != NULL) {

		if (*this_token == '\0') continue;

		tokens[token_count] = this_token;

		printf("Token %d: %s\n", token_count, tokens[token_count]);

		token_count++;

		// if there are more tokens than space ,reallocate more space
		if(token_count >= size){
			size*=2;

			assert ( (tokens = realloc(tokens, sizeof(char*) * size)) != NULL);
		}
	}
}




int callpipe(char *input)
{


		int pipefd[2],status,done=0;
		pid_t cpid;

		pipe(pipefd);

		cpid = fork();


	 if (cpid == 0)
	 {
		 //printf("In CHILD-1 (PID=%d): executing command %s ...\n", getpid(), argv1[0]);

		 dup2(pipefd[1], STDOUT_FILENO);	// waitpid(-1,&status,0);
		// waitpid(-1,&status,0);

		 execvp(tokens[0], tokens);
	 }

	 cpid = fork();
	 // child2 executes
	 if (cpid == 0)
	  {
			 //	printf("In CHILD-2 (PID=%d): executing command %s ...\n", getpid(), argv2[0]);
			   close(pipefd[1]);
				 dup2(pipefd[0], STDIN_FILENO);

				 execvp(tokens[3], tokens);
	  }

	 // parent executes
	 close(pipefd[0]);
	 close(pipefd[1]);

		// waitpid(-1,&status,0);
		// waitpid(-1,&status,0);

return 0;
}



void read_command()
{

	// getline will reallocate if input exceeds max length
	assert( getline(&line, &MAX_LINE_LEN, fp) > -1);

	printf("Shell read this line: %s\n", line);

	tokenize(line);
}

int run_command()
{



	  char buffer[100];
		int Shell_exit = 0;

		while(Shell_exit != 1)
		{
						int ip_flag = 0;
						int op_flag = 0;
						int pipeflag =0;
						char *input_file_name = NULL;
						char *output_file_name = NULL;
						int i = 0;

						printf("Shrijeet-Shell->");


						read_command();

						 if (strcmp(line,"\n") == 0)
						{
										continue;
						}
						else
						{
										line[strcspn(line,"\n")]='\0';
						}


						int j = 0;
						while(tokens[j]!= NULL)
						{

						//	printf("inside of second while\n");
										if(strcmp(tokens[j],">")==0)
										{
														output_file_name = tokens[j + 1];
													//	printf("%s\n",output_file );
														break;

										}
										else if(strcmp(tokens[j],"<")==0)
										{
														input_file_name = tokens[j + 1];
														//printf("-----------%s\n",input_file );
														break;
										}

										j++;
						}



						if((strcmp(line,"quit") == 0)||(strcmp(line,"exit") == 0)|| (strcmp(line,"q") == 0))
						{
										for(int m = 0; m < 100; m++)
										{
														if(kill(pid_list[m],9)==-1)
														{
																		printf("ERROR IN KILL\n");
														}
										}

										Shell_exit = 1;
						}
						else if (strcmp(tokens[0],"cd") == 0)
						{
										if (chdir(tokens[1]) != 0) {
														fprintf(stderr, "Error: cd  to path %s failed\n", tokens[1]);
										}
						}
						else if (strcmp(tokens[0],"pwd") == 0)
						{
										char cwd[1024];
										if (getcwd(cwd, sizeof(cwd)) != NULL) {
														printf("Current Directory: %s\n", cwd);
										} else {
														perror("Error: pwd failed");
										}
						}
						else if ((strcmp(tokens[0],"echo") == 0))
						{
										perror("echo dosen't read std input \n");
										break;
						}




						else if (tokens[0] != NULL)
						{
										pid_t pid;
										int process_status;
										pid = fork();

									  if(pid<0)
										{
														perror("fork failed!!\n");
														exit(-1);
										}
										else if(pid == 0)
										{      if (input_file_name != NULL)
													 {
																	 int fd0;
																	 if ((fd0 = open(input_file_name, O_RDONLY, 0)) < 0)
																	  {
																					 perror("Couldn't open input file");
																					 exit(0);
																	  }
																	 dup2(fd0, 0);
																	 close(fd0);
													 }
													 if (output_file_name != NULL)
													 {
																	 int fd1;
																	 if ((fd1 = creat(output_file_name,0644)) < 0)
																	  {
																					 perror("Couldn't open the output file");
																					 exit(0);
																	  }

																	 dup2(fd1, 1);
																	 close(fd1);
													 }


													 if(pipeflag == 1)
													 {
														callpipe(line);

													 }
													 else
													 {
																	 execvp(tokens[0], tokens);
																//	 printf("%s:command not found\n",tokens[0]);
																	 exit(-1);
													 }
											}


										 else
										 {
														waitpid(pid, &process_status, 0);
									  	}

						}



		}





	return UNKNOWN_CMD;
}

int main(int argc, char const *argv[])
{
 printf("\n----------------------------Welcome to MyShell----------------\n");

	initialize();

  run_command();

printf("\n------------------------------End of MyShell----------------\n");

	return 0;
}
