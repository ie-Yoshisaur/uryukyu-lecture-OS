#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void f(int n, int fd){
    int f_i;

    for(f_i = 0; f_i < n; f_i++){
        write(fd, "0", 1);
    }
}

void g(int n, int m, int fd){
    int g_i;

    for(g_i = 0; g_i < m; g_i++){
        f(n, fd);
    }
}
 
int main(int argc, char *argv[]){
    int n = atol(argv[1]);
    int m = atol(argv[2]);
    int fd = open("3_1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    g(n, m, fd);
    close(fd);   
}
