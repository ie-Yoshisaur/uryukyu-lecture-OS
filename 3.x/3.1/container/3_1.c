#include <stdio.h>
#include <stdlib.h>

void f(int n){
    int f_i;
    for(f_i = 0; f_i < n; f_i++){
        printf("0");
    }
}

void g(int n, int m){
    int g_i;
    for(g_i = 0; g_i < m; g_i++){
        f(n);
    }
    printf("\n");
}
 
int main(int argc, char *argv[]){
    int n = atol(argv[1]);
    int m = atol(argv[2]);
    g(n, m);
}
