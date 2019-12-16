#ifndef __IO_REDIRECT_HPP__
#define __IO_REDIRECT_HPP__

#include "StratDecorator.hpp"
#include "../Interpreter.hpp"

#include <string>
#include <iostream>
#include <vector>

class O_Redirect : public StratDecorator{
        private:
                InterpreterStrat* Base;
		bool READING_NEXT_TOKEN; //if true, takes in the next token as 
        public:	
		// NEW / OVERWRITTEN
		O_Redirect(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> execute(){
			std::vector<std::string> calls;
			
			if (READING_NEXT_TOKEN == false){		
			    calls.push_back("04"); //set END_OF_COMMAND to true. but we don't turn off the strategy yet. we want to append a redirector command
			    calls.push_back("15 O_SOURCE"); //set the command decorator to O_SOURCE
			    this->READING_NEXT_TOKEN = true;
			} else {
			    this->READING_NEXT_TOKEN = false;
			    calls.push_back("15 O_TARGET"); //set the command decorator to O_TARGET

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

class I_Redirect : public StratDecorator{
        private:
                InterpreterStrat* Base;
		bool READING_NEXT_TOKEN; //if true, takes in the next token as 
        public:	
		// NEW / OVERWRITTEN
		I_Redirect(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> execute(){
			std::vector<std::string> calls;
			
			if (READING_NEXT_TOKEN == false){		
			    calls.push_back("04"); //set END_OF_COMMAND to true. but we don't turn off the strategy yet. we want to append a redirector command
			    calls.push_back("15 I_TARGET"); //set the command decorator to I_TARGET
			    this->READING_NEXT_TOKEN = true;
			} else {
			    this->READING_NEXT_TOKEN = false;
			    calls.push_back("15 I_SOURCE"); //set the command decorator to I_SOURCE

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

