#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#define BUFFER_SIZE 100
char buffer[BUFFER_SIZE];

int main(int argc, char** argv){
    int fd;
    ssize_t bytes_read; 

    if(argc >= 2){
        int i;
        for(i = 1; i < argc; i++){
            restart: 
            fd = open(argv[i], O_RDONLY);
            while((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0){
                write(STDOUT_FILENO, buffer, bytes_read);
            }
            if(bytes_read < 0){
                if(EINTR == errno){ //interrupted by signal before data read.
                    goto restart;
                }
                perror("read()"); //print error
            }
            close(fd);
        }
    }else if (argc == 1){
        start:
        while((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0){
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        if(bytes_read < 0){
            if(EINTR == errno){
                goto start;
            }
            perror("read()");
        }
        close(fd);
    }
    return 0;
}
