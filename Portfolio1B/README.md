Portfolio 1B- Undo/Redo Text Buffer

# What I built
-> A stack-based text editor simulator. TYPE appends text to the end of the document, DELETE removes characters from the end, and UNDO/REDO are backed by my own Stack<T>class.

# How to BUild and run

->
make
./main

Runs interactively-type commands like TYPE Hello, PRINT, UNDO, REDO, DELETE 5,QUIT.

For the optional bonus(It can load commands from a file and print a summary):

./main commands.txt

Here commands.txt is included in this folder as a sample command file.

# Known Limitations
-> 
1. TYPE only appends to the end and DELETE only removes from the end- there is no cursor, so mid document edits aren't supported.
2. No input validation beyond checking DELETE doesn't remove more characters than exit.
