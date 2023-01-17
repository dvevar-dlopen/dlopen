#me@comp:~$ uname -a
#Linux dell 5.15.0-58-generic #64-Ubuntu SMP Thu Jan 5 11:43:13 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux

#me@comp:~$ which g++
#/usr/bin/g++

#me@comp:~$ realpath /usr/bin/g++
#/usr/bin/x86_64-linux-gnu-g++-11

#me@comp:~$ g++ --version
#g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
#Copyright (C) 2021 Free Software Foundation, Inc.
#This is free software; see the source for copying conditions.  There is NO
#warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

CC = g++
NAME = lib_info

################################################################################
all : link

link : compile
	$(CC) -o $(NAME) $(NAME).o

compile :
	$(CC) -o $(NAME).o -c $(NAME).cpp

clean :
	rm -f *.o
	rm -f $(NAME)
