#ifndef __STRICTCHARSTRAT_HPP__
#define __STRICTCHARSTRAT_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>

class StrictCharStrat : public InterpreterStrat{ //similar to loosechar strat but the difference is that it runs different strats depending on the character

        private:
                bool status = false;
		std::string character;
        public:
		StrictCharStrat(std::string characters){this->character = characters;}
                virtual std::vector<std::string> interpret(std::string token){
			//calls pause to the interpreter every time the character matches, sets status to true if it's a 100% match.
			std::vector<std::string> calls;
			//IF POTENTIALLY THE CHARACTER(S)
			if (token.size() == character.size()){
			
  			    //DETERMINE IF VALID CONNECTOR (only partial. not invalid. ex. ">" and not ">blah" for ">>"
  			    bool PASSED_MATCHING_CHAR = false;
			    bool isValid = false;
			    for (auto ch = token.begin(); ch != token.end(); ch++){
				if (this->character.find(*ch) != std::string::npos){ //our token does contain a character from our strat
				    PASSED_MATCHING_CHAR = true;
				    isValid = true; //this may get overwritten to false if a wrong char
				} else {
				    //check if whitespace
				    if (*ch == ' '){
					//its just whitespace. we're good
					isValid = true;
				    } else { //if not whitespace, than its an invalid token
				        if (PASSED_MATCHING_CHAR == true){
					    //we actually do have an invalid character
					    isValid = false;
					} else {
					    //we're not even there yet
					}
				    }
				}
			    }
			    if (token == character){
			        if (isValid == true){
				    //std::cout << "we got it here" <<std::endl;
				    this->set_status(true);
			        }
				calls.push_back("03"); //disable pause
			    } else {
				if (isValid == true){ //if valid but not all of the needed token
				    this->set_status(true);
				    std::string spaceless;
				    for (auto ch = token.begin(); ch != token.end(); ch++){
				        if ((*ch) != ' '){
					    spaceless.push_back(*ch);
					}
				    }
				    
				    calls.push_back("09 " + spaceless); //override the token with the whitespaceless version of the captured character
				} else { //if invalid
				    //std::cout << token << " didnt make it" <<std::endl;
				}
			    	calls.push_back("03"); //disable pause
			    }
			} else {
			    if (token == character.substr(0,token.size())){
				calls.push_back("08");
				calls.push_back("02"); //enable pause
			    } else {
			    }
			}
			
			return calls;
		}
                virtual std::vector<std::string> execute(){;} //this is decorated by character-specific strats, so we just leave this empty
                virtual bool get_status() {return this->status;}
		virtual bool set_status(bool value) {this->status = value;}
		virtual std::string get_character() {return this->character;}
		virtual int previous_must_be() {return -1;} //this is also decorated
		virtual void reset_vars() {;} //no vars needed to reset
};

#endif

