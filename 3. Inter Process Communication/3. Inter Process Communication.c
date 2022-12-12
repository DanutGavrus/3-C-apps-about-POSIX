// Basic libraries
#include<stdlib.h>
#include<stdio.h>

// Strings
#include <string.h>

// Pipes
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Shared Memory
#include <sys/ipc.h>
#include <sys/shm.h>

// Mapping
#include <sys/mman.h>

int resp, req, shm;
unsigned char *myMap;
unsigned char *myMap2;
char *pShmChar;
char task[100], size;

void writeOnResp(char *aux) {
    int n = 0;
    while(aux[n] != '\0') n++;
    size = n;
    write(resp, &size, 1);
    write(resp, aux, size);
}

void readTaskFromReq() {
    read(req, &size, 1);
    int n = read(req, task, size);
    task[n] = '\0';
}

void makeConnections() {
    // Create the response pipe
    resp = mkfifo("RESP_PIPE_53294", 0666); if (resp < 0) { printf("ERROR\n"); printf("cannot create the response pipe"); exit(1); }
    // Open the request pipe
    req = open("REQ_PIPE_53294", O_RDONLY); if (req < 0) { printf("ERROR\n"); printf("cannot open the request pipe"); exit(1); }
    // Open the response pipe
    resp = open("RESP_PIPE_53294", O_WRONLY);
    // Write on the response pipe
    writeOnResp("CONNECT");
}

void ping() {
    unsigned int nr;
    if (strcmp(task, "PING") == 0) {
        writeOnResp("PING");
        writeOnResp("PONG");
        nr = 53294; write(resp, &nr, sizeof(nr));
    }
}

void createShm() {
    unsigned int nr;
    if (strcmp(task, "CREATE_SHM") == 0) {
        read(req, &nr, 4);
        if (nr == 4832913) {
            writeOnResp("CREATE_SHM");
            shm = shmget(14601, nr, IPC_CREAT | 0664);
            if (shm < 0) {
                writeOnResp("ERROR");
            }
            else {
                writeOnResp("SUCCESS");
            }
        }
    }
}

void writeShm() {
    unsigned int offset, value;
    pShmChar = (char*)shmat(shm, 0, 0);
    readTaskFromReq();
    while (strcmp(task, "WRITE_TO_SHM") == 0) {
        writeOnResp("WRITE_TO_SHM");
        read(req, &offset, 4);
        read(req, &value, 4);
        if (offset > 0 && (offset + sizeof(value)) <= 4832913) {
            *(int*)&pShmChar[offset] = value;
            writeOnResp("SUCCESS");
        }
        else {
            writeOnResp("ERROR");
        }
        readTaskFromReq();
    }
}

void mapFile() {
    if (strcmp(task, "MAP_FILE") == 0) {
        writeOnResp("MAP_FILE");
        readTaskFromReq();
        int fd = open(task, O_RDONLY);
        if (fd < 0) writeOnResp("ERROR");
        else {
            off_t size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            myMap = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (myMap < 0) writeOnResp("ERROR");
            else writeOnResp("SUCCESS");
        }
    }
}

void readFromFile() {
    if (strcmp(task, "MAP_FILE") == 0) {
        writeOnResp("MAP_FILE");
        readTaskFromReq();
        int fd = open(task, O_RDONLY);
        if (fd < 0) writeOnResp("ERROR");
        else {
            off_t size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            myMap2 = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
            int size2 = size;
            if (myMap2 < 0) writeOnResp("ERROR");
            else writeOnResp("SUCCESS");
            readTaskFromReq();
            // Read from file offset
            while (strcmp(task, "READ_FROM_FILE_OFFSET") == 0) {
                unsigned int offset, no_of_bytes;
                read(req, &offset, 4);
                read(req, &no_of_bytes, 4);
                writeOnResp("READ_FROM_FILE_OFFSET");
                if(offset + no_of_bytes < size2) {
                    for(int i = 0; i < no_of_bytes; i++)
                        pShmChar[i] = myMap2[offset + i];
                    writeOnResp("SUCCESS");
                }
                else writeOnResp("ERROR");
                readTaskFromReq();
            }
            // Read from file section
            while (strcmp(task, "READ_FROM_FILE_SECTION") == 0) {
                unsigned int section_no, offset, no_of_bytes;
                read(req, &section_no, 4);
                read(req, &offset, 4);
                read(req, &no_of_bytes, 4);
                writeOnResp("READ_FROM_FILE_SECTION");
                unsigned int section_offset = 0;
                int d = 1;
                for (int i = 0; i < 4; i++) {
                    section_offset = section_offset + d * myMap2[8 + (section_no - 1) * 31 + 23 + i];
                    d *= 256;
                }
                if (offset + no_of_bytes < size2 && section_no < 6) {
                    for (int i = 0; i < no_of_bytes; i++)
                        pShmChar[i] = myMap2[section_offset + offset + i];
                    writeOnResp("SUCCESS");
                }
                else writeOnResp("ERROR");
                readTaskFromReq();
            }
            while (strcmp(task, "READ_FROM_LOGICAL_SPACE_OFFSET") == 0) {
                unsigned int logical_offset, no_of_bytes;
                read(req, &logical_offset, 4);
                read(req, &no_of_bytes, 4);
                writeOnResp("READ_FROM_LOGICAL_SPACE_OFFSET");
                for (int i = 0; i < no_of_bytes; i++) {
                    pShmChar[i] = myMap2[logical_offset + i];
                }
                writeOnResp("SUCCESS");
                readTaskFromReq();
            }
        }
    }
}

int main() {
    makeConnections();
    readTaskFromReq();
    mapFile();
    ping();
    createShm();
    writeShm();
    readFromFile();
    return 0;
}
