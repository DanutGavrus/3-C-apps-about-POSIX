# Linux POSIX Projects
These projects were made in Linux using POSIX and implement the following:
```
-Listing directory contents;
-Identifying and parsing section files;
-Working with sections;
-Sections filtering;
-Create a Process Hierarchy;
-Synchronizing threads from the same process;
-Create a Threads Barrier;
-Synchronizing threads from different processes;
-Pipe Based Connection;
-Ping Request;
-Request to Create a SHM: Shared Memory Region;
-Write to Shared Memory Request;
-Memory-Map File Request;
-Read From File Offset Request;
-Read From File Section Request;
-Read From Logical Memory Space Offset Request.
```

## Getting Started
You can find each project's description(Chapter 1) and requirements(Chapter 2) in the .pdf file attached to it. Also, you should take a quick look over the code written in the .c file.
```
In order to test any project you need to:
1) Download .rar file and extract it;
2) Open a new terminal in that directory and write(Chapter 2.1): 
   2.1)"gcc -Wall a1.c -o a1";
   2.2)"gcc -Wall a2.c a2_helper.c -o a2 -lpthread"
   2.3)"gcc -Wall a3.c -o a3".
3) In order to test anyone write: "python tester.py" (Chapter 3.1);
4) If you want more details while testing, open tester.py and change verbose to true before step3.
```
When submitted, each project was tested with a similar python file. Below I attached photos for all showing the results of those tests.

## Python Test Scores
1. File System Module
![alt text](https://github.com/DanutGavrus/Photos/blob/master/1.%20File%20System%20Module.png)<br/><br/><br/>
2. Processes, Threads and Synchronization
![alt text](https://github.com/DanutGavrus/Photos/blob/master/2.%20Processes%2C%20Threads%20and%20Synchronization.png)<br/><br/><br/>
3. Inter Process Communication
![alt text](https://github.com/DanutGavrus/Photos/blob/master/3.%20Inter-Process%20Communication.png)

### Warning !
```
! If you can not extract the .rar, open a new terminal and write: sudo apt-get install unrar
```

### Built With
* [Code::Blocks](http://www.codeblocks.org/) - The C IDE used
* [Lubuntu](https://lubuntu.net/) - The OS used