# Linux POSIX Projects
These projects were made for POSIX and implement the following:
```
-Listing directory contents.
-Identifying and parsing section files.
-Working with sections.
-Sections filtering.
-Create a Process Hierarchy.
-Synchronizing threads from the same process.
-Create a Threads Barrier.
-Synchronizing threads from different processes.
-Pipe Based Connection.
-Ping Request.
-Request to Create a SHM: Shared Memory Region.
-Write to Shared Memory Request.
-Memory-Map File Request.
-Read From File Offset Request.
-Read From File Section Request.
-Read From Logical Memory Space Offset Request.
```

## Getting Started
* You can find each project's description(Chapter 1) and requirements(Chapter 2) in the .pdf file attached to it.
* You may take a quick look over the code written in the .c files.

In order to manually test any project you need to:
```
1) Download the .rar file and extract it;
2) Open a new terminal in that directory and write: 
   2.1)"gcc -Wall a1.c -o a1" for 1st.
   2.2)"gcc -Wall a2.c a2_helper.c -o a2 -lpthread" for 2nd.
   2.3)"gcc -Wall a3.c -o a3" for 3rd.
3) Run "python tester.py" to test it.
4) If you want more details while testing, open tester.py and change verbose to true before step3.
```

## Python Test Scores
##### Each project was tested with a similar python file. Below I have attached the results of those tests.
###### File System Module
![File System Module](https://user-images.githubusercontent.com/56603839/226363422-cb9f07a4-c0db-4cf2-bc8d-8ee6296d4693.png)
###### Processes, Threads and Synchronization
![Processes, Threads and Synchronization](https://user-images.githubusercontent.com/56603839/226363474-ab7306f6-eac7-4abb-8918-2c0860514b56.png)
###### Inter Process Communication
![Inter Process Communication](https://user-images.githubusercontent.com/56603839/226363535-2e61c458-8adb-4efb-83b2-4e095b055efd.png)
