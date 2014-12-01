#include <iostream>
#include <ctype.h>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fcntl.h>
#include <string>
using namespace std;


void CTRLC(int i)
{
    signal(SIGINT,SIG_IGN);
}

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

void checkIO(char** args)
{
    for(int i=0; args[i]!='\0'; i++)
    {
        int fd=0;
        if(!strcmp(args[i],"<"))
        {
            args[i]=0;
            if((fd=open(args[i+1],O_RDONLY))==-1)
                perror("open");
            if((dup2(fd,0))==-1)
                perror("dup2");
        }
        else if(!strcmp(args[i],">"))
        {
            if(!strcmp(args[i+1],">"))//found > and > next to each other, ">>"
            {
                args[i]=0;
                args[i+1]=0;
                if((fd=open(args[i+2],O_CREAT|O_WRONLY|O_APPEND,0666))==-1)
                    perror("open");
                if((dup2(fd,1))==-1)
                    perror("dup2");
                i++;
            }
            else// ">"
            {
                args[i]=0;
                if((fd=open(args[i+1],O_CREAT|O_WRONLY|O_TRUNC,0666))==-1)
                    perror("open");
                if((dup2(fd,1))==-1)
                    perror("dup2");
            }
            
        }
    }
}

void execute(vector<string> commandlist)
{
    unsigned sz=commandlist.size();
    char** argument = new char*[sz+1];
    argument[sz] = '\0';
    for(unsigned i=0; i<sz; i++)
    {
        argument[i] = new char[commandlist.at(i).size()];
        strcpy(argument[i], commandlist.at(i).c_str());
    }
    checkIO(argument);
    if(execvp(argument[0], argument)==-1){
        delete[] argument;
        perror("execvp");
        exit(1);
    }
}



void ReplaceStringInPlace(string& subject, const string& search, const string& replace) 
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) 
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void connectorFixer(string& s)
{
    ReplaceStringInPlace(s, "<", " < ");
    ReplaceStringInPlace(s, ">>", " >> ");
    ReplaceStringInPlace(s, ">", " > ");
    ReplaceStringInPlace(s, "|", " | ");
}
void pipes1(vector<string> cmdList)//passes in a command that was seprated by connectors
{
    vector<string> pt1;
    vector<string> pt2;
    unsigned i;
    for(i=0;i<cmdList.size()&&cmdList.at(i)!="|"; i++)
    {
        pt1.push_back(cmdList.at(i));
    }
    for (i=i+1; i<cmdList.size(); i++)
    {
        pt2.push_back(cmdList.at(i));
    }
    if (pt2.size()==0)//no pipes
    {
        execute(cmdList);
    }
    else
    {
        //pipes2(pt1,pt2);
    int fd[2];
	if(pipe(fd) == -1)
	    perror("pipe");
	    int forktest=fork();
	    if (forktest==-1)
	        perror("fork");
        else if (forktest==0)//child
        {
            //check for I/O redirection
            if(dup2(fd[1],1)==-1)
                perror("dup2");
            if(-1==close(fd[0]))
                perror("close");
            execute(pt1);
        }
        else //parent
        {
            int restore;
            int status=0;
            if((restore=dup(0))==-1)
                perror("dup");
            if(dup2(fd[0],0)==-1)
                perror("dup2");
            if(close(fd[1])==-1)
                perror("close");
            if(waitpid(-1, &status, 0)==-1)
                perror("wait");
            pipes1(pt2);
            if(dup2(restore,0)==-1)
                perror("dup2");
        }
    }
}


int main()
{
    char hostname[128];
    if(gethostname(hostname,sizeof hostname)==-1)
    {
        perror("gethostname");
    }
    while(1)
    {
        cont:
        
        string cmdLine;
        char buffer[BUFSIZ];
	    if(!getcwd(buffer, sizeof(buffer))) //potential error
	    	perror("getcwd");
    	cout<<buffer<<endl;
        cout << getlogin() << "@" << hostname << "$ ";
        getline(cin, cmdLine);
        noCommentZone(cmdLine);
        if(cmdLine.at(0)=='#')//fix bug if everything is a comment
        {
            goto cont;
        }
        connectorFixer(cmdLine); //set up cmdLine to find I/O and pipes
        char *instring=new char[cmdLine.size()+1]; //requires 1 index for '\0'
        strcpy(instring, cmdLine.c_str());
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
        //edge case NO CONNECTORS
        if(curConnector=="")
        {
            int status=0;

            commandlist=tokenizer(instring, " ");
            //check if vector has one index and contains "exit"
            if(commandlist.size()==1&&commandlist.at(0)=="exit")
            {
                cout << commandlist.at(0) << endl;
                cout << commandlist.at(0) << endl;
                delete[] instring;
                goto end;
            }
            signal(SIGINT, CTRLC);
            if(commandlist.at(0)=="cd")
            {
                if(commandlist.size()==1)
                {
                    if(chdir(getenv("HOME"))==-1)
                        perror("chdir");
                }
                else
                {
                    char tmp[commandlist.size()+1];
                    strcpy(tmp, commandlist.at(1).c_str());
                    if(chdir(tmp)==-1)
                        perror("chdir");
                    goto cont;
                    
                }
            }
            int forktest=fork();
            if (forktest==-1)
            {
                perror("fork");
                exit(1);
            }
            else if(forktest==0)
            {
                pipes1(commandlist);
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
            signal(SIGINT, CTRLC);
            if(exV.at(i).at(0)=="cd")
            {
                if(exV.at(i).size()==1)
                {
                    if(chdir(getenv("HOME"))==-1)
                        perror("chdir");
                }
                else
                {
                    char tmp[exV.at(i).at(1).size()+1];
                    strcpy(tmp,exV.at(i).at(1).c_str());
                    if(chdir(tmp)==-1)
                        perror("chdir");
                }
                continue;
            }
            else
            {
                int forktest=fork();
                if (forktest==-1)
                {
                    perror("fork");
                    exit(1);
                }
                else if(forktest==0)//CHILD
                {
                    pipes1(exV.at(i));
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
        
        }
        
    
    
    delete[] instring;
    }
    
    
    
    
    
    end:
    return 0;
}
