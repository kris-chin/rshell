#ifndef __COMMENT_HPP__
#define __COMMENT_HPP__

#include "StratDecorator.hpp"
#include "../Interpreter.hpp"

#include <string>
#include <iostream>
#include <vector>

class Comment : public StratDecorator{
        private:
                InterpreterStrat* Base;
        public:	
		// NEW / OVERWRITTEN
		Comment(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<std::string> execute(){
			return {"00"}; //set ignore to true
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

