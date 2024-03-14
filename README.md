> [!WARNING]
> To anyone in general, but fellow 42 students in particular: when faced with a problem, it is always better to attempt solving it first alone without consulting the previous works of others. Your precursors were no smarter than and just as fallible as you; Blindly following their footsteps will only lead you on the same detours they took and prevent you from seeing new, faster routes along the way.

# pipex
A project about pipes, forks and IO redirection.

> [!NOTE]  
> 42 Cursus C projects need to follow specific rules and formatting dictated by the [42 Norm](https://github.com/42School/norminette/tree/master/pdf).

## ISSUES
- if a latter fork fails, wait for previous forks before exiting
- command is made of only delimiter characters
- separate checks for file existing and file being executable
- fix error handling messages to be consistent with either bash or zsh
- file open access error, fail process if tied in or outfile fails
- fix memory leak in ./pipex 'infile' '/bin/hello' '/bin/hello' 'outfile2' ( < infile /bin/hello | /bin/hello > outfile )

## Introduction
This was a 42 cursus project where the aim was to replicate pipes in a shell environmnet. The mandatory part required redirecting standard input to a specific file, creating a pipe to communicate between child processes executing different commands, forking and managing said processes, and redirecting output to a specific file. Error handling needed to mimic that of the actual shell (in my case Bash).

In short, `./pipex infile cmd1 cmd2 outfile` behaves exactly like `< infile cmd1 | cmd2 > outfile`. 

One bonus feature I implemented was to handle any number of piped commands, ie. `./pipex infile cmd1 ... cmdN outfile` works like `< infile cmd1 | ... | cmdN > outfile`.

## Installation
Git clone the repository, move into it and run `make` to compile the program. To enable piping with more than two commands, use 'make bonus'.

## Usage
Run the program with `./pipex infile cmd1 cmd2 outfile`

## Implementation
A brief description of my approach to the problem, the optimisations I made and possible areas that could be improved.

### Solution
XXX

### Possible improvements

## Acknowledgements
- Peer evaluators: Carlos!!!!

## Resources
Cyrus McConnell, M C. (2017, November 8). Pipes, Forks & Dups: Understanding Command Execution and Input/Output Data Flow. Retrieved from https://www.rozmichelle.com/pipes-forks-dups/
