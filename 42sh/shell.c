#include "ast.h"
#include "front.h"
#include <stdio.h>

void my_free_tree(void *pt)
{
    free_tree((node_t *)pt);
}



void initialize(void)
{
    // This code will be called once at startup
}

void run_command(node_t* node)
{
    // ...
    register_global_mem(node, &my_free_tree);
    // (for testing:)
    print_tree(node);

	
	// check if its a built in command

	int pid = fork();	
	if(pid){

		// if its not 0, then its the parent
		if(pid < 0){
			perror("AAAAAAAH");
			return 1;
		}
		else{
			// something failed
			// question why is there no fail check for the child? or is this it?
		}
	
	}
	else{

		// the child code goes here, the child has pid 0
		// if i understand it correctly the child should do the command that is asked and then exit when its done, allowing the shell itself (the parent) to keep running?


	}	
 
	
	// if its a regular command
	if (node->type == NODE_COMMAND){
		
		char *program = node->command.program;
		char **argv = node->command.argv;

		//printf("%s", argv[1]);
		execvp(program, argv);


	}
	// if its a pipe
	else if (node->type == NODE_PIPE){
		printf("pipe?");
	}
	// if its a sequence of commands
	else if (node->type == NODE_SEQUENCE){
		printf("sequence and shit"); 

		// somehow separate them???
		// is the max sequence 2, because there are only 2 parts in the struct?


		// console.log( kept for future reference to track yearly record of stupidity


		// get the partS out I HAVE NO IDEA BUT I GOT THIS RIGHT ON THE FIRST TRY????? WTF
		char *program = node->sequence.first->command.program;
		char **argv = node->sequence.first->command.argv;

		// 2nd part of sequence
		char *programtwo = node->sequence.second->command.program;
		char **argvtwo = node->sequence.second->command.argv;

		execvp(program, argv);
		execvp(programtwo, argvtwo);
	


	}




    // don't forget:
    free_from_global_mem(node);
}
