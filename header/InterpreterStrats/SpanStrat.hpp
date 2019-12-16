#ifndef __SPANSTRAT_HPP__
#define __SPANSTRAT_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>

class SpanStrat : public InterpreterStrat{

        private:
                bool status = false;
		std::string character;
		std::vector<std::string> captures; //a vector of strings that contains our "captured strings". the size of the vector is  nesting. (ex captures.at(0) = outer span, captures.at(1) = inner span
		std::vector<std::string> spans; //all completed captures are pushed to this vector
		std::vector<std::string> calls;
		bool spanComplete; //set to true when the final span is completed.
		
		void OpenSpan(){ //helper function for opening a new span
		    captures.push_back(""); //append an empty string to the captures vector (signifies adding a layer)
		    this->set_status(true); //activate
		    calls.push_back("08"); //enable inAction so no other strats get in the way
		    calls.push_back("02"); //enable pause
		    //std::cout << "Span opened. captures size is now: " << captures.size() <<std::endl;
		}
		
		void CloseSpan(){ //helper function for closing a span
		    if (captures.size() == 0){
		    	std::cout << "INVALID INPUT" <<std::endl;
		    }
		    std::string prevSpan = captures.at(captures.size()-1);
		    spans.push_back(prevSpan);
		    //std::cout << "Span closed. Span held \"" << prevSpan << "\". Captures size is now: " << captures.size()-1 <<  std::endl;
		    captures.pop_back();
		    if (captures.size() != 0){
		        //cout << "this was a nested span. appending \"" << prevSpan << " to " << captures.at(captures.size()-1) << std::endl;
		        captures.at(captures.size()-1).append(prevSpan);
		        //cout << "at is now: \"" << captures.at(captures.size()-1) << std::endl;
				
		        calls.push_back("08"); //enable inAction to continue interpretation
				
		    } else {
		        calls.push_back("03"); //disable pauses
		        //cout << "nested span is complete. final span = \"" << prevSpan << "\"" << std::endl;
		        spanComplete = true;
		    }
		}
		
		void PrintCaptures(){ //prints all spans
		    for (unsigned i = 0; i < captures.size(); i++){
		    	//cout << "Capture " << i << ": \"" << captures.at(i) << "\"" <<endl;
		    }
		}
		
		std::string CombinedCaptures(){ //returns all captures in a combined string
		    std::string returnString;
		    for (unsigned i = 0; i < captures.size(); i++){
		        returnString.append(captures.at(i));
		    }
		    return returnString;
		}

        public:
		SpanStrat(std::string characterPair){
		    if (characterPair.size() == 1){ characterPair.append(characterPair);} //double our character to match our format
		    this->character = characterPair;
		}
                std::vector<std::string> interpret(std::string token){
			calls.clear();
			//std::cout << "interpreting " << token << endl;
			if (token == string(1,character.at(0))){ //BEGINNING CHARACTER
			    OpenSpan();
			} else if (token.at(token.size()-1) == character.at(1)){ // ENDING CHARACTER
		    	    if (captures.size() == 0){
			    	//std::cout << "INVALID INPUT" << std::endl;
			    } else {
			        captures.at(captures.size()-1).append(string(1,token.at(token.size()-1)));
			        CloseSpan();
			    }
			} else {
			    if (this->get_status() == true){ //if capturing.
				    //cout << "Checking to see if at(), which is'" << captures.at(captures.size()-1) << "', is in " << token << "..." <<endl;
				    //we're only appending to at() the differing characters between at() and the token
			 	    if (token.find(captures.at(captures.size()-1)) != std::string::npos){ //if the token contains at(), then the token contains the at() + new differences.
				       // cout << "it is, ";
				        if (token.at(token.size()-1) == character.at(0)){
					    //cout << "and the token's last character is the same as the openSpan character." <<endl;
					    OpenSpan();
				        } else if (captures.at(captures.size()-1) == ""){
					    //cout << "but at is blank." << endl;
					    size_t index = token.find(CombinedCaptures());
					    if (index == std::string::npos){
						//cout << "combinedCaptures is not in the token, so we set index to 0" << endl;
					        index = 0;
					    } else {
						//cout << "comboinedCaptures is IN the token, so we set index to combinedCaptures.size()" << endl;
						index = CombinedCaptures().size();
				  	        //cout << "token.substr(index) =  " << token.substr(index) << endl;
					    }
					    std::string differingCharacters = token.substr(index);
					    //cout << "so we will set at to the differing characters between the token (" << token << ") and the combined captures, (" << CombinedCaptures() << "), which is " << differingCharacters << endl;
					    captures.at(captures.size()-1) = differingCharacters;
					} else { //closing spans shuoldn't make it here. only opening spans
					    //cout << "and the token's last character is NOT the same as the openSpan character." <<endl;
					    //cout << "so '" << captures.at(captures.size()-1) << " will have " << token.substr(CombinedCaptures().size()) << " appended to it since it contains the differences" <<  endl;
					    captures.at(captures.size()-1).append(token.substr(CombinedCaptures().size()));
				        }
				    } else { //if the token doesn't contain at(), we know we can just append.
				        //cout << "it does not" <<endl;
				        captures.at(captures.size()-1).append(token);
				    }
				PrintCaptures();
				calls.push_back("08"); //enable inAction
			    }
			}
			return calls;
		}
		
		//spanstrat decorators MUST call their base's exectute() and MODIFY the output to be returned at the end of the call.
                std::vector<std::string> execute(){
			calls.clear();
			//call something to delete the ends of the span and sets the token to the span only if the whole span is completed.
			if (spanComplete == true){
				//cout << "SPAN COMPLETE. FINAL SPAN: \"" << spans.at(spans.size()-1) << "\"" <<  endl;
				this->set_status(false);
				spanComplete = false; //reset
				
				calls.push_back("13 ERROR: If you are seeing this message, a SpanStrat is undecorated.");
				for (unsigned i = 0; i < spans.size(); i++){
				    calls.push_back("-1 " + spans.at(i)); //return every span.
				}
				spans.clear(); //clear spans
				calls.push_back("12"); //clear STRATEGY_IN_PROGRESS
				
			    	return calls;
			} else {
			    return calls;
			}
			
		}
                bool get_status() {return this->status;}
		bool set_status(bool value) {this->status = value;}
		std::string get_character() {return this->character;}
		int previous_must_be() {return -1;} //this is also decorated
		void reset_vars() {
		    this->spanComplete = false;
		    this->captures.clear(); 
		    this->spans.clear();
		    this->calls.clear();
		}
};

#endif

