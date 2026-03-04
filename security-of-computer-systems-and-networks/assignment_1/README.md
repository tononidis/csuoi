# Assignment 1

In this exercise we were tasked to do a buffer over flow attack on a vulnerable server process that was complied with unsafe flags.
When the attack is completed a shell (/bin/sh) opens inside a gdb that runs the vulnerable server.

Compiling with unsafe flags and using gdb makes this attack easy, this is only an example on how a buffer over flow attack works.

I am not the author of the original vulnerable server source code so I created a similar one for the purposes of this exercise.
To make the server use:
make server

We can see from the source code (or with gdb by seeing how much the stack pointer register moved) the size of the overflowed buffer in this example is 512 bytes.
The vulnerable server uses an unsafe call to a memcpy that can overflow the buffer.

A Perl script is used in order to automate the process of attacking the vulnerable server with a malicious payload.
See exploit.pl for more info about the attack steps.

The payload was already given for the purposes of the exercise, but I created a simple assembly program that we can compile into x86 machine instructions.
To extract the payload use:
make shell
objdump -d shell

This will output each assembly instruction as x86 machine instructions, we then glue all these instructions into a payload at the Perl script.

To reproduce the attack create two terminal sessions (should only work in x86_64 Linux):

In terminal 1, lunch in the server in gdb and run it:
gdb ./vulnerable_server
run

In terminal 2, execute the perl script:
perl exploit.pl

A '$' symbol should appear inside gdb, this means we successful open a shell inside the server process.