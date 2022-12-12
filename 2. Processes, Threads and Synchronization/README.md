# Processes, Threads and Synchronization
This project is made in Linux using POSIX and implements the following as described in the .pdf file:
```
-Create a Process Hierarchy(Chapter 2.2);
-Synchronizing threads from the same process(Chapter 2.3);
-Create a Threads Barrier(Chapter 2.4);
-Synchronizing threads from different processes(Chapter 2.5).
```

## Getting Started
You can find this project's description(Chapter 1) and requirements(Chapter 2) in the .pdf file attached to it. Also, you should take a quick look over the code written in the .c file.
```
In order to test this project you need to:
1) Download .c and tester.py files in the same directory;
2) Open a new terminal in that directory and write: "gcc -Wall a2.c a2_helper.c -o a2 -lpthread" (Chapter 2.1);
3) In order to test it write: "python tester.py" (Chapter 3.1);
4) If you want more details while testing, open tester.py and change verbose to true before step3.
```
When submitted, this project was tested with a similar python file. Below I attached a photo showing the result of those tests.

## Python Tests Passed
![alt text](https://github.com/DanutGavrus/Photos/blob/master/2.%20Processes%2C%20Threads%20and%20Synchronization.png)

### Warning !
```
! If you can not extract the .rar, open a new terminal and write: sudo apt-get install unrar
```

### Built With
* [Code::Blocks](http://www.codeblocks.org/) - The C IDE used
* [Lubuntu](https://lubuntu.net/) - The OS used 