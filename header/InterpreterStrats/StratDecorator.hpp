#ifndef __STRATDECORATOR_HPP__
#define __STRATDECORATOR_HPP__

#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>

class StratDecorator : public InterpreterStrat{
        private:
		InterpreterStrat* Base;
                bool status;
                std::string character;
		vector<std::string> calls;
        public:
                virtual vector<std::string>  interpret(std::string token){return Base->interpret(token);}
                virtual vector<std::string> execute (){return Base->execute();}//this is overwritten in subclasses
                virtual bool get_status() {return Base->get_status();}
		virtual std::string get_character() {return Base->get_character();}
		virtual int previous_must_be() {return -1;} //this is also decorated
		virtual void reset_vars() {this->Base->reset_vars();}
};

#endif
