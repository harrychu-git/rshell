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



KNOWN BUGS
----------------------------------
- When a command is typed in with two connectors side by side, ("; ;", "&& &&", "|| ||")
  it does not output an error message.

- echo does not print the correct amount of spaces when more than one space is used
	(echo "apple                      pie" prints "apple pie").
- Typing in a single hashtag results in an error message rather than interpreting it as an empty comment.








Report unknown bugs to hchu007@ucr.edu.


