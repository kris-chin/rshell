#ifndef __DOUBLEPIPE_HPP__
#define __DOUBLEPIPE_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>
#include <iostream>

class Doublepipe : public StratDecorator{

        private:
		InterpreterStrat* Base;
                bool status;
                std::string character;
        public:
                Doublepipe(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> interpret(std::string token){return this->Base->interpret(token);}
                virtual std::vector<std::string> execute(){
		    std::vector<std::string> calls;
		    calls.push_back("04"); //enable tokenReset
		    calls.push_back("12");
		    this->set_status(false);
		    return calls;
		}
                virtual bool get_status() {return Base->get_status();}
                virtual bool set_status(bool value) {Base->set_status(value);}
		virtual std::string get_character() {return Base->get_character();}
		virtual int previous_must_be() {return 1;}
		virtual void reset_vars() {this->Base->reset_vars();}
};	

#endif

