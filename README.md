# Project Information
Fall 2019
Authors: Abraham Park, Krischin Layon

# Introduction
RShell is a command shell written in C++ that is able to execute commands read from standard input and write the results to standard out. User input is read by an interpreter and parsed through to run any executable commands along with any optional arguments. The interpreter is able to recognize connectors and can separate multiple commands written at once. An executable only runs when the executable program is in the PATH environment variable. 

The shell utilizes the composite design pattern. The interpreter class handles user input with the Word base class handling all strings of the input. A Command subclass reads in an executable object to be run along with any optional agruments objects. A connector subclass recognizes any connectors and is able to separate multiple commands. 

# Diagram
[OMT_DIAGRAM]: images/diagram3.jpg "OMT Diagram"

![alt-text][OMT_DIAGRAM]

# Classes

### Interpreter
Handles all user input. Reads an inputted string and runs system executables based off of the formatting of the input. Implemented using the Boost C++ Library. (See below)

``` $ [command] [arguments for command, seperated by whitespace] ```

If the inputted string contains a '#', then it will ignore any words after the '#', treating them as comments.
You can chain additional commands by either ending the last input word with a ";" and inputing a new command, or by inputting an additional connector, (&& or ||)
The interpreter will loop input and output until the user types in 'exit'
### How it works
Specifically, the interpretor 'divides' the string into 'word' objects by whitespace.
Every new user command interpreted by the interpreter is appended to the interpreter's sequence vector as a Command object with an Exec object. 

``` Interpreter Sequence: [command0] ```

``` command0 Sequence: [exec0] ```

If the user specifies additional arguments in the string, they will be interpreted as Arg objects and be appended to the arguments vector of the last Command object.

``` Interpreter Sequence: [command0] ```

``` command0 Sqeuence: [exec0, arg00, arg01] ```

If a new command is added to the user input via a connector (;, &&, ||), the interpreter will append a Connector object and then a new Command object with an Exec object. It will also append additional arguments if specified.

``` Interpreter Seqeunce: [command0, connector0, command1, connector1, command2] ```

``` command0 Sequence: [exec0, arg00, arg01] ```

``` command1 Sequence: [exec1, arg10, arg11, arg12] ```

``` command2 Sequence: [exec2] ```

This interpretation process continues until the interpreter reaches the end of the user string.

Once the interpreter reaches the end of the user string, it will execute every command in it's sequence vector. After a command is executed, it's respective Command is popped from the vector.
This continues until the sequence vector is empty. Afterwards, the interpreter will redo the input process until it receieves an 'exit' command from the user.

### Word
Every input in the user string is interpreted as a type of Word object, each Word object has a 'value' string that contains its text value directly from the string.

### Command
Commands are composed of one Exec object and a user-defined amount of  Arg objects and serve as a a "full-length" command inputted by the user.
When a command's execute() is called, it runs the application in the /bin folder with the same name as the value of its 'exec' object.
If the application can't be found, the interpreter will throw an error and attempt to run the next command in it's sequence.

### Exec
Exec objects are word objects with their 'value' string representing the application that the user wants to run. 

### Arg
Arg objects are word objects with their 'value' string representing an argument that the user wants to pass onto their respective application.

### Connector
Connector objects are appended into the interpretor's sequence as 'seperator' objects between the first command and the next command.
When the interpretor reaches a connector in the execution sequence, it simply pops it.

# Prototypes/Research
The prototype code of this project is found within the prototype directory. There are two files: a prototype.c file where syscalls are tested to see how they function; a parser.cpp file where a prototype parser is tested to be used in this project. 

The syscalls will be used to run any executable commands in the background while the interactive shell stays active. The parser function will use the tokenizer() function from the Boost C++ library. The parserwill detect the user input for any commands and arguments along with any possible connectors and comments that could be inputted at once. 


# Development and Testing Roadmap
1. - [x] Create Word base class
1. - [x] Create Executable subclass 
1. - [x] Create Agrument subclass 
1. - [x] Create Connector subclass 
1. - [x] Create Interpreter class
1. - [x] Create Parser 
1. - [x] Create unit tests 
