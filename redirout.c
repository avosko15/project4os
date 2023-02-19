#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    if (argc <= 2) {
        printf("I need 3 args, please try again\n");
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    int pfd[2];
    pipe(pfd);

    if (fork() == 0) {
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(argv[2], argv + 2);
        perror("child exec");
        exit(1);
    } 

    close(pfd[1]);
    char buffer[2048];
    int bytes;

    while((bytes = read(pfd[0], buffer, 2048)) > 0) {
        write(fd, buffer, bytes);
    }

    close(pfd[0]);
    close(fd);
    return 0;
}
