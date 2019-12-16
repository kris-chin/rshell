#ifndef __REDIRECT__HPP__
#define __REDIRECT__HPP__
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

//dup
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//rename
#include <stdio.h>

#include "Word.hpp"
#include "Exec.hpp"
#include "Arg.hpp"
#include "Command.hpp"

using namespace std;

//handles both input and output redirection

class O_Source: public Command { //LHS OF >
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        O_Source(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "O Source: " << this->Base->get_word() << std::endl;
		this->sequence = Base->get_sequence(); //get sequence from base command
		
		int saved_stdin = dup(0);
		int TEMP_I_INPUT = access("TEMP_I_TARGET", F_OK);
		if (TEMP_I_INPUT == 0){ //if we already have existing input, also switch out our stdin
		    close(0);
		    open("TEMP_I_TARGET",O_RDWR);
		}

		//SET FILE TO BE NEW OUTPUT
		int saved_stdout = dup(1);
		close(1);
		int fileOut = open("TEMP_O_TARGET",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU); //create a new file called TEMP_REDIRECT. it's file descriptor is saved at 1.
		
		//RUN COMMAND AS NORMAL
		std::string result = this->Base->execute(); //call execute as normal. anything in sent to cout is sent to outFd.
		
		//SET STDOUT TO OUTPUT AGAIN
		dup2(saved_stdout,1);		
		if (TEMP_I_INPUT == 0){
		    dup2(saved_stdin,0);
		}
		//std::cout << "Reached end of Redirect!" << std::endl;
		return result;

	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        void set_run(bool value){this->Base->set_run(value);}
};

class O_Target : public Command{ //RHS of >
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        O_Target(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "O Target: " << this->Base->get_word() << std::endl;
		const char* FILEPATH = Base->get_word().c_str();

		//if FILEPATH is a real path, "set" the contents of FILEPATH to the contents to TEMP_REDIRECT. and then delete TEMP_REDIRECT
		int exists = access(FILEPATH,F_OK);
		if (exists == 0){ //delete FILEPATH, since it exists. we out here doing ghetto rewrites
		    remove(FILEPATH);
		}
		rename("TEMP_O_TARGET",FILEPATH);

		//run execute. this is if something is both an O_Target and an I_Source. we do know this is a file so we will disable it's allow_run
		this->Base->set_run(false);
		std::string result = this->Base->execute();

		return "16 0";
	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        void set_run(bool value){this->Base->set_run(value);}
};


class I_Target: public Command { //LHS of <
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;
	unsigned runCount = 0; //number of times the command has been run. used for tracking if the command was called again after the source.
	int saved_stdin;
	int openFd;

    public:
        I_Target(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "I Target: " <<  this->Base->get_word() << std::endl;
		//we want the command to run after the redirection, since the source is only defined after this command is called the first time.
		if (runCount == 0){
		    //std::cout << "first run" << std::endl;
		    //create a temporay file and set that to be our temporary stdin fd.
		    
		    saved_stdin = dup(0);
		    close(0);
		    openFd = open("TEMP_I_TARGET",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU); //create a new file called TEMP_I_TARGET. it's file descriptor is saved at 0
		    
		    runCount++;
		    return "16 3"; //tell the interpreter to run the command again after the next command.
		} else {
		    //std::cout << "next run" << std::endl;
		    //since this isn't the first time the command is being called, it is assumed that our TEMP_I_TARGET has been written to.
		
		    std::string result = this->Base->execute();
		    runCount = 0;
		    dup2(saved_stdin,0);
		    remove("TEMP_I_TARGET");	
		    //std::cout << "ended" <<std::endl;
		    //return result;
		    return result;
		}
			
	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        void set_run(bool value){this->Base->set_run(value);}
};

class I_Source : public Command{ //RHS of <
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        I_Source(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "I Source: " << this->Base->get_word() << std::endl;
		const char* FILEPATH = Base->get_word().c_str();

		//if FILEPATH is a real path, write to TEMP_I_TARGET, which should be the current stdin fd.
		int exists = access(FILEPATH,F_OK);
		if (exists == 0){
		    //write the contents of FILEPATH to  TEMP_I_TARGET
		    ifstream input; input.open(FILEPATH);
		    ofstream output; output.open("TEMP_I_TARGET");		    
		    
		    std::string line;
		    while (getline(input,line)){ //take every line in the input file and send it to the output file, "TEMP_I_TARGET", whose fd is our current input.
		        output << line << std::endl;
		    }
			
		    input.close(); output.close();
		} else {
		    //std::cout << "'" << FILEPATH << "' does not exist." << std::endl;
		}

		
		//run execute. this is if something is both an I_Source or an O_target. we do know that this is a file so we disable it's allow_run
		this->Base->set_run(false);
		std::string result = this->Base->execute();
		
		return "16 4"; //prime the interpreter to call the command in the register. we don't need to return any new result information since this is a file.
	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        void set_run(bool value){this->Base->set_run(value);}
};


#endif //__O_HPP__
