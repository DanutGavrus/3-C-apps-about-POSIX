# Inter Process Communication
This project is made in Linux using POSIX and implements the following as described in the .pdf file:
```
-Pipe Based Connection(Chapter 2.2);
-Ping Request(Chapter 2.3);
-Request to Create a SHM: Shared Memory Region(Chapter 2.4);
-Write to Shared Memory Request(Chapter 2.5);
-Memory-Map File Request(Chapter 2.6);
-Read From File Offset Request(Chapter 2.7);
-Read From File Section Request(Chapter 2.8);
-Read From Logical Memory Space Offset Request(Chapter 2.9).
```

## Getting Started
You can find this project's description(Chapter 1) and requirements(Chapter 2) in the .pdf file attached to it. Also, you should take a quick look over the code written in the .c file.
```
In order to test this project you need to:
1) Download .c and tester.py files in the same directory;
2) Open a new terminal in that directory and write: "gcc -Wall a3.c -o a3" (Chapter 2.1);
3) In order to test it write: "python tester.py" (Chapter 3.1);
4) If you want more details while testing, open tester.py and change verbose to true before step3.
```
When submitted, this project was tested with a similar python file. Below I attached a photo showing the result of those tests.

## Python Tests Passed
![alt text](https://github.com/DanutGavrus/Photos/blob/master/3.%20Inter-Process%20Communication.png)

### Warning !
```
! If you can not extract the .rar, open a new terminal and write: sudo apt-get install unrar
```

### Built With
* [Code::Blocks](http://www.codeblocks.org/) - The C IDE used
* [Lubuntu](https://lubuntu.net/) - The OS used 