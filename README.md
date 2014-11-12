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








Report unknown bugs to hchu007@ucr.edu.



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
Open rshell by executing the following commands in your terminal or git shell
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
- When using -R aving a chain of folders inside folders (root<--dir1<--dir2) causes all of the files to be output in the "deepest" folder








Report unknown bugs to hchu007@ucr.edu.


