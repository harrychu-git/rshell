#include <iostream>
#include <ctype.h>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

vector <string> tokenizer(char s[], string connector)
{
    const char* c2=connector.c_str();
    char* c = strtok(s, c2); //find first command
    vector <string> v;
    while(c != '\0')
    {
        v.push_back(c);
        c = strtok('\0', c2); //find rest of commands
    }
    return v; //returns a vector of strings separated by spaces
}

void noCommentZone(string &comments)
{
    if (comments.find('#')==string::npos||comments.find('#')==0)
    {} //do nothing
    else
        comments=comments.substr(0,comments.find('#'));
}

void execute(vector<string> commandlist)
{
    int sz=commandlist.size();
    char** argument = new char*[sz+1];
    argument[sz] = '\0';
    for(unsigned i=0; i<sz; i++)
    {
        argument[i] = new char[commandlist.at(i).size()];
        strcpy(argument[i], commandlist.at(i).c_str());
    }
    if(execvp(argument[0], argument)==-1){
        delete[] argument;
        perror("execvp");
        exit(1);
    }
}

int main()
{
    char hostname[128];
    gethostname(hostname,sizeof hostname);
    /*char myString[] = "ls &&ls ;ls; ls";
    vector<string> tokens;
    tokens=tokenizer(myString);
    for (int i=0; i<tokens.size(); i++)
    {
        cout << tokens.at(i) << endl;
    }*/
    while(1)
    {
        cont:
        string cmdLine;
        cout << getlogin() << "@" << hostname << "$ ";
        getline(cin, cmdLine);
        noCommentZone(cmdLine);
        if(cmdLine.at(0)=='#')//fix bug if everything is a comment
        {
            goto cont;
        }
        char *instring=new char[cmdLine.size()+1]; //requires 1 index for '\0'
        strcpy(instring, cmdLine.c_str());
        //printf("%s\n", instring);//test if cstring matches input
        //the vector that holds the commands to be converted and executed
        vector<string> commandlist;
        string curConnector="";
        
        
        //determine which connector is being used in this command
        if(cmdLine.find(";")!=string::npos)
        {
            commandlist=tokenizer(instring, ";");
            curConnector=";";
        }
        else if(cmdLine.find("&&")!=string::npos)
        {
            commandlist=tokenizer(instring, "&&");
            curConnector="&&";
        }
        else if(cmdLine.find("||")!=string::npos)
        {
            commandlist=tokenizer(instring, "||");
            curConnector="||";
        }
        //edge case
        if(curConnector=="")
        {
            int status=0;
            commandlist=tokenizer(instring, " ");
            //check if vector has one index and contains "exit"
            if(commandlist.size()==1&&commandlist.at(0)=="exit")
            {
                delete[] instring;
                goto end;
            }
            int forktest=fork();
            if (forktest==-1)
            {
                perror("fork");
                exit(1);
            }
            else if(forktest==0)
            {
                execute(commandlist);
            }
            else
            {
                if(waitpid(-1, &status, 0)==-1)
                {
                    perror("waitpid");
                    exit(1);
                }
                else
                {
                    delete[] instring;
                    goto cont;
                }
            }
        }
        vector<vector<string> >exV;
        //fill vector with vectors of strings (eg "ls", "-a")
        for (unsigned i=0; i<commandlist.size(); i++)
        {
            char* c=new char[commandlist.at(i).size()+1];
            strcpy(c,commandlist.at(i).c_str());
            exV.push_back(tokenizer(c," "));
        }
        for(unsigned i=0; i< exV.size(); i++)
        {
            int status=0;
            //check if vector has one index and contains "exit"
            if(exV.at(i).size()==1&&exV.at(i).at(0)=="exit")
            {
                delete[] instring;
                goto end;
            }
            int forktest=fork();
            if (forktest==-1)
            {
                perror("fork");
                exit(1);
            }
            else if(forktest==0)//CHILD
            {
                execute(exV.at(i));
            }
            else//PARENT
            {
                if(waitpid(-1, &status, 0)==-1)
                {
                    perror("waitpid");
                    exit(1);
                }
                else
                {
                    if(curConnector=="")
                    {
                        break;
                    }
                    else if(curConnector==";")
                    {}//do nothing and execute next command
                    else if(curConnector=="||")
                    {
                        if (status<=0)
                        {
                            break;
                        }
                    }
                    else if(curConnector=="&&")
                    {
                        if(status>0)
                        {
                            break;
                        }
                    }
                }
                
            }
        }
    
    
    
    
    
    delete[] instring;
    }
    
    
    
    
    
    end:
    return 0;
}
