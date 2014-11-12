#include <ctype.h>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <ctime>
#include <algorithm>

using namespace std;

void recursion(char*, int);
int findszlk(char*, bool);
int blocks(char*, int);
void printl(struct stat, int, int, char*, char*);


/*
 * This is a BARE BONES example of how to use opendir/readdir/closedir.  Notice
 * that there is no error checking on these functions.  You MUST add error 
 * checking yourself.
 */

int main(int argc, char** argv)
{
    vector<string> v; //if argv is a file or folder
    v.push_back(".");
    bool files=false;
    //a=2, l=3, R=5, a&l=6, a&R=10, l&R=15, a&l&R=30, no flags=1
    //allows use of modulos
    unsigned flag=1;
    for(int i=1; i<argc; i++)
    {
        if(!strcmp(argv[i],"-a"))
            flag*=2;
        else if(!strcmp(argv[i],"-l"))
            flag*=3;
        else if(!strcmp(argv[i],"-R"))
            flag*=5;
        else if(!strcmp(argv[i],"-al"))
            flag*=6;
        else if(!strcmp(argv[i],"-la"))
            flag*=6;
        else if(!strcmp(argv[i],"-aR"))
            flag*=10;
        else if(!strcmp(argv[i],"-Ra"))
            flag*=10;
        else if(!strcmp(argv[i],"-lR"))
            flag*=15;
        else if(!strcmp(argv[i],"-Rl"))
            flag*=15;
        else if(!strcmp(argv[i],"-alR")) 
            flag*=30;
		else if(!strcmp(argv[i],"-aRl")) 
		    flag*=30;
		else if(!strcmp(argv[i],"-laR"))
		    flag*=30;
		else if(!strcmp(argv[i],"-lRa"))
		    flag*=30;
		else if(!strcmp(argv[i],"-Ral"))
		    flag*=30;
		else if(!strcmp(argv[i],"-Rla"))
		    flag*=30;
        else
        {
            if(files==false)//enables file/folder as a parameter
            {
                files=true;
                string tmp=argv[i];
                v.at(0)=tmp;
            }
            else
                v.push_back(argv[i]);
        }
    }
    if (files==false)
    {
	    char tmp[]=".";
            recursion(tmp,flag);//./rshell
    }
    else
    {
        for (unsigned i=0; i<v.size(); i++)
        {
            DIR* dirStream;
            dirent* curDir;
            struct stat s;
            char holder[2000];

            if(!(dirStream = opendir("."))) 
            {
                perror("opendir");
                exit(1);
            }
            //bool file, tester;
            while((curDir=readdir(dirStream)))
            {
                if(errno!=0)
                {
                    perror("readdir");
                    exit(1);
                }
                strcpy(holder,v.at(i).c_str());
                char pathName[1000];
                strcpy(pathName, "./");
                strcat(pathName, curDir->d_name);
                if(stat(pathName,&s)==-1)
                {
                    perror("stat");
                    exit(1);
                }
                if(!strcmp(holder, curDir->d_name))
                {
                    //tester=true;
                    if(s.st_mode&S_IFREG)
                    {
                        //file=true;
                        break;
                    }
                }
                if(closedir(dirStream)==-1)
                {
                    perror("closedir");
                    exit(1);
                }
                //if (tester&&file)
                    recursion(holder,flag);
                
            }
        }
    }
     
    return 0;
}

void recursion(char* dirnm, int flag)//a,l,r = 2,3,5
{
    DIR* dirStream;
    dirent *curDir;
    if (flag%5==0)//set up for -R
        cout << dirnm << ":" << endl;
    //set up for -l
    if(!(dirStream=opendir(dirnm)))
    {
        perror("opendir");
        exit(1);
    }
    int blk, maxsz, maxlk;
    if(flag%3==0)
    {
        blk=blocks(dirnm,flag);
        maxsz=findszlk(dirnm,true);
        maxlk=findszlk(dirnm,false);
    }
    if(flag%3==0)
            cout<<"Total: "<<(blk/2)<<endl;
    vector <char*> cstrings;
    while((curDir=readdir(dirStream)))
    {
        cstrings.push_back(curDir->d_name);
    }
    sort(cstrings.begin(),cstrings.end());
    for (unsigned i=0; i<cstrings.size(); i++)
    {
        if (errno!=0)
        {
            perror("readdir");
        }
        char pathName[1000];
        strcpy(pathName, dirnm);
        strcat(pathName, "/");
        strcat(pathName, cstrings.at(i));
        struct stat s;
        if(stat(pathName,&s)==-1)
            perror("stat");
    
        if (flag==1)
        {
            //no flags, just ls
            if(cstrings.at(i)[0]!='.')
            {
                if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0]=='.')
                    cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
                else if((s.st_mode & S_IXUSR)&&cstrings.at(i)[0]=='.')
                    cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
                else if(s.st_mode & S_IFDIR)
                    cout <<"\x1b[94m"<< cstrings.at(i) << "\x1b[0m";
                else if(s.st_mode & S_IXUSR)
                    cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
                else
                    cout << cstrings.at(i);
                if(s.st_mode & S_IFDIR)
                    cout << '/';
                else if (s.st_mode & S_IXUSR)
                    cout << '*';
                cout << "   ";
                
            }
                
        }
        else if (flag%2==0 && flag%3==0 && flag%5==0)//-a, -l, -R FIXME
        {
            printl(s,maxsz,maxlk,pathName, cstrings.at(i));
            if((s.st_mode&S_IFDIR)&&cstrings.at(i)[0]!='.')
            {
                cout << endl << endl;
                recursion(pathName, flag);
            }
        }
        else if (flag%2==0 && flag%3==0 && flag%5!=0)//-a, -l
        {
            printl(s,maxsz,maxlk,pathName,cstrings.at(i));
        }
        else if (flag%2==0 && flag%3!=0 && flag%5==0)//-a, -R
        {
            if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0]=='.')
                cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
            else if((s.st_mode & S_IXUSR)&&cstrings.at(i)[0]=='.')
                cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
            else if(s.st_mode & S_IFDIR)
                cout <<"\x1b[94m"<< cstrings.at(i) << "\x1b[0m";
            else if(s.st_mode & S_IXUSR)
                cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
            else
                cout << cstrings.at(i);
            if(s.st_mode & S_IXUSR)
                cout << '*';
            else if (s.st_mode & S_IFDIR)
                cout << '/';
            cout << "   ";
            if(cstrings.at(i)[0]!='.' && (s.st_mode & S_IFDIR))
            {
                cout << endl << endl;
                recursion(pathName,flag);
            }
        }
        else if (flag%2!=0 && flag%3==0 && flag%5==0)//-l, -R FIXME
        {
            if(cstrings.at(i)[0]!='.')
                printl(s,maxsz,maxlk,pathName, cstrings.at(i));
            if((s.st_mode&S_IFDIR)&&cstrings.at(i)[0]!='.')
            {
                cout << endl << endl;
                recursion(pathName, flag);
            }
        }
        else if (flag%2==0 && flag%3!=0 && flag%5!=0)//-a
        {
            if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0]=='.')
                cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
            else if((s.st_mode & S_IXUSR)&&cstrings.at(i)[0]=='.')
                cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
            else if(s.st_mode & S_IFDIR)
                cout <<"\x1b[94m"<< cstrings.at(i) << "\x1b[0m";
            else if(s.st_mode & S_IXUSR)
                cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
            else
                cout << cstrings.at(i);
            if(s.st_mode & S_IFDIR)
                cout << '/';
            else if (s.st_mode & S_IXUSR)
                cout << '*';
            cout << "   ";
        }
        else if (flag%2!=0 && flag%3==0 && flag%5!=0)//-l
        {
            if(cstrings.at(i)[0]!='.')
                printl(s,maxsz,maxlk,pathName,cstrings.at(i));
        }
        else if (flag%2!=0 && flag%3!=0 && flag%5==0)//-R
        {
            if(cstrings.at(i)[0]!='.')
            {
                if((s.st_mode & S_IFDIR)&&cstrings.at(i)[0]=='.')
                    cout << "\x1b[94m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
                else if((s.st_mode & S_IXUSR)&&cstrings.at(i)[0]=='.')
                    cout << "\x1b[92m\x1b[100m" << cstrings.at(i) << "\x1b[0m";
                else if(s.st_mode & S_IFDIR)
                    cout <<"\x1b[94m"<< cstrings.at(i) << "\x1b[0m";
                else if(s.st_mode & S_IXUSR)
                    cout << "\x1b[92m" << cstrings.at(i) << "\x1b[0m";
                else
                    cout << cstrings.at(i);
                if(s.st_mode & S_IFDIR)
                    cout << '/';
                else if (s.st_mode & S_IXUSR)
                    cout << '*';
                cout << "   ";
                
            }
            if(cstrings.at(i)[0]!='.' && (s.st_mode & S_IFDIR))
            {
                cout << endl << endl;
                recursion(pathName,flag);
            }
        }
    }
}

int findszlk(char* dirnm, bool isSz)
{
    DIR *dirStream;
    dirent *curDir;
    int max=0;
    if(!(dirStream=opendir(dirnm)))
    {
        perror("opendir");
    }
    while((curDir=readdir(dirStream)))
    {
        int len=1;
        if(errno!=0)
            perror("readdir");
        struct stat s;
        char pathName[1000];
        strcpy(pathName,dirnm);
        strcat(pathName,"/");
        strcat(pathName,curDir->d_name);
        if(stat(pathName,&s)==-1)
            perror("stat");
        if (isSz)
        {
            int temp=s.st_size;
            while(temp/=10)
                len++;
        }
        else
        {
            int temp=s.st_nlink;
            while(temp/=10)
                len++;
        }
        if (len>max)
            max=len;
    }
    return max;
    if(closedir(dirStream)==-1)
        perror("closedir");
}
int blocks(char* dirnm, int flag)
{
    int block=0;
    DIR *dirStream;
    dirent *curDir;
    if(!(dirStream=opendir(dirnm)))
    {
        perror("opendir");
    }
    while((curDir=readdir(dirStream)))
    {
        if(errno!=0)
            perror("readdir");
        struct stat s;
        char pathName[1000];
        strcpy(pathName,dirnm);
        strcat(pathName,"/");
        strcat(pathName,curDir->d_name);
        if(stat(pathName,&s)==-1)
            perror("stat");
        if(flag%2==0)
            block+=s.st_blocks;
        else
        {
            if (curDir->d_name[0]!='.')
                block+=s.st_blocks;
        }
    }
    if(closedir(dirStream)==-1)
        perror("closedir");
    return block;
}

void printl(struct stat s, int maxsz, int maxlk, char* str, char* name)
{
    string dirns=str;
    string command = "stat --printf='%A %h %U %G %s' " + dirns;
    system(command.c_str());
    char temp[18];
    struct tm *Time;
    Time=localtime(&(s.st_mtime));
    strftime(temp,18,"%b %d %H:%M",Time);
    cout <<left << setw(15) << ' ' << temp << ' '; //how do i do this?????
    if((s.st_mode & S_IFDIR)&&str[0]=='.')
        cout << "\x1b[94m\x1b[100m" << name << "\x1b[0m";
    else if((s.st_mode & S_IXUSR)&&str[0]=='.')
            cout << "\x1b[92m\x1b[100m" << name << "\x1b[0m";
    else if(s.st_mode & S_IFDIR)
        cout <<"\x1b[94m"<<name << "\x1b[0m";
    else if(s.st_mode & S_IXUSR)
        cout << "\x1b[92m" << name << "\x1b[0m";
    else
        cout << name;
    if(s.st_mode & S_IFDIR)
        cout << '/';
    else if (s.st_mode & S_IXUSR)
        cout << '*';
    cout << "   ";
    cout << endl;
}
