#include <iostream> 
#include <cstring>
#include <string>

int main(){

    //std::string command;
    char str[256]; 
    std::cout << "User Input: ";
    std::cin.getline(str, 256);
    char* token = std::strtok(str, " ");
    
    int num_or = 0;
    int num_and = 0;
    int word = 0;
     
    while (token != NULL){
        if(token == "||"){
            num_or++;
        } else if (token == "&&") {
            num_and++;
        } else {
            word++;
        }
        std::cout << token << std::endl;
        token = std::strtok(NULL, " ");
    }
    std::cout << "Number of ||: " << num_or << std::endl;
    std::cout << "Number of &&: " << num_and << std::endl;
    std::cout << "Number of execs/args: " << word << std::endl;

    return 0;

}
