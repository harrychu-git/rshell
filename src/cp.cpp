#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "Timer.h"

using namespace std;

int main(int argc, char *argv[])
{
	vector<string> files;
	bool Argc3 = false;
	char*  op;
	bool opTrue = false;
	if(argc == 3)
	{
		Argc3 = true;
		for(int i=0;i < argc; i++)
			files.push_back(argv[i]);
	}
	else if(argc == 4)
	{
		for(int i=0;i<argc;i++)
		{
			if(strcmp(argv[i],"-inout")==0)
			{
				op = argv[i];
				opTrue = true;
			}
			else if(strcmp(argv[i],"-rw1")==0)
			{
				op = argv[i];
				opTrue = true;
			}
			else if(strcmp(argv[i], "-rw2")==0)
			{
				op = argv[i];
				opTrue = true;
			}
			else
			{
				files.push_back(argv[i]);
			}
		}	
	}
	else
	{
		cout << "Error: Incorrect input, the possible arguments are (Input files, Output files, and optional parameters('-inout', '-rw1', '-rw2'))" << endl;
		exit(1);
	}
	if(!Argc3 && !opTrue)
	{
		cout << "Error: Incorrect input, the possible arguments are (Input files, Output files, and optional parameters('-inout', '-rw1', '-rw2'))" << endl;
	
	}
	ifstream f(files[2].c_str());
	if(f.good())
	{
		f.close();
		cout << "Error: Output file already exist." << endl;
		exit(1);
	}
	else
	{
		f.close();
	}	
	Timer t;
	double eTime;
	t.start();
	if(!Argc3 && strcmp(op,"-inout"))
	{
		ifstream in (files[1].c_str());
		ofstream out (files[2].c_str());
	
		while(in.good())
		{
			char c = in.get();
			if(in.good())
			out.put(c);
		}	
		in.close();
		out.close();
	}
	else if(!Argc3 && strcmp(op,"-rw1"))
	{
		int infd= open(files[1].c_str(),O_RDONLY);
		if (infd == -1)
		{
			perror("open");
			exit(1);
		}
		//What the fuck?
		int outfd = open(files[2].c_str(),O_WRONLY | O_CREAT, 0777);
		if (outfd == -1)
		{
			perror("open");
			exit(1);
		}
		char buf[1];
		int n;
		while((n = read(infd,buf,1)) > 0)
		{
			if(write(outfd,buf,n)==-1)
			{
				perror("write");
				exit(1);
			} 
		}
		if(n==-1)
		{
			perror("read");
			exit(1);
		}	
	}
	
	else if(Argc3 || strcmp(op,"-rw2"))
	{
		int infd= open(files[1].c_str(),O_RDONLY);
		if (infd == -1)
		{
			perror("open");
			exit(1);
		}
		int outfd = open(files[2].c_str(),O_WRONLY | O_CREAT,0777);
		if (outfd == -1)
		{
			perror("open");
			exit(1);
		}
		char buf[BUFSIZ];
		int n;
		while((n = read(infd,buf,BUFSIZ)) > 0)
		{
			if(write(outfd,buf,n)==-1)
			{
				perror("write");
				exit(1);
			} 
		}
		if(n==-1)
		{
			perror("read");
			exit(1);
		}
		close(infd);
		close(outfd);
	}
	t.elapsedWallclockTime(eTime);
	cout << eTime << endl;
	double wTime = eTime;
	t.elapsedUserTime(eTime);
	cout << eTime << endl;
	double uTime = eTime;
	t.elapsedSystemTime(eTime);
	cout << eTime << endl;
	double sTime = eTime;
	t.elapsedTime(wTime,uTime,sTime);
	


	return 0;
}
