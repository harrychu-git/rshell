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
	bool Argc3 = false;
	if(argc == 3)
		Argc3 = true;
	else if((strcmp(argv[3],"inout")) && (strcmp(argv[3],"rw1")) && (strcmp(argv[3],"rw2")))
	{
		cerr << "Syntax Error: Please enter the following as parameters: \nInput File\nOutput File\nOptional Parameter (inout, rw1, rw2)" << endl;
		exit(1);
	}
	Timer t;
	double eTime;
	t.start();
	if(!Argc3 && strcmp(argv[3],"inout"))
	{
		cout << "hi " <<  argv[1] << " " << argv[2] << endl;
		ifstream in (argv[1]);
		ofstream out (argv[2]);
	
		while(in.good())
		{
			char c = in.get();
			if(in.good())
			out.put(c);
		}	
	}
	else if(!Argc3 && strcmp(argv[3],"rw1"))
	{
		int infd= open(argv[1],O_RDONLY);
		if (infd == -1)
		{
			perror("open");
			exit(1);
		}
		//What the fuck?
		int outfd = open(argv[2],O_WRONLY | O_CREAT, 0777);
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
	
	else if(Argc3 || strcmp(argv[3],"rw2"))
	{
		int infd= open(argv[1],O_RDONLY);
		if (infd == -1)
		{
			perror("open");
			exit(1);
		}
		int outfd = open(argv[2],O_WRONLY | O_CREAT);
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
