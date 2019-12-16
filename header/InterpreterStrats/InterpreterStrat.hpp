#ifndef __INTERPRETERSTRAT_HPP__
#define __INTERPRETERSTRAT_HPP__

#include "../Interpreter.hpp"
#include <string>
#include <vector>

class Interpreter;

class InterpreterStrat{

	private:
		bool status;
		std::string character;
	public:
		virtual std::vector<std::string> interpret(std::string token) = 0; //always called. if the passed-in token matches the interpreter, it sets it's status to true
		virtual std::vector<std::string> execute() = 0; //only called if status is true. takes in the intepreter so it can access the interpreter's functions
		virtual bool get_status() = 0; //returns the status
		virtual bool set_status(bool value) = 0; //sets the status
		virtual std::string  get_character() = 0;
		virtual int previous_must_be() = 0; //conditional execution
		virtual void reset_vars() = 0;
};

#endif
