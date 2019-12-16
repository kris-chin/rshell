#ifndef __QUOTES_HPP__
#define __QUOTES_HPP__

#include "StratDecorator.hpp"
#include "../Interpreter.hpp"

#include <string>
#include <iostream>
#include <vector>

class Quotes : public StratDecorator{
        private:
                InterpreterStrat* Base;
        public:	
		// NEW / OVERWRITTEN
		Quotes(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> execute(){
			std::vector<std::string> calls = this->Base->execute();
			
			if (calls.size() > 0){ //if we have calls (which we should have calls if the spanStrat properly executed)
			    calls.erase(calls.begin()); //delete the "this spanstrat is not decorated" message  
			    calls.at(0)[0] = '0'; calls.at(0)[1] = '9'; //set the new code to the TOKEN_OVERRIDE call
			    calls.at(0).erase(3,1); calls.at(0).erase(calls.at(0).size()-1,1); //delete the quotes from the span
			}
			return calls;
		};
		
		//NOT OVERWRITTEN
		virtual std::vector<std::string> interpret(std::string token){return Base->interpret(token);}
                virtual bool get_status() {return Base->get_status();}
		virtual bool set_status(bool value) {Base->set_status(value);}
		virtual std::string get_character() {return Base->get_character();}
		virtual void reset_vars() {this->Base->reset_vars();}
};

#endif

