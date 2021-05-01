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


string showToken = "0";
map<string, string> variables; //map<name,value>
string prompt = "shell > ";
vector<string> processes; //background processes
int quitVar = -1; //will be changed when called to exit the shell

vector<string> createToken(string str)
{
  istringstream ISS(str);
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

//returns true if there's an error relating to num of tokens
bool errorCheck(vector<string>& tokens, unsigned int wanted)
{
    if (tokens.size() > wanted)
    {
        if (tokens[wanted] != "#")
        {
            fprintf(stderr, "Too many arguements for %s\n", tokens[0].c_str());
            return true;
        }
    }
    if (tokens.size() < wanted)
    {
        fprintf(stderr, "Too few arguements for %s\n", tokens[0].c_str());
        return true;
    }
    for (unsigned int i = 1; i < wanted; i++)
    {
        if (tokens[i] == "#")
        {
            fprintf(stderr, "Too few arguements for %s\n", tokens[0].c_str());
            return true;
        }
    }
    return false;
}


void done(vector<string> &tokens)
{
    //change quitVar to 0 on no value
    if (tokens.size() == 1)
    {
        if (errorCheck(tokens, 1))
        {
            return;
            quitVar = 0;
        }
        else
        {
            if (errorCheck(tokens, 2))
                return;
            string val = tokens[1];
            for (unsigned int i = 0; i < val.length(); i++)
            {
                if (isdigit(val[i]))
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
}

void showProcesses(vector<string> &tokens)
{
  if (errorCheck(tokens, 1))
      return;
  for (auto &str: processes)
  {
    printf("PROC: %s", str.c_str());
  }
}

//choice(0) = run, choice(1) = fly, choice(2) = toVar
void doRun(vector<string> &tokens, int choice)
{
  if (choice == 0)
  {
    if (tokens.size() < 2)
    {
      errorCheck(tokens, 2);
      return;
    }
  }
  for (unsigned int i = 0; i < tokens.size(); i++)
  {
    if (tokens[i].substr(0, 1) == "^")
    {
      tokens[i] = variables.find(tokens[i].substr(1))->second;
    }
  }
  
  char* arg[tokens.size()];
  string path = variables.find("PATH")->second;
  
  int sizeDif = 1;
  if (choice == 3)
  {
    sizeDif = 2;
  }
  for(unsigned int i = sizeDif; i < tokens.size(); i++)
  {
    arg[i-sizeDif] = const_cast<char*>(tokens[i].c_str());
  }
  arg[tokens.size() - sizeDif] = NULL;
  
  int pid = fork();
  int fork1 = pid;
  
  if (pid == 0 && choice == 1)
  {
    pid = fork();
  }
  
  if (pid == 0)
  {
    if (choice == 3)
    {
      variables[tokens[1]] = execvp(arg[0], arg);
    }
    else
    {
      execvp(arg[0], arg);
    }
    fprintf(stderr, "Execv did not work \n");
		exit(1);
  }
  
  if (pid != fork1 && choice == 1)
  {
    waitpid(pid, NULL, 0); 
		string argsString = arg[0];
		for (unsigned int i = 1; i < (tokens.size() - 1); i++) 
    {
			argsString = argsString + " " + arg[i];
		}
        exit(1);
  }
  
  if(choice == 0) {
		waitpid(pid, NULL, 0);
	}
}
      
void doFly(vector<string> &tokens)
{
  doRun(tokens, 1);
}
      
void dotoVar(vector<string> &tokens)
{
  doRun(tokens, 3);
}
      
void setVar(vector<string> &tokens)
{
  if (errorCheck(tokens, 3))
      return;
  string var = tokens[1];
      
  //if first character in var isn't a letter
  for (unsigned int i = 1; i < variables.size(); i++) 
  {
		if (isalpha(var[i]) || isdigit(var[i])) 
    {
		}
		else
    {
			fprintf(stderr,"Variable names must only consist of letters and numbers.\n");
			return;
		}
  }
  
  string val = tokens[2];
	if (var == "ShowTokens") {
		showToken = val;
	}
	else
		variables[var] = val;
}
      
int setdir(vector<string> &tokens)
{
  if (errorCheck(tokens, 2))
  {
		return -1;
	}
	int code = chdir(tokens[1].c_str());
	if (code != 0) 
  {
		fprintf(stderr, "%s: No such directory\n", tokens[1].c_str());
		return -1;
	}
	return 0;
}
      

      
void changePrompt(vector<string> &tokens)
{
	if (errorCheck(tokens, 2))
  {
		return;
	}
	prompt = tokens[1];
}

void execute(vector<string>& tokens) //sets if/else if/else cases for calling all available commands
{
    if (tokens.size() == 0)
        return;

    string cmd = tokens[0];

    if (cmd == "setvar")
    {
        setVar(tokens);
    }
    else if (cmd == "setprompt")
    {
        changePrompt(tokens);
    }
    else if (cmd == "setdir")
    {
        setdir(tokens);
    }
    else if (cmd == "showprocs")
    {
        showProcesses(tokens);
    }
    else if (cmd == "#")
    {
        //comment function, doesn't need anything
    }
    else if (cmd == "done")
    {
        done(tokens);
    }
    else if (cmd == "run")
    {
        doRun(tokens, 0);
    }
    else if (cmd == "fly")
    {
        doFly(tokens);
    }
    else if (cmd == "tovar")
    {
        dotoVar(tokens);
    }
    else
    {
        fprintf(stderr, "%s is not a valid command.\n", cmd.c_str());
    }
}

      
int main()
{
  variables["PATH"] = "/bin:/usr/bin";
	string cmd;
	vector<string> parameters;
	
  while (true) 
  {
		cout << prompt;
		getline(cin, cmd);
		vector<string> tokens = createToken(cmd);
		execute(tokens);
		if (quitVar > -1)
    {
			return quitVar;
		}
		if (showToken == "1")
    {
			for (auto& str : tokens)
      {
				cout << str << endl;
      }
    }
	}
  
  return -1; //shouldn't be called, will produce an error if it is
}

