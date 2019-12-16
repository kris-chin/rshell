#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>



#include "Word.hpp"
#include "Command.hpp"
#include "Exec.hpp"
#include "Arg.hpp"
#include "Connector.hpp"
//INTERPRETER STRATS
#include "InterpreterStrats/InterpreterStrat.hpp"

#include "InterpreterStrats/SingleCharStrat.hpp"
#include "InterpreterStrats/Comment.hpp"
#include "InterpreterStrats/Semicolon.hpp"
#include "InterpreterStrats/Pipe.hpp"

#include "InterpreterStrats/MultiCharStrat.hpp"
#include "InterpreterStrats/Doublepipe.hpp"
#include "InterpreterStrats/Doubleamp.hpp"

#include "InterpreterStrats/LooseCharStrat.hpp"
#include "InterpreterStrats/IO_Redirect.hpp"

#include "InterpreterStrats/StrictCharStrat.hpp"

#include "InterpreterStrats/SpanStrat.hpp"
#include "InterpreterStrats/Quotes.hpp"
#include "InterpreterStrats/Paren.hpp"
#include "InterpreterStrats/Brackets.hpp"
//COMMAND DECORATORS
#include "Span.hpp"
#include "Test.hpp"
#include <boost/filesystem.hpp> //used for test  command
#include "Redirect.hpp"
#include "Pipe_Redirect.hpp"

using namespace std;
using namespace boost::filesystem;

class Interpreter {
    private:
 	std::vector<std::string> calls; //holds interpreter calls. clears after they are all called
	bool HALT_INTERPRETATION = false; //if true, don't even interpret input
	bool APPEND_TO_PREVIOUS_TOKEN = false; //if true, assumes current input is a part of previous input
	bool END_OF_COMMAND = false; //if true, resets token interpretation. (used for new commands)
	int wordCount = 0; //the number of words interpreted. resets when token is resetted
	bool INTERPRETATION_IN_PROGRESS = false; //if true,the interpreter is mid-interpretation (ex. n-char). switches to false at the end of each loop
	std::string TOKEN_OVERRIDE; //if this string is not empty, the interpreter replaces the current token with the value of this string and clears the override.
	std::string STRATEGY_IN_PROGRESS; //if this string is not empty, the interpreter can only pay attention to the strategy in this string.
	
	int PREVIOUS_EXECUTE = -1; //the status of the previous command->execute(). used for nested commands
	Word* WORD_REGISTER = nullptr; //a pointer to a word. used as a register for commands when calling execute();
	std::vector<Word*>* CURRENT_WORD_LIST; //a pointer to the current word_list that the interpreter is working on. used for referencing the wordlist outside of the read_line function call	

	std::vector<std::string> COMMAND_DECORATOR; //if not, empty, decorates interpreted commands with the values in the vector


	//strategies
	std::vector<InterpreterStrat*> strats = {
	    new Comment(new SingleCharStrat("#")),
	    new Semicolon(new SingleCharStrat(";")),
	    new Doublepipe(new StrictCharStrat("||")),
	    new Doubleamp(new MultiCharStrat("&&")),
	    new Quotes(new SpanStrat("\"")),
	    new Paren(new SpanStrat("()")),
	    new Brackets(new SpanStrat("[]")),
	    new O_Redirect(new LooseCharStrat(">>")),
	    new I_Redirect(new SingleCharStrat("<")),
	    new Pipe(new SingleCharStrat("|")) //gonna use a different character for now for testing purposes
	};


    public:
        Interpreter() {};
        
	void InterpreterCall(std::string input){
	    //format: "XX string input"
	    //the first 2 characters are an interpreter code. this is followed by a whitespace
	    //all of the characters after the whitespace is string input that is used for some codes. 
	    int INTERPRETER_CODE = -1;
	    try{
	        INTERPRETER_CODE = std::stoi(input.substr(0,2),nullptr);
	    } catch(...){
	        cout << "Invalid Interpreter Code: '" << input.substr(0,2) << "'" << endl;
	   	return;
	    }
	    std::string STRING_INPUT;
	    if (input.size() > 2){
	    	STRING_INPUT = input.substr(3);
	    } else {
	        STRING_INPUT = "NO STRING INPUT";
	    }
	    //cout << "INT CODE: " << INTERPRETER_CODE << endl;
	    //cout << "STRING_INPUT: " << STRING_INPUT <<endl;
	   
	    switch (INTERPRETER_CODE){
	        case 0: this->HALT_INTERPRETATION = true; break; //0 = set ignore to true
	        case 1: this->HALT_INTERPRETATION = false; break; //1 = set ignore to false
	        case 2: this->APPEND_TO_PREVIOUS_TOKEN = true; break; //2 = set pause to true. used for both N-char and Span
	        case 3: this->APPEND_TO_PREVIOUS_TOKEN = false; break; //3 = set pause to false. used for both N-char and Span
	        case 4: this->END_OF_COMMAND = true; break; //4 = turn on tokenReset
	        case 5: this->END_OF_COMMAND = false; break; //5 = turn off token reset
	        case 6: this->wordCount++; break; //6 = increment wordCount
	        case 7: this->wordCount--; break; //7 = decrement wordCount
	        case 8: this->INTERPRETATION_IN_PROGRESS = true; break; //8 = in Action. turns off at the end of each token loop
		case 9: this->TOKEN_OVERRIDE = STRING_INPUT; break; //9 = set TOKEN_OVERRIDE to string input
		case 10: this->PREVIOUS_EXECUTE = temp_execute(STRING_INPUT); break; //10 = immediately run STRING_INPUT as a command and save the result in PREVIOUS_EXECUTE
		case 11: temp_insert(CURRENT_WORD_LIST, temp_read(STRING_INPUT)); break; //interprets STRING_INPUT and pushes all contents to to CURRENT_WORD_LIST
		case 12: this->STRATEGY_IN_PROGRESS = ""; break; //12 = clears STRATEGY_IN_PROGRESS
		case 13: cout << STRING_INPUT << endl; break; //13 = output STRING_INPUT
	        case 14: //set all vars to false
		    this->HALT_INTERPRETATION = false;
		    this->APPEND_TO_PREVIOUS_TOKEN = false;
		    this->END_OF_COMMAND = false;
		    this->INTERPRETATION_IN_PROGRESS = false;
		    this->STRATEGY_IN_PROGRESS = "";
		    break;
		case 15: this->COMMAND_DECORATOR.push_back(STRING_INPUT); break; //15 = sets COMMAND_DECORATOR to STRING_INPUT
		case 16: this->PREVIOUS_EXECUTE = std::stoi(STRING_INPUT.substr(0,1),nullptr); //16 = sets PREVIOUS_EXECUTE to the int version of the first character of STRING_INPUT
		default: break;
	    }
	}
	
	std::string convertExecute(int value){ //helper function that returns a string version of the value.
	    switch (value){
		case -1: return "(Invalid)";
		case 0:  return "(True)";
		case 1:  return "(False)";
		default: return "(Invalid)";
		}
	}

	void temp_insert(std::vector<Word*>* a, std::vector<Word*> b){ //a helper function for temporary vector insertion since we can't pull this off in the switch case
		a->insert(a->end(),b.begin(),b.end());   
	}
	
	std::vector<Word*> temp_read(std::string input){ //helper function that creates an interpreter that reads input and returns a word vector
	    Interpreter* temp = new Interpreter;
	    std::vector<Word*> word_list = temp->read_line(input);
	    delete temp;
	    return word_list;
	}

	int temp_execute(std::string input){ //helper function that creates a temporary interpreter that reads and runs the input. returns the result
	    Interpreter* temp = new Interpreter;
	    //cout << "plugging in '" << input << "' into temporary interpreter." << endl;
	    int result = temp->execute(temp->read_line(input));    
	    delete temp;
	    return result;
	}
	
	void makecalls(){ //helper function that makes all interpreter calls in calls vector
	    for (auto call = this->calls.begin(); call != this->calls.end(); call++){
	    	InterpreterCall(*call);
	    }
	    this->calls.clear();
	}
	
	void debug(int layer, std::string m){ //a debug helper function that just prints a colored message with a certain amount of indentation`
	    for (int i = 0; i < layer; i++){std::cout << "    ";}
	    std::cout << m << std::endl;
	}
	
	std::vector<Word*> read_line(std::string line){ //returns a vector of Word Objects (only Command and Connector objects)
	    std::vector<Word*> word_list;
	    CURRENT_WORD_LIST = &word_list; //this pointer is to reference the word_list outside of read_line
	    line.append(" "); //append space character so the last token runs.
	    
	    //take all of the characters in our strategy and put them into a char array as sperators characters for our tokenizer
	    std::string strategyChars = " ";
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){
	        if (strategyChars.find((*strit)->get_character()) == std::string::npos){
		    strategyChars.append((*strit)->get_character());
		}
	    }	
            boost::char_separator<char> delim("", strategyChars.c_str()); //if the interpreter encounters any of the characters in the 2nd arg, it tokenizes the character individually
            boost::tokenizer<boost::char_separator<char>> tokens(line,delim); //TOKENINZE USER INPUT
		
	    //prepatory vars
	    std::string PREVIOUS_TOKEN;
	    Command* command = new Command();
	    Command* command_ptr = command; //this is a pointer to the actual command object. we append this instead of the command object for decorator functionality.

	    //INTERPRET TOKENS
	    for(auto it = tokens.begin(); it != tokens.end(); it++){   
		std::string CURRENT_TOKEN = *it;
		if (this->APPEND_TO_PREVIOUS_TOKEN == true){ //add previous token if pause is on.
			//std::cout << "pause = true; appending \"" << PREVIOUS_TOKEN << "\" to " << CURRENT_TOKEN << std::endl;
			CURRENT_TOKEN = PREVIOUS_TOKEN.append(CURRENT_TOKEN);
		} else {
		    //debug(1,"APPEND_TO_PREVIOUS_TOKEN is off");
		}
		//debug(0, "token: " + CURRENT_TOKEN);
		//go through various interpretation strategies
		for (auto strit = strats.begin(); strit != strats.end(); strit++){
		    if (this->INTERPRETATION_IN_PROGRESS == false){ 
			if (this->STRATEGY_IN_PROGRESS == "" || this->STRATEGY_IN_PROGRESS == (*strit)->get_character()){
		            calls = (*strit)->interpret(CURRENT_TOKEN);
			    this->makecalls();
			    if (this->INTERPRETATION_IN_PROGRESS == true){ //if inAction was set to true from the current strategy
			        this->STRATEGY_IN_PROGRESS = (*strit)->get_character();
			    } 
			} else {
			    //debug(1, "STRATEGY_IN_PROGRESS != 0 or same strat");
			}
		    }
		}
		//make interpreter calls according to interpretations
		for (auto strit = strats.begin(); strit != strats.end(); strit++){
		    if (this->HALT_INTERPRETATION == false){
			if ((*strit)->get_status() == true){
			    //debug(2,(*strit)->get_character() + " status: " +(*strit)->get_status());
			    //debug(2, (*strit)->get_character() + " is executing.");
			    calls  = (*strit)->execute();
			    this->makecalls();
			}
	   	    }
		}
		//override current token if TOKEN_OVERRIDE isn't empty.
		if (TOKEN_OVERRIDE != ""){
		    CURRENT_TOKEN = TOKEN_OVERRIDE;
		    this->TOKEN_OVERRIDE = ""; //reset override
		}

		//decorate command by changing the pointer
		if (COMMAND_DECORATOR.size() != 0){
		    while (COMMAND_DECORATOR.size() > 0){
			Command* dummy_ptr = command_ptr; //points to what the command_ptr is pointing to
		    	Command* decorator_ptr; //points to the new instantiated decorator that points to the dummy ptr.
			std::string decorator = COMMAND_DECORATOR.at(COMMAND_DECORATOR.size()-1);
			
			//these are kinda hardcoded atm but can easily be fixed.
		        if (decorator == "SPAN"){
			    decorator_ptr = new Span(dummy_ptr);
		        } else if (decorator == "TEST"){
		            decorator_ptr = new Test(dummy_ptr);
		        } else if (decorator == "O_SOURCE"){
			    decorator_ptr = new O_Source(dummy_ptr);
			} else if (decorator == "O_TARGET"){
			    decorator_ptr = new O_Target(dummy_ptr);
			} else if (decorator == "I_SOURCE"){
			    decorator_ptr = new I_Source(dummy_ptr); 
			} else if (decorator == "I_TARGET"){
			    decorator_ptr = new I_Target(dummy_ptr);
			} else if (decorator == "PIPE_START"){
			    decorator_ptr = new Pipe_Start(dummy_ptr);
			} else if (decorator == "PIPE_PIECE"){
			    decorator_ptr = new Pipe_Piece(dummy_ptr);
			}
			
			command_ptr = decorator_ptr; //point the command_ptr to the new decorator_ptr.
			COMMAND_DECORATOR.pop_back();
		    }
		}

		//IF NOT IGNORING TOKENS
		if (this->HALT_INTERPRETATION == false){
		    if (this->END_OF_COMMAND == false){
			if (this->APPEND_TO_PREVIOUS_TOKEN == false && this->INTERPRETATION_IN_PROGRESS == false){
		    	    if (CURRENT_TOKEN != " "){ //ignore space tokens
		    	        if (wordCount == 0){ //APPENED A EXEC
			            command->add_word(new Exec(CURRENT_TOKEN));
				    //debug(2,"\"" + CURRENT_TOKEN + "\" was appended as an EXEC. Wordcount: " + to_string(this->wordCount));
			        } else { //APPEND AN ARGa
				    command->add_word(new Arg(CURRENT_TOKEN));
				    //debug(2,"\"" + CURRENT_TOKEN + "\" was appended as an ARG. Wordcount: " + to_string(this->wordCount));
				    }
			        this->wordCount++;    
			        //cout << "New Wordcount: " << this->wordCount << endl;
				PREVIOUS_TOKEN = CURRENT_TOKEN;
			    } else {
			    	//`std::cout << "pause is false but space detected" << std::endl;
			    }
			} else {
			    PREVIOUS_TOKEN = CURRENT_TOKEN;
			    //debug(2, "APPEND_TO_PREVIOUS_TOKEN is true. new prev_token is \"" + prev_token + "\"");
			}
		    } 
		    //RESET TOKENS IF NECESSARY
		    else {
			//append the built command we have made and instantiate a new command object to append to
			word_list.push_back(command_ptr); //push back the pointer of our command
			command = new Command(); //create a new command
			command_ptr = command; //repoint to the new command
			
			//append our connector
			word_list.push_back(new Connector(CURRENT_TOKEN));
			//debug(2, "\"" + CURRENT_TOKEN + "\" was appended as a CONNECTOR");
			
			//reset
		        PREVIOUS_TOKEN = "";
		        this->END_OF_COMMAND = false;
		        this->wordCount = 0;
		    }
		}

		this->INTERPRETATION_IN_PROGRESS = false;
            }
	    //EOL
	    word_list.push_back(command_ptr); //push the last command in
	    
	    //check for invalid inputs (ex: incomplete spans)
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){
	    	if ((*strit)->get_status() == true){
		    cout << "ERROR: Invalid Input. (Maybe an incomplete span?)" << endl;
		    word_list.clear();
		    this->InterpreterCall("14");
		}
		(*strit)->reset_vars();
		(*strit)->set_status(false);
	    }    
	
	    this->wordCount = 0; //reset wordCount since we are at the end of the line
	    return word_list;
	}

	int execute(std::vector<Word*> word_list){ //executes commands in the passed in vector of words objects.
	    //EXECUTE COMMANDS
	    bool IGNORE_NEXT_WORD = false; //if true, ignores the next command
	    bool AVOID_EXECUTE = false; //if true, avoids the execution of the command	    

	
	    //std::cout << "here is our wordlist: " << endl;
	    /*for (unsigned i = 0; i < word_list.size(); i++){
	    std::cout << "word " << i << ": '" << word_list.at(i)->get_word() << "'" << std::endl;
	    }*/
	    for (auto it = word_list.begin(); it != word_list.end(); it++){
		//cout << "WORD: '" << (*it)->get_word() << "'" << endl;
		for (auto strit = strats.begin(); strit != strats.end(); strit++){ //check for conditional running for our strategies
		    if ((*strit)->get_character() == (*it)->get_word()){ //if our word matches any of our interpretation strategies (connectors)
		    	if ((*strit)->previous_must_be() != -1){ //-1 means the strategy doesn't need any previous output. by using !=, the strategy requires a certain output
			    if (PREVIOUS_EXECUTE != (*strit)->previous_must_be()){
				IGNORE_NEXT_WORD = true;
			    }
		
			}
			AVOID_EXECUTE = true;
		    }
		}
		
		//custom one-word commands

		if ((*it)->get_word() == "exit"){ //exit command
		    return 2; //2 is now our new exit command
		} else if ((*it)->get_word() == ""){ //blank command (if plugging in only a span
		    AVOID_EXECUTE = true;
		    IGNORE_NEXT_WORD = false; //acknowledge next word
		} else if ((*it)->get_word() == "(True)"){ //logic override
		    PREVIOUS_EXECUTE = 0;
		    AVOID_EXECUTE = true;
		} else if ((*it)->get_word() == "(False)"){ //logic override
		    PREVIOUS_EXECUTE = 1;
		    AVOID_EXECUTE = true;
		}

		//custom multi-word commands
		
		//cout << "calling get_sequence()" << endl;
		vector<Word*> it_s;
		if (AVOID_EXECUTE == false){ //for some reason it gives a seg fault if you pass a connector on the below line
			it_s = dynamic_cast<Command*>(*it)->get_sequence(); //get the words in the command in the list.
		}
		//cout << "successfully called" <<endl;
		if (it_s.size() > 0){
			std::string firstWord = it_s.at(0)->get_word();
			//cout << "first word: '" << firstWord << "'" <<endl;
			
			//this is kinda hardcode-y but this can be fixed

			if (firstWord == "test"){ //TEST COMMAND
			    //cout << " TEST COMMAND" << endl;
			    char flag = 'e'; //default
			    int result = 0;
			    std::string location; //location to test			    

			    //determine behavior
			    std::string secondWord = it_s.at(1)->get_word();
			    if (secondWord == "-e"){ //check if file/directory exists
				if (it_s.size() > 2){
				    location = it_s.at(2)->get_word(); //set location to third word
				} else { //if only flag was specified
				}
			    } else if (secondWord == "-f"){ //check if file/directory exists and is a FILE
			        if (it_s.size() >2){
				    location = it_s.at(2)->get_word(); //set location to third word
				    flag = 'f';
				} else { //if only flag was specified
				}
			    } else if (secondWord == "-d"){ //check if file/direectory exists and is a DIRECTORY
				if (it_s.size() > 2) {
				    location = it_s.at(2)->get_word(); //set location to third word
				    flag = 'd';
				} else { //if only flag was specified
				}
			    } else { //no flag specified
				location = secondWord; //set location to second word
			    }
			   
			    //we use boost::filesystem 's namespace so these don't need to be prefixed
			    char * location_cstr = const_cast<char*>(location.c_str());
			    path p (location_cstr); //path is a boost::filesystem object

			    //first, test if location exists
			    if (exists(p)){
			        result = 0; //return true
				//cout << "exists" <<endl;
			        //next, test only based on flags
			        if (flag == 'f'){

			    	    if (is_regular_file(p)){ //if file
				        result = 0; //kinda redundant
					//cout << "file exists" <<endl;
				    } else {
					//cout << "not a file though" << endl;
				        result = 1;
				    }

			        } else if (flag == 'd'){

			    	   if (is_directory(p)){ //if directory
				       result = 0; //kinda redundant
				       //cout << "directory exists" <<endl;
				   } else {
				       //cout << "not a directory though" << endl;
				       result = 1;
				   }

			        }
			    } else {
				//cout <<"doesn't exist" <<endl;
				result = 1;
			    }			    
			    
			    //prevent execute() call, since we dont want to use the test command in bin
			    PREVIOUS_EXECUTE = result;
			    AVOID_EXECUTE = true;
			    InterpreterCall("13 " + convertExecute(result)); //print result

			}

		}
		
		if (AVOID_EXECUTE == false){
		    //cout << "calling execute()" <<endl;
		    
		    this->InterpreterCall((*it)->execute()); //we use an interpreter call to set our PREVIOUS_EXECUTE now
		    //run the command in the register when told to. used for if the command should be ran again after the one after it
		    if (PREVIOUS_EXECUTE == 4){
			this->InterpreterCall(WORD_REGISTER->execute());
			WORD_REGISTER = nullptr; //reset register to nothing
		    }
		

	    	    if (PREVIOUS_EXECUTE == -1){
		        std::cout << "Invalid Command: \"" << (*it)->get_word() << "\"" <<  std::endl;
		    	PREVIOUS_EXECUTE = 1; //treat invalid commands as failures
		    }

		    if (PREVIOUS_EXECUTE == 3){ //3 means the command should be ran again after the command after it, so a pointer to it is saved.
			WORD_REGISTER = (*it);
			PREVIOUS_EXECUTE = 0;
		    }

		} else {
		    if (IGNORE_NEXT_WORD == true){
		    	if ( (it+1) != word_list.end()){
		        	it++; //skip this word and move to the next
		    	}
		    	IGNORE_NEXT_WORD = false;
		    }
		    AVOID_EXECUTE = false;
		}
		//std::cout << "PREVIOUS_EXECUTE: " << PREVIOUS_EXECUTE << std::endl;
	    }
	    //checking for if word_register was left hanging
	    if (WORD_REGISTER != nullptr){
	        this->InterpreterCall(WORD_REGISTER->execute());
		WORD_REGISTER = nullptr;
		//std::cout << "something was left hanging" << std::endl;
	    }
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){(*strit)->set_status(false);} //set all status of strats to false since we are done with the line
	    //cout << "This sequence returns: " << PREVIOUS_EXECUTE << endl;
	    return PREVIOUS_EXECUTE;
        };

};

#endif
