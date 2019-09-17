# Simple-C-Shell
The objectives of this project are to learn how a Unix shell works, to write a simple shell, to create processes, to handle signals, to use Unix system calls and C library function calls, and to become a better programmer. 

## Overview
To begin, a loop will print a prompt (consisting of a "prefix," which is initially a blank (space), followed by ' [cwd]> ', where cwd is the "current working directory," and will execute commands until the exit command is entered. In the loop you should check if the command is one of your built-in commands or a program in the search paths. If the command entered is neither one of your built-in commands nor in the search paths, you should print "command: Command not found." Where command is the command name that was entered. 

## Project Goals

 
## v0.8 
### **Features/commands implemented:**
1) *which* - Searches the path for executable, prints the first instance that is found in the path
2) *where* - Searches the path for executable, prints every instance that is found in the path.
3) *cd* - chdir(2) to directory given; with no arguments, chdir to the home directory, with a '-' as the only argument, chdirs to directory previously in
4) *pwd* - print the current working directory.
5) *list* - with no arguments, lists the files in the current working directory one per line. With arguments, lists the files in each directory given as an argument, with a blank line then the name of the directory followed by a : before the list of files in that directory.
6)  *pid* - prints the pid of the shell
7)  *kill* - When given just a pid sends a SIGTERM to it with kill(2). When given a signal number (with a - in front of it) sends that signal to the pid. (e.g., kill 5678, kill -9 5678).
8) *prompt* - When ran with no arguments, prompts for a new prompt prefix string. When given an argument make that the new prompt prefix.
9) *printenv* - Works the same as the tcsh built-in one. When ran with no arguments, prints the whole environment.
10) *setenv* - Works the same as the tcsh built-in one. When ran with no arguments, prints the whole environment, the same as printenv. When ran with one argument, sets that as an empty environment variable. When ran with two arguments, the second one is the value of the first. When ran with more args, print the same error message to stderr that tcsh does.
11) Ctrl-C caught and ignored if the shell is prompting for a command, but sent to the running child process otherwise.  Ctrl-Z, and SIGTERM are ignored.

## How to run:
1) Clone this repo
2) cd to the directory containing the src files
3) In the commandline enter and run *make*    
4) In the commandline enter and run *./mysh*    
5) Once in shell, run command *exit* to exit

### **Features/commands NOT YET implemented:**  
1) Support of the * wildcard character when a single * is given.  This should work just like it does in csh/tcsh when noglob is not set.  
2) Support of the ? wildcard character which should match any single character (exactly one character match) in a filename (anywhere in the filename). The * and ? should also work together. 

## Known Bugs  


## Patch Logs  
**v0.8:**  Fixed CWD and prompt not being updated after use of *cd* command  
**v0.8.2:** Fixed *cd ..* command not updating the prompt to the current working directory  

