#include "../header/Interpreter.hpp"
#include "../header/Word.hpp"

#include <iostream>
#include <string>
#include <vector>

std::string get_input(){
    std::string line;
    cout << "$ ";
    getline(std::cin,line);
    return line;
};

int main() {
    
    //std::cout << "RSHELL" << std::endl;
    Interpreter* shell = new Interpreter();
    
    while (shell->execute(shell->read_line(get_input())) != 2) {;} //2 is our new exit command
    return 0;
}
