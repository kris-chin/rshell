#ifndef __PIPE_REDIRECT__HPP__
#define __PIPE_REDIRECT__HPP__
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

class Pipe_Start: public Command { //BEGINNING OF PIPE
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;
	int mainPipe;

    public:
        Pipe_Start(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "Pipe Start: " << this->Base->get_word() << std::endl;
		this->Base->execute();
		return "16 0";

	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        void set_run(bool value){this->Base->set_run(value);}
};

class Pipe_Piece : public Command{ //COMMANDS TO FEED INTO
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        Pipe_Piece(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "Pipe Piece: " << this->Base->get_word() << std::endl;
		
		int saved_stdin = dup(0);
		close(0);
		int I_TARGET = open("TEMP_I_TARGET",O_RDWR);

		//since our in is still the TEMP_I_TARGET, this should write to TEMP_I_TARGET. we'll also temporarily set our out to TEMP_I_TARGET and run this.
		int saved_stdout = dup(1);
		close(1);
		int piece_temp = open("PIECE_TEMP",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU); //open PIECE_TEMP, which should be now our STOUT
		
		//run the command. this will output to piece_temp
		this->Base->execute();
		
		//go back to default stdout
		dup2(saved_stdout,1);
		dup2(saved_stdin,0);
		//now manually rewrite TEMP_I_TARGET. yes this is super hacky. i dont care.
		ifstream input; input.open("PIECE_TEMP");
		ofstream output; output.open("TEMP_I_TARGET");
		
		std::string line; while (getline(input,line)){
		    //std::cout << "detected line" << std::endl;
		    output << line << std::endl;
		}
		input.clear();output.clear();
		remove("PIECE_TEMP");
		return "16 4";
	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        void set_run(bool value){this->Base->set_run(value);}
};


#endif //__PIPE_REDIRECT_HPP__
