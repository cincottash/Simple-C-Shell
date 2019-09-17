#include <glob.h>
#define max_buf_size 1024
#include "get_path.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#include "pwd.h"
#define MAXLINE 128
 
struct pathelement *p;

char *which(char *command, struct pathelement *pathlist );
char *where(char *command, struct pathelement *pathlist );

int sh( int argc, char **argv, char **envp )
{
  char new_prompt[MAXLINE];  //Used in prompt command, holds our new prompt string
  char *previous1;  //used in cd to hold previous directory
  char *previous2;  //used in cd to hold previous directory
  char	buf[MAXLINE];  
  pid_t	pid;
  char *cwd;
  char *ptr; //used to hold our prompt
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);    //Get passwd info
  homedir = password_entry->pw_dir;  //Home directory to start out with


  if((pwd = getcwd(NULL, PATH_MAX+1)) == NULL ){
    perror("getcwd");
    exit(2);
  }

  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  pathlist = get_path();  //put PATH into a linked list

  strcpy(new_prompt, "ShaneShell:");

  while (go){
	//handles control c and control v
	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, signal_stp_handler);

	ptr = getcwd(NULL, 0); //ptr holds the current working directory

	printf("%s%s%s", new_prompt, ptr, " >>> ");  //Printing out prompt

    	/* get command line and process */
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		int len = strlen(buf);
		buf[len-1] = '\0';   //removes a newline and replaces with NULL char
		int itteration = 0;  //itteration is the argument count
		char *buf1[3][128];  //Making a 3x128 array to hold our arguments
		char *token = strtok(buf, " ");  //parse the command line by a space
		while (token != NULL) { //parsing command line
			 if(strstr(token, "*") != NULL || strstr(token, "?") != NULL){
				  glob_t  paths;
  				  int     csource;
				  int 	  hsource;
  				  char    **p;
  				  printf("argv[0] = %s\n", argv[0]);
  				  printf("argv[1] = %s\n", argv[1]);  /* as pathname */
  				  /* Find all ".c" files in a given directory */
  				  csource = glob("*.h", 0, NULL, &paths);
				  //hsource = glob("*.h", 0, NULL, &paths);
  				  if (csource == 0) {
    				    for (p = paths.gl_pathv; *p != NULL; ++p){
      					//strcpy(args[itteration], *p);
					puts(*p);
					//itteration++;
					//globfree(&paths);
					//break;
				    }//for
					globfree(&paths);
					//break;
  				  }//if


			  }//if

			 else {
			   //printf("%s\n", token);
			   strcpy(buf1[itteration], token);
        		   itteration = itteration +1;
			   token = strtok(NULL, " ");
			}


    		}

		if (buf[strlen(buf) - 1] == '\n'){
			buf[strlen(buf) - 1] = 0; // replace newline with null

		}
                if (strcmp(buf, "pwd") == 0) {   // built-in command pwd
		  printf("Executing built in command: pwd...\n");
	          printf("Getting Current Working Directory...\n");
                  printf("CWD = [%s]\n", ptr);

	        }

		else if(strcmp(buf, "exit") == 0) {
			printf("Executing built in command: exit...\n");
			printf("Exiting Shell...\n");
			exit(0);
	    	}

		else if(strcmp(buf, "pid") == 0){
			printf("Executing built in command: pid...\n");
			printf("Getting pid...\n");
			printf("%d\n", getpid());
		}

		else if(strcmp(buf1[0], "which") == 0){
			printf("Executing built in command: which...\n");
			printf("Running which...\n");
			which(buf1[1], pathlist);
		}

		else if(strcmp(buf1[0], "where") == 0){
			printf("Executing built in command: where...\n");
			printf("Running where...\n");
			where(buf1[1], pathlist);

		}

		else if(strcmp(buf1[0], "cd") == 0){
			printf("Executing built in command: cd...\n");
			previous1 = getcwd(NULL,0);
			if (itteration == 1) {
				if(chdir(getpwuid(getuid())->pw_dir) < 0){
					printf("Failed to change directory\n");
			}
		else {
			previous2 = previous1;
			printf("cd to home: success\n");
			ptr = getcwd(NULL, 0);
		}

		} else if (strcmp(buf1[1],"-") == 0){
			printf("Going back\n");
			if(chdir(previous2)<0){
				printf("Failed to change directory\n");
			} else {
				previous2 = previous1;
				printf("cd back: success\n");
				ptr = getcwd(NULL, 0);
			}
		} else if (chdir(buf1[1]) < 0){
			printf("cd to %s failed\n",buf1[1]);
		} else {
			previous2 = previous1;
			printf("cd to %s succeeded\n",buf1[1]);
			ptr = getcwd(NULL, 0);
		}

		}

		else if(strcmp(buf1[0], "list") == 0){
				printf("Executing built in command: list...\n");
				printf("%d\n",itteration);
				  if(itteration == 1){
				    printf("Running list...\n");
				    char cwd[max_buf_size];
				    getcwd(cwd, sizeof(cwd));
				    printf("%s\n", cwd);
				    list(cwd);
			       	  }

				 else if(itteration == 2){
					printf("Running list...\n");
					int i;
					//list for all args
					for(i = 1; i < itteration; i++)
					{
						printf("%s\n", buf1[i]);
						list(buf1[i]);

					}

				}

		}

		else if(strcmp(buf1[0], "kill") == 0){
			printf("Executing built in command: kill...\n");
			if (itteration == 3) {
                	  char *pid_str = buf1[2]; //get pid to kill
                	  char *signal_str = buf1[1];  //get signal

                	  char *end;
                	  long pid_num; 
                	  long sig_num;

                	  //strtol because supports error catching
                	  pid_num = strtol(pid_str, &end, 10);
                	  //converting pid
                	  if (end == pid_str) {
                    	    printf("%s\n", "Cannot convert string to number");
               	  	  }
                	  //get rid of the "-" flag
                  	signal_str[0] = ' ';
                  	sig_num = strtol(signal_str, &end, 10);

                  	if (end == signal_str) {
                      	  printf("%s\n", "Cannot convert string to number");
                	}

                	int id = (int) pid_num;
                	int sig = (int) sig_num;
                	kill(id, sig_num);
                      }//if

		else if (itteration == 2) {
                	char *pid_str = buf1[1];
                	char *end;
                	long num;
                	num = strtol(pid_str, &end, 10);
                	if (end == pid_str) {
                    	printf("%s\n", "Cannot convert string to number");
                	}
                	int id = (int) num;
                	kill(id, SIGTERM);
               }


	     else {
                printf("%s\n", "Incorrect argument count");
            }

	  }//else if kill

		else if(strcmp(buf1[0], "printenv") == 0){
			printf("Executing built in command: printenv...\n");
			if(itteration == 1){
				printenv(envp);
			}

			else if(itteration == 2){
				print_env_variable(buf1[1]);
			}

			else{
			  printf("Incorrect argument count\n");

			}

		}

		else if(strcmp(buf1[0], "setenv") == 0){
			printf("Executing built in command: setenv...\n");
			printf("Running setenv\n");
			if(itteration == 1){
				printenv(envp);
			}
			
			else if(itteration == 2){
				set_env(buf1[1], " ");
				if(strcmp(buf1[1], "PATH") == 0){
					free_pathlist(pathlist);  //clearing the path before we set it ourselves
					pathlist = get_path();
				}

			}

			else if(itteration == 3){
				set_env(buf1[1], buf1[2]);
				if(strcmp(buf1[1], "PATH") == 0){
					free_pathlist(pathlist);
					pathlist = get_path();
				}
			
			}

			else{
			  printf("Incorrect argument count\n");
			}

		}

		else if(strcmp(buf1[0], "prompt") == 0){
			printf("Executing built in command: prompt...\n");
			if(itteration == 1){
				printf("Enter a new prefix string\n");
				scanf("%s", new_prompt);
			}
			else if(itteration == 2){
				strcpy(new_prompt, buf1[1]);
				//printf(new_prompt);

			}

		}




		else {                           /* external command */
			if ((pid = fork()) < 0) {
			printf("fork error\n");
			exit(1);
		}

		else if (pid == 0) {		/* child */
			execlp(buf, buf, (char *)0);
			printf("couldn't execute: %s\n", buf);
			exit(127);
		}

		/* parent */
		if ((pid = waitpid(pid, &status, 0)) < 0)
			printf("waitpid error\n");
             }
	     printf("%s%s%s%s", new_prompt,":", ptr, " >>> ");
	}
    /* check for each built in command and implement */

     /*  else  program to exec */
    //{
       /* find it */
       /* do fork(), execve() and waitpid() */

      //else
        //fprintf(stderr, "%s: Command not found.\n", args[0]);
    //}
  }
  free(owd);
  free(args);
  free(prompt);
  free(commandline);
  free(ptr);
  exit(0);
} /* sh() */



char *which(char *command, struct pathelement *pathlist){
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
	char cmd[64];
	while(pathlist){
	  sprintf(cmd, "%s/%s", pathlist->element, command);
	  printf("Checking [%s]\n", cmd);
	  if(access(cmd, F_OK) == 0){
	  	printf("[%s]\n", cmd);
		break;

	  }
	  pathlist = pathlist->next;
	}

}

//similar to which but we return all the locations the exe exists
char *where(char *command, struct pathelement *pathlist ){
	char cmd[64];
	while(pathlist){
	  sprintf(cmd, "%s/%s", pathlist->element, command);
	  printf("Checking [%s]\n", cmd);
	  if(access(cmd, F_OK) == 0){
	  	printf("[%s]\n", cmd);

	  }

	  pathlist = pathlist->next;

	}
}

void list (char *dir){
  DIR* directory;
  struct dirent* entryp;
  directory = opendir(dir);
  if(directory == NULL){
    printf("Could not open directory %s\n", dir);
  }
  else{
    while((entryp = readdir(directory)) != NULL){
      printf("%s\n", entryp->d_name);
    }
    closedir(directory);
  }
}


void printenv(char **envp){
  char **env;
  env = envp;
  while(*env){
    printf("%s\n", *env);
    env++;
  }
}


void print_env_variable(char *env_var){
  char *env = getenv(env_var);
  if(env != NULL){
    printf("%s\n", env);
  }
  else {
    printf("Enviornment does not exist\n");
  }
}


void set_env(char *envname, char *envval){
  setenv(envname, envval, 1); 
}


void free_pathlist(struct pathelement *head){
  struct pathelement *current = head;
  struct pathelement *temp;
  while(current->next != NULL){
    temp = current;
    current = current->next;
    free(temp);
  }
  free(current);
}



//signal handler for SIGINT 
void sigint_handler(int sig_num){
  signal(SIGCHLD, sigint_handler);
  fflush(stdout);
  return;
}

void signal_stp_handler(int sig_num){
  signal(SIGTSTP, signal_stp_handler);
  fflush(stdout);
  return;
}


