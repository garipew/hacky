# getenvaddr
This program has the objective to predict the address of the given environment variable on the targeted program.

It was written based on my reading of the "Hacking: The art of Exploitation" book by Jon Erickson.

## Building
Since the publish of the book's second edition, in 2008, a lot has changed. The behaviour presented on the book does not happen by default, but can still be achieved. In order to help with that, both scripts 'activate.sh' and 'deactivate.sh' were included in this repo.

To build the program, execute as root the 'activate.sh' script.
```
sudo ./activate.sh 
```
This disables ASLR and compiles the program with the appropriated flags.

### Warning
In order to demonstrate the workings of this program, a security measure is disabled in your system by 'activate.sh'. Before leaving the context of this demonstration, make sure to reenable it with the 'deactivate.sh' script. Again, executing it as root.
```
sudo ./deactivate.sh
```

## Execution
In order to validate the results of this experiment, the "printenvaddr" program is included. This program prints the address of a targeted environment variable. These programs can be executed in sequence in order to observate the workings of the 'getenvaddr' program. 
```
export TEST="This is a test environment variable"
./getenvaddr printenvaddr TEST
./printenvaddr TEST
```
With this, we can check that the addresses outputted by both programs are the same, meaning that 'getenvaddr' can successfully predict the addresses of environment variables in other programs.    
