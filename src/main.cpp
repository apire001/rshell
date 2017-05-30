//Andrew Pirelli
//Atreyu Wittman
// Bugs:
// Each command must have no more than 2048 words
// Commands must have the format specified in the assignment specs, otherwise it will not execute properly

#include <sys/types.h>
#include <sys/stat.h>
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
        int test;
        bool testflag;
        bool leftflag;
        bool rightflag;
        bool flage;
        bool flagf;
        bool flagd;
    public:
        Command(string in1) { //turns string into components for fork, execvp, waitid and test
            cmd = in1;
            test = 0;
            testflag = false;
            leftflag = false;
            rightflag = false;
            flage = false;
            flagf = false;
            flagd = false;
            if(cmd.find("-e") != string::npos) {
                flage = true;
            }
            else if(cmd.find("-f") != string::npos) {
                flagf = true;
            }
            else if(cmd.find("-d") != string::npos) {
                flagd = true;
            }
            size_t testpos = cmd.find("test");
            if(testpos != string::npos && (testpos == 1 || testpos == 0)) {
                cmd.erase(testpos, 4);
                test = 1;
                testflag = true;
            }
            size_t leftbpos = cmd.find("[");
            if(leftbpos != string::npos && (leftbpos == 1 || leftbpos == 0)) {
                cmd.erase(leftbpos, 1);
                test = 1;
                leftflag == true;
            }
            size_t rightbpos = cmd.find("]");
            if(rightbpos != string::npos && (rightbpos == cmd.size() - 1 || rightbpos == cmd.size() - 2)) {
                rightflag == true;
                cmd.erase(rightbpos, 1);
                test = 1;
            }
            if(test == 1){
                size_t flagloc;
                if(flage) {
                    flagloc = cmd.find("-e");
                    if(!(flagloc == string::npos)){
                        cmd.erase(flagloc, 2);
                    }
                }
                else if(flagd) {
                    flagloc = cmd.find("-f");
                    cmd.erase(flagloc, 2);
                }
                else if(flagf) {
                    flagloc = cmd.find("-d");
                    cmd.erase(flagloc, 2);
                }
            }
        }
        virtual bool execute() {
            if(test) { //run as test command
                if ((flage && flagf) || (flage && flagd) || (flagf && flagd)){
                    perror("Error: More than 1 flag");
                    return false;
                }
                if(!testflag && !leftflag && rightflag){
                    perror("Missing left bracket for test");
                }
                if(!testflag && leftflag && !rightflag){
                    perror("Missing right bracket for test");
                }
                if(testflag && !leftflag && rightflag){
                    perror("Test and right bracket detected");
                }
                if(testflag && leftflag && !rightflag){
                    perror("Test and left bracket detected");
                }
                if(testflag && leftflag && rightflag){
                    perror("Test and brackets detected");
                }
                char* testLine = new char[cmd.size()];
                strcpy(testLine, cmd.c_str());
                testLine = strtok(&cmd[0], " \t");
                struct stat testStruct;
                if(flage) {
                    if(stat(testLine, &testStruct) == 0) {
                        cout << "(True)\n";
                        return true;
                    }
                    else{
                        cout << "(False)\n";
                        return false;
                    }
                }
                if(flagf){
                    if (stat(testLine, &testStruct) == 0 && S_ISREG(testStruct.st_mode)) {
                        cout << "(True)\n";
                        return true;
                    }
                    else{
                        cout << "(False)\n";
                        return false;
                    }
                }
                if(flagd) {
                    if(stat(testLine, &testStruct) == 0 && S_ISDIR(testStruct.st_mode)) {
                        cout << "(True)\n";
                        return true;
                    }
                    else{
                        cout << "(False)\n";
                        return false;
                    }
                }
            }
            else {  //execute command
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
                unsigned pCount = 0;
                unsigned pOccur = 0;
                Terminal* curr = 0;
                Terminal* temp = 0;
                int prevCmd = 0; //1 is &&, 2 is ||, 3 is ;
                string cmd;
                for (unsigned i = 0; i < in1.size(); ) { //loop for traversing string
                    if(in1.at(i) == '(') { //Handle parentheses
                        ++pCount; //used for finding the corresponding end parentheses
                        ++pOccur; //parentheses flag
                        while(pCount != 0 && i < in1.size()) {
                            ++i;
                            ++j;
                            if(in1.at(i) == '(') {
                                ++pCount;
                            }
                            if(in1.at(i) == ')') {
                                --pCount;
                            }
                            if(pCount != 0) {
                                cmd.push_back(in1.at(i)); //stores everything in corresponding parentheses
                            }
                        }
                    }
                    if(j < in1.size()){
                        if ((in1.at(i) == '&') && (in1.at(j) == '&')) { //Case 1
                            if (!curr) { //base case (first command in command line will not be a composition)
                                if(pOccur == 1){
                                    curr = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    curr = new Command(cmd);
                                }
                                Top = curr;
                                cmd.clear();
                                prevCmd = 1;
                            }
                            else { //past base case
                                if(pOccur == 1){
                                    temp = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    temp = new Command(cmd);
                                }
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
                                if(pOccur == 1){
                                    curr = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    curr = new Command(cmd);
                                }
                                Top = curr;
                                cmd.clear();
                                prevCmd = 2;
                            }
                            else { //past base case
                                if(pOccur == 1){
                                    temp = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    temp = new Command(cmd);
                                }
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
                            }
                            i = i + 2;
                            j = j + 2;
                        }
                        else if (in1.at(i) == ';') { //Case 3
                            if (!curr) { //base case (first command will not be a composition)
                                if(pOccur == 1){
                                    curr = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    curr = new Command(cmd);
                                }
                                Top = curr;
                                cmd.clear();
                                prevCmd = 3;
                            }
                            else { //past base 
                                if(pOccur == 1){
                                    temp = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    temp = new Command(cmd);
                                }
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
                            }
                            ++i;
                            ++j;
                        }
                        else if (in1.at(i) == '#') { //Case 4
                            if (!curr) { //base case (first command will not be a composition)
                                if(pOccur == 1){
                                    curr = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    curr = new Command(cmd);
                                }
                                Top = curr;
                                i = in1.size();
                                cmd.clear();
                                prevCmd = 4;
                            }
                            else { //past base case
                                if(pOccur == 1){
                                    temp = new Command_Line(cmd);
                                    --pOccur;
                                }
                                else{
                                    temp = new Command(cmd);
                                }
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
                            }
                        }
                    else {
                        if(pOccur == 0 && i < in1.size()){
                            cmd.push_back(in1.at(i));
                        }
                        ++i;
                        ++j;
                    }
                    }
                }
                if (prevCmd == 0) { //Case 0, command line only contatins a single command
                    if(pOccur == 1){
                        curr = new Command_Line(cmd);
                        --pOccur;
                    }
                    else{
                        curr = new Command(cmd);
                    }
                    Top = curr;
                }
                if (prevCmd == 1) { //Case 1, last connector was &&
                    if(pOccur == 1){
                        temp = new Command_Line(cmd);
                        --pOccur;
                    }
                    else{
                        temp = new Command(cmd);
                    }
                    Top = new Command_And(curr, temp);
                }
                if (prevCmd == 2) { //Case 2, last connector was ||
                    if(pOccur == 1){
                        temp = new Command_Line(cmd);
                        --pOccur;
                    }
                    else{
                        temp = new Command(cmd);
                    }
                    Top = new Command_Xor(curr, temp);
                }
                if (prevCmd == 3) { //Case 3, last connector was ;
                    if(pOccur == 1){
                        temp = new Command_Line(cmd);
                        --pOccur;
                    }
                    else{
                        temp = new Command(cmd);
                    }
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