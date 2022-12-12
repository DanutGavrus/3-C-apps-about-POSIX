// input output and string
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// working with directories and files
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

// using bool variables
#include <stdbool.h>

// for using access and lseek
#include <unistd.h>

// function for printing the variant
void printVariant() {
    printf("53294\n");
}

// function for listing the files in a directory
void list (char *path, bool isRecursive, int size_smaller, bool has_perm_write, int rec) {
    if (strcmp(path, "") == 0) printf("ERROR\ninvalid directory path\n");
    else { // the path was given
        DIR *d;
        struct dirent *dir;
        d = opendir(path);
        if (d) {
            if (rec == 0) printf("SUCCESS\n");
            dir = readdir(d);
            while (dir != NULL) {
                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                    /// create the complete path to display
                    char newPath[200] = "";
                    strcpy(newPath, path);
                    strcat(newPath, "/");
                    strcat(newPath, dir->d_name);
                    /// check if it is recursive
                    if (isRecursive) list(newPath, isRecursive, size_smaller, has_perm_write, 1);
                    /// get some info about the file before displaying it
                    struct stat st;
                    stat(newPath, &st);
                    int fileSize = st.st_size;
                    bool isFile = false;
                    if (S_ISREG(st.st_mode)) isFile = true;
                    /// print the path
                    if (size_smaller == 0 && has_perm_write == false) printf("%s\n", newPath);
                    else if (size_smaller > 0 && has_perm_write == true) {
                        if (access(newPath, W_OK) == 0 && fileSize < size_smaller) printf("%s\n", newPath);
                    }
                    else if (size_smaller > 0 && fileSize < size_smaller && isFile == true) printf("%s\n", newPath);
                    else if (has_perm_write == true && access(newPath, W_OK) == 0) printf("%s\n", newPath);
                }
                dir = readdir(d);
            }
            closedir(d);
        }
        else if (rec == 0) printf("ERROR\ninvalid directory path\n");
    }
}

// function for parsing the file
void parse(char *path) {
    char magic[2] = "";
    int version = 0, no_of_sections = 0;
    int fd = open(path, O_RDONLY);
    bool error = false;
    if (fd) { // path is good
        // read magic
        lseek(fd, 0, SEEK_SET);
        read(fd, &magic, 1);
        magic[1] = '\0'; // end line for printing
        if (strcmp(magic, "V")) { printf("ERROR\nwrong magic\n"); error = true;}
        // read version
        lseek(fd, 3, SEEK_SET);
        read(fd, &version, 4);
        if (version < 122 || version > 212) { printf("ERROR\nwrong version\n"); error = true;}
        // read nr of sections
        lseek(fd, 7, SEEK_SET);
        read(fd, &no_of_sections, 1);
        if (no_of_sections < 4 || no_of_sections > 16) { printf("ERROR\nwrong sect_nr"); error = true;}
        // position and read the sections
        char sect_name[no_of_sections][100];
        int sect_type[no_of_sections];
        int sect_size[no_of_sections];
        lseek(fd, 8, SEEK_SET);
        for (int i = 0; i < no_of_sections; i++) {
            read(fd, &sect_name[i], 19);
            sect_name[i][19] = '\0';
            read(fd, &sect_type[i], 4);
            if (sect_type[i] != 57 && sect_type[i] != 89 && sect_type[i] != 67 && sect_type[i] != 27 && sect_type[i] != 64) { printf("ERROR\nwrong sect_types"); error = true;}
            lseek(fd, 4, SEEK_CUR);
            read(fd, &sect_size[i], 4);
        }
        // display what was read
        if (!error) {
            printf("SUCCESS\n");
            printf("version=%d\n", version);
            printf("nr_sections=%d\n", no_of_sections);
            for (int i = 0; i < no_of_sections; i++) {
                printf("section%d: %s %d %d\n", i + 1, sect_name[i], sect_type[i], sect_size[i]);
            }
        }
    } else printf("ERROR\nwrong path\n");
}

// function for extracting from file
void extract(char *path, int section, int line) {
    int no_of_sections = 0, sect_offset = 0, sect_size = 0;
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("ERROR\ninvalid file\n");
    }
    else {
        // read nr of sections
        lseek(fd, 7, SEEK_SET);
        read(fd, &no_of_sections, 1);
        if (section > no_of_sections) printf("ERROR\ninvalid section");
        else {
            lseek(fd, 31 + 31 * (section - 1), SEEK_SET);
            read(fd, &sect_offset, 4);
            read(fd, &sect_size, 4);
            lseek(fd, sect_offset, SEEK_SET);
            char *lineText = (char*)malloc(sect_size);
            char c;
            int lineCount = 0, pos = 0, i;
            for (i = 0; i < sect_size; i++) {
                read(fd, &c, 1);
                lineText[pos] = c;
                pos++;
                if (c == 0x0A) lineCount++;
            }
            if (line > lineCount + 1) printf("ERROR\ninvalid line");
            else {
                printf("SUCCESS\n");
                pos = 0;
                i = 0;
                while (i < sect_size) {
                    if(lineText[i] == 0x0A) pos++;
                    if(pos == lineCount - line + 2) {
                        i--;
                        while (i >= 0 && lineText[i] != 0x0A) {
                            printf("%c", lineText[i]);
                            i--;
                        }
                        printf("%c", 0x0A);
                        i = sect_size;
                    }
                    i++;
                }
            }
            free(lineText);
        }
    }
}

// function for finding all files
void findall(char *path, int rec) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        if (rec == 0) printf("SUCCESS\n");
        dir = readdir(d);
        while (dir != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                /// create the complete path to display
                char newPath[200] = "";
                strcpy(newPath, path);
                strcat(newPath, "/");
                strcat(newPath, dir->d_name);
                /// check if it is recursive
                findall(newPath, 1);
                int fd = open(newPath, O_RDONLY);
                int no_of_sections = 0, lineCount, sect_offset, sect_size;
                bool good = false;
                lseek(fd, 7, SEEK_SET);
                read(fd, &no_of_sections, 1);
                for (int section = 1; section <= no_of_sections; section++) {
                    lineCount = 0;
                    lseek(fd, 31 + 31 * (section - 1), SEEK_SET);
                    read(fd, &sect_offset, 4);
                    read(fd, &sect_size, 4);
                    lseek(fd, sect_offset, SEEK_SET);
                    char c;
                    for (int i = 0; i <= sect_size; i++) {
                        read(fd, &c, 1);
                        if (c == 0x0A) lineCount++;
                    }
                    if(lineCount >= 16) good = true;
                }
                if (good) printf("%s\n", newPath);
            }
            dir = readdir(d);
        }
        closedir(d);
    }
    else if (rec == 0) printf("ERROR\ninvalid directory path\n");
}

// main function that comands which function will be called
int main(int argc, char **argv){
    if (argc >= 2){
        char path[200] = "";
        if (strcmp(argv[1], "variant") == 0) { /// variant function will be called
            printVariant();
        }
        else if (strcmp(argv[1], "list") == 0) { /// list function will be called
            /// parameters initialization
            bool isRecursive = false;
            int size_smaller = 0;
            bool has_perm_write = false;
            char size_aux[100] = "";
            /// parameters set up
            for (int i = 2; i < argc; i++) {
                if (strstr(argv[i], "path=") != NULL) strcpy(path, argv[i] + 5);
                if (strcmp(argv[i], "recursive") == 0) isRecursive = true;
                if (strstr(argv[i], "size_smaller=")) {
                    strcpy(size_aux, argv[i] + 13);
                    size_smaller = atoi(size_aux);
                }
                if (strcmp(argv[i], "has_perm_write") == 0) has_perm_write = true;
            }
            // call function
            list(path, isRecursive, size_smaller, has_perm_write, 0);
        }
        else if (strcmp(argv[1], "parse") == 0) { /// parse function will be called
            /// parameter set up
            strcpy(path, argv[2] + 5);
            // call function
            parse(path);
        }
        else if (strcmp(argv[1], "extract") == 0) { /// extract function will be called
            /// parameter initialization
            int section = 0;
            int line = 0;
            char sectionAux[100];
            char lineAux[100];
            /// parameter set up
            strcpy(path, argv[2] + 5);
            strcpy(sectionAux, argv[3] + 8);
            strcpy(lineAux, argv[4] + 5);
            section = atoi(sectionAux);
            line = atoi(lineAux);
            extract(path, section, line);
        }
        else if (strcmp(argv[1], "findall") == 0) { /// findall function will be called
            /// parameter set up
            strcpy(path, argv[2] + 5);
            findall(path, 0);
        }
        //}
    }
    return 0;
}
