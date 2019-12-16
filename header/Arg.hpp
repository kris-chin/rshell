#ifndef __ARG__HPP__
#define __ARG__HPP__
#include <iostream>
#include <string>

#include "Word.hpp"

class Arg : public Word {
    protected:
        std::string word;

    public:
        Arg() {}
	Arg(std::string word){this->word = word;}
        std::string get_word(){return this->word;}
	void set_word(Word* word) {this->word = word->get_word();}
	std::string execute() {return "16 0";}
};
#endif //__ARG__HPP__
