RSHELL By: Harry Chu
----------------------------------


Rshell is a command shell that can execute commands.
There are many commands including but not limited to:
- cd (change directories)
- echo (print to screen)
- touch (create a file)
- ls (display all files and folders in current repository)

This program contains a file called Makefile which will compile the code.
Makefile will create an executable in a new folder called /bin.

Rshell can take commands as input and execute them. Type "man [command name]"
to open a guide for the command. Multiple commands can be executed by using
"connectors".
Usable connectors are:
-";"
-"&&"
-"||"

HOW TO RUN
----------------------------------
Open rshell by executing the following commands in your terminal or git shell
- git clone  http://github.com/hchu007/rshell.git
- cd rshell
- git checkout hw0
- make
- bin/rshell

CONNECTORS
----------------------------------
-The ";" connector will execute all commands in order.
-The "&&" connector will not execute a command if the previous command failed.
-The "||" connector will not execute a command if the previous command succeeded.

Currently, this version of rshell does not support multiple types of connectors in one command.


KNOWN BUGS
----------------------------------
- When a command is typed in with multiple connectors side by side, ("; ;", "&& &&", "|| ||")
  it does not output an error message.

- echo does not print the correct amount of spaces when multiple spaces are next to each other.
- A command with a connector at the front (";" or ";ls") will not output an error.





LS By: Harry Chu
----------------------------------
ls is a bash command that lists all folders and files in your current directory. ls can be used with 3 flags and can take any file or folder name as a parameter. 

Flags
----------------------------------
-a lists hidden files

-l lists descriptions of each file or folder including
  1. Access rights
  2. Number of links
  3. Number of blocks
  4. Username of owner
  5. Groupname of owner
  6. Date last modified

-R is a recursive call that lists all the files and the files inside each folder

Parameters
----------------------------------
Files and folders can also be used as parameters. When this happens, ls prints out the files and folders FROM THE PARAMETER passed in.

HOW TO RUN
----------------------------------
Open ls by executing the following commands in your terminal or git shell
- git clone  http://github.com/hchu007/rshell.git
- cd rshell
- git checkout hw0
- make
- bin/ls


KNOWN BUGS
----------------------------------
- The formatting for the -l flag is off
- Every output is printed all on one line
- The output when using -R lists the next folder being output as a folder inside the folder currently being output
- When using -R, having a chain of folders inside folders (root<--dir1<--dir2) causes all of the files to be output in the "deepest" folder
- Running bin/ls will create memory leaks






I/O Redirection and Piping By: Harry Chu
----------------------------------
As of November 18th, 2014, rshell is now capable of piping and input/output redirection. Input and output redirection allows input and output beyond the use of stdin, stdout, and stderr. Piping is used to connect multiple data processes.


HOW TO USE I/O REDIRECTION & PIPING
------------------------------------
There are three symbols for I/O redirection ('<', '>', ">>") and one for piping ('|').
1. < is used to accept input from a file as opposed to stdin. For example...
  - wc -l < main.cpp    will count the amount of lines in main.cpp and output it to stdout.
2. > is used to redirect stdout to a file. For example...
  - ls > output1.txt    will store a list of all files and folders in the current directory to output1.txt.
3. >> is similar to > but instead of replacing all of the contents ('>'), it will append onto the contents. For example...
  - echo copyright-2014 >> LICENSE    will append "copyright-2014" at the end of LICENSE
4. | is a general purpose chaining tool. It can be used in a variety of different, yet useful ways. For example...
  - history | tail -10    will list the last 10 commands executed.

All of these symbols can be chained together in the same command. Here are some examples!
 - 2013Names.txt | sort >> 2012Names.txt will sort all of the content in 2013Names.txt and append it at the end of 2012Names.txt
 - wc -l < README > readmeLines.txt will count the lines in README and store it in readmeLines.txt

KNOWN BUGS
----------------------------------
- Cannot handle multiple connectors of the same type. However, using two different connectors will still work.
- Sometimes creates junk files while using output redirection (results have not been duplicated).
- Some invalid commands will create an infinite loop that must be stopped/killed/interrupted.



Signals By: Harry Chu
----------------------------------
As of December 5th, 2014, rshell is now capable of using the PATH variable to find commands, interruption of foreground processes with ^C, and the use of the "cd" command.

HOW TO USE NEW RSHELL
----------------------------------
- Typing "cd" will change directories to the HOME path.
- Typing "cd" with another parameter will change directories to the parameter passed in.
- Pressing CTRL + C will interrupt a foreground process.
- In order to exit rshell, type "exit" (CTRL + C will not work!)

KNOWN BUGS
----------------------------------
- cd will not work with connectors.
- ^C will sometimes not terminate the foreground process and rshell must be "killed" with ^Z.
- ^C will display on the screen.
- cd cannot support more than one parameter.






Report unknown bugs to hchu007@ucr.edu.

