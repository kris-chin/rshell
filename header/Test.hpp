#ifndef __TEST__HPP__
#define __TEST__HPP__
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

#include "Word.hpp"
#include "Exec.hpp"
#include "Arg.hpp"
#include "Command.hpp"

using namespace std;

class Test: public Command {
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        Test(Command* base) {
	    this->Base = base;
	}
	std::string get_word(){
	    return Base->get_word();
	}
	void set_word(Word* word){
	    this->Base->set_word(word);
	}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		return "10 " + this->get_word() ;
	}
	void add_word(Word* new_word){
		this->Base->add_word(new_word);
	}
	void set_run(bool value){ this->Base->set_run(value);}
        
};
#endif //__SPAN_HPP__
