all:
	if [ -d "/bin" ]; then \
        mkdir bin; \
    fi
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell
rshell:
	g++ -Wall -Werror -ansi -pedantic src/main.cpp -o bin/rshell