//Andrew Pirelli
//Atreyu Wittman
// Bugs:
// Each command must have no more than 2048 words
// Commands must have the format specified in the assignment specs, otherwise it will not execute properly

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <boost/tokenizer.hpp>

using namespace std;

class Terminal{
    public:
        virtual bool execute() = 0;
        //adding the bool component became neccesary for executing the connector logic
};

class Command: public Terminal { //base class for single command
    private:
        string cmd; //Assuming no more than 64 words in a command
    public:
        Command(string in1) { //turns string into components for fork, execvp and waitid
            cmd = in1;
        }
        virtual bool execute() {
                char* cmdLine[2048];
                char* whitespace = strtok(&cmd[0], " \t"); //removes whitespace
                vector<string> temps;
                while (whitespace) {
                    temps.push_back(whitespace); //pushing back without whitespace
                    whitespace = strtok(0, " \t");
                }
                for (unsigned i = 0; i < temps.size(); ++i) {
                    string& swap = temps.at(i);
                    cmdLine[i] = &swap[0];
                }
                cmdLine[temps.size()] = '\0';
                if (strcmp(cmdLine[0], "exit") == 0) {
                    exit(0);
                }
            	int child_pid = fork(); //fork test fatal for fatal errors
            	if (child_pid <= -1) {	//-1 is error
            		perror("Fork creation error");
            		return false;
            	}
            	else if (child_pid == 0) { //child process
            		int result = execvp(*cmdLine, cmdLine);
            		if (result <= -1) {
            			perror("Command line execution error");
            			return false;
            		}
            	}
            	else { //parent process
            		int childWait = waitpid(child_pid, &childWait, 0);
            		if (childWait <= -1) {
            			perror("Failed to wait for child");
            		}
            		if (childWait == 0) {
            			return true;
            		}
            		return true;
            	}
            	return false;
        }
};

class Command_And: public Terminal { // for && connector
    private:
        Terminal* Phrase1; //pointer to left command/composition
        Terminal* Phrase2; //pointer to right command
    public:
        Command_And(Terminal* in1, Terminal* in2) {
            Phrase1 = in1;
            Phrase2 = in2;
        }
        virtual bool execute() { //Logical AND
            bool temp1 = Phrase1->execute();
            bool temp2 = false;
            if (temp1) {
                temp2 = Phrase2->execute();
            }
            return (temp1 && temp2);
        }
};

class Command_Xor: public Terminal { //for || connector
    private:
        Terminal* Phrase1;
        Terminal* Phrase2;
    public:
        Command_Xor(Terminal* in1, Terminal* in2) {
            Phrase1 = in1;
            Phrase2 = in2;
        }
        virtual bool execute() { //Logical XOR
            bool temp1 = Phrase1->execute();
            bool temp2 = false;
            if(!temp1){
                temp2 = Phrase2->execute();
            }
            return(temp1 || temp2);
        }
};

class Command_List: public Terminal { // for ; connector
    private:
        Terminal* Phrase1; 
        Terminal* Phrase2; 
    public:
        Command_List(Terminal* in1, Terminal* in2) {
            Phrase1 = in1;
            Phrase2 = in2;
        }
        virtual bool execute() { //returns last element bool value
            Phrase1->execute();
            return Phrase2->execute();
        }
};

class Command_Line: public Terminal { //The class containing the full structure of command compositions
    private:
        Terminal* Top;
        const char* convert_to_arr(string in) {
            
        }
    public:
        Command_Line(string in1) {
            if (in1.empty()) {
                Top = new Command(in1);
                //Just to avoid out of bounds error
            }
            else {
                //neccesary temp variable for the constructor to create the structure
                unsigned j = 1;
                unsigned space = 0;
                Terminal* curr = 0;
                Terminal* temp = 0;
                int prevCmd = 0; //1 is &&, 2 is ||, 3 is ;
                string cmd;
                for (unsigned i = 0; i < in1.size(); ) { //loop for traversing string
                    if ((in1.at(i) == '&') && (in1.at(j) == '&')) { //Case 1
                        if (!curr) { //base case (first command in command line will not be a composition)
                            cmd.erase(cmd.size() - 1);
                            curr = new Command(cmd);
                            Top = curr;
                            cmd.clear();
                            prevCmd = 1;
                        }
                        else { //past base case
                            cmd.erase(cmd.size() - 1);
                            temp = new Command(cmd);
                            if (prevCmd == 1) {
                                Top = new Command_And(curr, temp);
                            }
                            if (prevCmd == 2) {
                                Top = new Command_Xor(curr, temp);
                            }
                            if (prevCmd == 3) {
                                Top = new Command_List(curr, temp);
                            }
                            curr = Top;
                            prevCmd = 1;
                            cmd.clear();
                        }
                        i = i + 2;
                        j = j + 2;
                    }
                    else if ((in1.at(i) == '|') && (in1.at(j) == '|')) { //Case 2
                        if (!curr) { //base case (first command will not be a composition)
                            cmd.erase(cmd.size() - 1);
                            curr = new Command(cmd);
                            Top = curr;
                            cmd.clear();
                            prevCmd = 2;
                            space = 1;
                        }
                        else { //past base case
                            cmd.erase(cmd.size() - 1);
                            temp = new Command(cmd);
                            if (prevCmd == 1) {
                                Top = new Command_And(curr, temp);
                            }
                            if (prevCmd == 2) {
                                Top = new Command_Xor(curr, temp);
                            }
                            if (prevCmd == 3) {
                                Top = new Command_List(curr, temp);
                            }
                            curr = Top;
                            prevCmd = 2;
                            cmd.clear();
                            space = 1;
                        }
                        i = i + 2;
                        j = j + 2;
                    }
                    else if (in1.at(i) == ';') { //Case 3
                        if (!curr) { //base case (first command will not be a composition)
                            curr = new Command(cmd);
                            Top = curr;
                            cmd.clear();
                            prevCmd = 3;
                            space = 1;
                        }
                        else { //past base 
                            temp = new Command(cmd);
                            if (prevCmd == 1) {
                                Top = new Command_And(curr, temp);
                            }
                            if (prevCmd == 2) {
                                Top = new Command_Xor(curr, temp);
                            }
                            if (prevCmd == 3) {
                                Top = new Command_List(curr, temp);
                            }
                            curr = Top;
                            prevCmd = 3;
                            cmd.clear();
                            space = 1;
                        }
                        ++i;
                        ++j;
                    }
                    else if (in1.at(i) == '#') { //Case 4
                        if (!curr) { //base case (first command will not be a composition)
                            cmd.erase(cmd.size() - 1);
                            curr = new Command(cmd);
                            Top = curr;
                            i = in1.size();
                            cmd.clear();
                            prevCmd = 4;
                            space = 1;
                        }
                        else { //past base case
                            cmd.erase(cmd.size() - 1);
                            temp = new Command(cmd);
                            if (prevCmd == 1) {
                                Top = new Command_And(curr, temp);
                            }
                            if (prevCmd == 2) {
                                Top = new Command_Xor(curr, temp);
                            }
                            if (prevCmd == 3) {
                                Top = new Command_List(curr, temp);
                            }
                            curr = Top;
                            i = in1.size();
                            cmd.clear();
                            prevCmd = 4;
                            space = 1;
                        }
                    }
                    else {
                        if (space == 1) {
                            //string for created commands
                        }
                        else {
                            cmd.push_back(in1.at(i));
                        }
                        space = 0;
                        ++i;
                        ++j;
                    }
                }
                if (prevCmd == 0) { //Case 0, command line only contatins a single command
                    curr = new Command(cmd);
                    Top = curr;
                }
                if (prevCmd == 1) { //Case 1, last connector was &&
                    temp = new Command(cmd);
                    Top = new Command_And(curr, temp);
                }
                if (prevCmd == 2) { //Case 2, last connector was ||
                    temp = new Command(cmd);
                    Top = new Command_Xor(curr, temp);
                }
                if (prevCmd == 3) { //Case 3, last connector was ;
                    temp = new Command(cmd);
                    Top = new Command_List(curr, temp);
                }
                //if it was case 4 in the above loop, this part is skipped as everything after # is ignored
            }
        }
        bool execute() {
            return Top->execute();
        }
};

int main() {
    string input;
    Terminal* Test;
    while (true) {
        cout << "$ ";
        getline(cin, input);
        if (!input.empty()) {
            Test = new Command_Line(input);
            Test->execute();
        }
    }
    return 0;
}