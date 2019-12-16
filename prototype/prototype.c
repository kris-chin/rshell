#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h>

int main (){

    pid_t pid; 
    pid = fork();
    int status;

    if(pid == 0){
        printf("child process, pid = %u\n",getpid());
        printf("Executing child process\n");
        char* args[] = {"/bin/bash", "-c", "echo hello world", NULL};
        execvp(args[0], args);
    } else {
        printf("parent process, pid = %u\n",getppid());
        printf("Waiting for child process\n");
        if(waitpid(pid, &status, 0) > 0) {
            if (WIFEXITED(status) && !WEXITSTATUS(status)){  
              printf("program execution successful\n"); 
            }
        }
    }
    return 0;
}
