#ifndef __PIPE_HPP__
#define __PIPE_HPP__

#include "StratDecorator.hpp"
#include "../Interpreter.hpp"

#include <string>
#include <iostream>
#include <vector>

class Pipe : public StratDecorator{
        private:
                InterpreterStrat* Base;
		bool READING_NEXT_TOKEN; //if true, takes in the next token as 
        public:	
		// NEW / OVERWRITTEN
		Pipe(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> execute(){
			std::vector<std::string> calls;
			
			if (READING_NEXT_TOKEN == false){		
			    calls.push_back("04"); //set END_OF_COMMAND to true. but we don't turn off the strategy yet. we want to append a redirector command
			    calls.push_back("15 PIPE_START"); //set the command decorator.
			    this->READING_NEXT_TOKEN = true;
			} else { //TODO: you gotta make this expandable for multiple pieces
			    this->READING_NEXT_TOKEN = false;
			    calls.push_back("15 PIPE_PIECE"); //set the command decorator.

			    calls.push_back("12"); //turn off STRATEGY IN PROGRESSa
			    this->set_status(false);
			}
			return calls;
		};
		
		//NOT OVERWRITTEN
		virtual std::vector<std::string> interpret(std::string token){return Base->interpret(token);}
                virtual bool get_status() {return Base->get_status();}
		virtual bool set_status(bool value) {Base->set_status(value);}
		virtual std::string get_character() {return Base->get_character();}
		virtual int previous_must_be() {return -1;}
		virtual void reset_vars() {this->Base->reset_vars();}
};

#endif

