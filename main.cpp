//-----------------------------------------------
// Authors: Noah Perry, Evan Courtwright
// Assignment: Project 4: Create a Simple Shell
//-----------------------------------------------

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void execute(vector<string> tokens);
vector<string> createToken(string str);
void changePrompt(vector<string> tokens);
void setVar(vector<string> tokens);
void doRun(vector<string> tokens, int choice);
void doFly(vector<string> tokens);
void dotoVar(vector<string> tokens);
bool errorCheck(vector<string> tokens, int args);
void showProcesses(vector<string> tokens);
void done(vector<string> tokens);
int setdir(vector<string> tokens);

string showToken = "0";
map<string, string> var; //map<name,value>
string prompt = "shell > ";
vector<string> processes; //background processes
int quitVar = -1; //will be changed when called to exit the shell

vector<string> createToken(string str)
{
  isstringstream ISS(str);
  vector<string> tokens;
  string s;
  
  while (ISS >> quoted(s))
  {
    if (s[0] == '#')
      break;
    tokens.push_back(s);
  }
  
  return tokens;
}

void execute(vector<string> tokens) //sets if/else if/else cases for calling all available commands
{
  if (tokens.size() == 0)
    return;
  
  string cmd = tokens[0];
  
  if (cmd = "setvar")
  {
    setVar(tokens);
  }
  else if (cmd = "setprompt")
  {
    changePrompt(tokens);
  }
  else if (cmd = "setdir")
  {
    setdir(tokens);
  }
  else if (cmd = "showprocs")
  {
    showProcesses(tokens);
  }
  else if (cmd = "#")
  {
    //comment function, doesn't need anything
  }
  else if (cmd = "done")
  {
    done(tokens);
  }
  else if (cmd = "run")
  {
    doRun(tokens, 0);
  }
  else if (cmd = "fly")
  {
    doFly(tokens);
  }
  else if (cmd = "tovar")
  {
    dotoVar(tokens);
  }
  else
  {
    fprintf(stderr, "%s is not a valid command.\n", cmd.c_str())
  }
}

void done(vector<string> tokens)
{
  //change quitVar to 0 on no value
  if (tokens.size() == 1)
  {
    if (errorCheck(tokens, 1)
        return;
    quitVar = 0;
  }
  else
  {
    if (errorCheck(tokens, 2)
        return;
    string val = tokens[1];
    for(int i = 0; i < val.length(); i++)
    {
        if (isdigit(value[i]))
        {
        }
        else
        {
          fprintf(stderr, "Values after done must consist only of numbers.\n");
          return;
        }
    }
    quitVar = stoi(val);
  }
}

void showProcesses(vector<string> tokens)
{
  if (errorCheck(tokens, 1)
      return;
  for (auto str: procs)
  {
    printf("PROC: %s", str.c_str());
  }
}
 
        
int main()
{
  
  return -1; //shouldn't be called, will produce an error if it is
}

