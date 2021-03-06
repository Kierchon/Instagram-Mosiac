#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	pid_t child, child2;
	int cstatus; //Exit status of child
	pid_t c;		//Pid of child to be returned by wait
     
    FILE *userInfo = fopen("userInfo.txt", "w");
    int pfds[2];
    pipe(pfds);
    if ((child = fork())==0 ){
        close(1);       /* close normal stdout */
        dup(pfds[1]);   /* make stdout same as pfds[1] */
        close(pfds[0]); /* we don't need this */
        execlp("curl", "curl", "https://api.instagram.com/v1/users/search?q=Kierchon&access_token=288688992.1fb234f.b71d32601a6746cb9809590d6b53b181", NULL);
    }else{//parent process for again
		 c = wait(&cstatus);
		 if ((child2 = fork()) == 0) {
        close(0);       // close normal stdin 
        dup(pfds[0]);   // make stdin same as pfds[0] 
        close(pfds[1]); // we don't need this 
        dup2(fileno(userInfo), STDOUT_FILENO);
        execlp("jsb", "jsb", NULL);
    	} else {
        //Parent process
		  c = wait(&cstatus);
			if (child == (pid_t)(-1)){
				printf("Fork failed\n"); exit(1);
			}else{
				c = wait(&cstatus); //Wait for child to complete
			//	printf("Child exited\n");
				// Start parsing here.
				char tmpid[65]; // Where we store the ID.
            char line[500]; // Each line. We might need a bigger length. Or smaller.
            userInfo = fopen("userInfo.txt", "r"); 
            fgets(line, sizeof(line), userInfo);
            puts(line);
            fgets(line, sizeof(line), userInfo);
            puts(line);
			}
		}
	}
	return 0;
}


