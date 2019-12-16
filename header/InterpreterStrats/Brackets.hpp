#ifndef __BRACKETS_HPP__
#define __BRACKETS_HPP__

#include "StratDecorator.hpp"
#include "../Interpreter.hpp"

#include <string>
#include <iostream>
#include <vector>

class Brackets : public StratDecorator{
        private:
                InterpreterStrat* Base;
        public:	
		// NEW / OVERWRITTEN
		Brackets(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> execute(){
			std::vector<std::string> calls = this->Base->execute();
			
			if (calls.size() > 0){ //if we have calls (which we should have calls if the spanStrat properly executed)
			    calls.erase(calls.begin()); //delete the "this spanstrat is not decorated" message  
			   
			    int index = calls.size()-2; //this is the index of the 2nd-to-last call, which holds the full parentheses.
				calls.at(index)[0] = '0'; calls.at(index)[1] = '9'; //set call code to 09, which sets TOKEN_OVERRIDE to the STRING_INPUT which is below
				calls.at(index).erase(3,1); calls.at(index).erase(calls.at(index).size()-1,1); //delete the ends of the span
				calls.at(index).insert(2," test" );
			    	calls.push_back("15 TEST"); //set COMMAND_DECORATOR to "TEST".
			    
			    //for (unsigned i = 0; i < calls.size(); i++){
			    //    std::cout << "CALL " << i << ": '" << calls.at(i) << "'" << std::endl;
			    //}    
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

