#ifndef __SINGLECHARSTRAT_HPP__
#define __SINGLECHARSTRAT_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>

class SingleCharStrat : public InterpreterStrat{

        private:
                bool status = false;
		std::string character;
        public:
		SingleCharStrat(std::string character){this->character = character;}
                std::vector<std::string> interpret(std::string token){
			if (token == character){
				this->status = true;
			}
			return {"-1"};
		}
                std::vector<std::string> execute(){return {"-1"};} //this is decorated by character-specific strats, so we just leave this empty
                bool get_status() {return this->status;}
		bool set_status(bool value) {this->status = value;}
		std::string get_character() {return this->character;}
		int previous_must_be() {return -1;} //this is also decorated
		void reset_vars() {;} //no vars to reset
};

#endif

