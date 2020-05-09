C - program is written in C.
pipe.c - contains actual code
makefile- kindly run makefile to run the project


Author :
Shrijeet Rupnar (srupnar1@binghamton.edu) B-N0 : B00808280 
State University of New York, Binghamton



This assignment helps you learn about I/O redirection and inter-process communication. You are asked to write a simple shell program called mysh.

Here is a sample code to get you started. It reads a line of input, breaks it up into individual tokens, and then waits for next line of input. Typing "exit" terminates the shell.

This shell must work as follows. You start the shell by running mysh program. This will give a prompt of your shell as follows:

mysh>
From here onwards, you should be able to execute and control any program/command just as you would in a normal shell. For instance
mysh> Command arg1 arg2 ... argN
[ Output of Command shown here ]
mysh>
Additionally, your shell should be able to do the following:

Redirect the input of a command from a file. For example:
		mysh> Command < input_file
		
Redirect the output of a command to a file. For example:
		mysh> Command > output_file
		
Implement filters, i.e., redirect the stdout of one command to stdin of another using pipes. For example:
		mysh> ls -l | wc -l
		
		mysh> cat somefile | grep somestring | less
		
Ideally, your shell should be able to handle any number of filter components.
Hint: Use of recursion is recommended but not mandatory.
Combine filters and file redirection. For example:
		mysh> Command1 | Command2  > output_file
		mysh> Command1 < input_file  | Command 2 
		mysh> Command1 < input_file  | Command 2 | Command 3 > output_file
		[Or any such valid combinations]
		


