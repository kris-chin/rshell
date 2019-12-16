#ifndef __TEST_COMMAND2_HPP__
#define __TEST_COMMAND2_HPP__


#include "../header/Word.hpp"
#include "../header/Command.hpp"
#include "../header/Arg.hpp"
#include "../header/Exec.hpp"

#include <vector>
#include <unistd.h>
#include <iostream>

#include <sys/wait.h>

using namespace std;

int main() {
    pid_t pid = fork();
    if (pid == 0){
    	//not our command
	char* argv[] = {"echo", "execvp is working", NULL};
 	cout << "testing native execvp()" <<endl;
	execvp(argv[0],argv);
    }else{
    	while (wait(0) > 0){ ;}
	//now for our command
    	vector<Word*> testSequence;
    	
    	Exec* test_exec = new  Exec("echo");
    	testSequence.push_back(test_exec);

    	Arg* test_arg = new Arg("execute_is_working");
    	testSequence.push_back(test_arg);
	
	Arg* test_arg2 = new Arg("part2");
	testSequence.push_back(test_arg2);
	
    	Command* test_command = new Command(testSequence);
    
	cout << "testing command execute()" <<endl;
        test_command->execute();
    }
    return 0;
}


#endif //__TEST_COMMAND2_HPP__
