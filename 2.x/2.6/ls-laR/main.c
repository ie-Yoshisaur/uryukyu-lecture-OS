#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define INITIAL_STACK_SIZE 10

typedef struct {
    char **data;
    int size;
    int top;
} Stack;

void stack_init(Stack *stack) {
    stack->data = malloc(INITIAL_STACK_SIZE * sizeof(char *));
    stack->size = INITIAL_STACK_SIZE;
    stack->top = -1;
}

void stack_push(Stack *stack, const char *value) {
    if (stack->top == stack->size - 1) {
        stack->size *= 2;
        stack->data = realloc(stack->data, stack->size * sizeof(char *));
    }
    stack->data[++stack->top] = strdup(value);
}

char *stack_pop(Stack *stack) {
    return stack->top == -1 ? NULL : stack->data[stack->top--];
}

void print_directory_contents(const char *dirpath, Stack *stack, Stack *dirnames, int print_dirname_total) {
    struct dirent *dp;
    DIR *dirp = opendir(dirpath);

    if (dirp == NULL) {
        perror("opendir failed");
        return;
    }

    long long total_size = 0;  // 合計サイズを格納する変数

    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        struct stat statbuf;
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, dp->d_name);
        if (lstat(fullpath, &statbuf) == -1) {
            perror("lstat failed");
            continue;
        }

        total_size += statbuf.st_blocks;  // ファイルのサイズを合計に追加

        if (S_ISDIR(statbuf.st_mode)) {
            stack_push(stack, fullpath);
        }
    }

    if (print_dirname_total) {
        printf("%s:\ntotal %lld\n", dirpath, total_size);  // ディレクトリ名と合計サイズを表示
    }

    rewinddir(dirp);

    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }

        struct stat statbuf;
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, dp->d_name);
        if (lstat(fullpath, &statbuf) == -1) {
            perror("lstat failed");
            continue;
        }

        printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
        printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
        printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
        printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
        printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
        printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
        printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
        printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
        printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
        printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %3hu", statbuf.st_nlink);

        struct passwd *pw = getpwuid(statbuf.st_uid);
        struct group *gr = getgrgid(statbuf.st_gid);
        printf(" %-8s", pw ? pw->pw_name : "UNKNOWN");
        printf(" %-8s", gr ? gr->gr_name : "UNKNOWN");

        printf(" %9lld", statbuf.st_size);

        char buff[20];
        struct tm *timeinfo;
        timeinfo = localtime(&statbuf.st_mtime);
        strftime(buff, sizeof(buff), "%b %d %H:%M", timeinfo);
        printf(" %s", buff);

        printf(" %s\n", dp->d_name);
    }

    closedir(dirp);
}

int main(int argc, char *argv[]) {
    Stack stack;
    stack_init(&stack);
    Stack dirnames;
    stack_init(&dirnames);
    int print_dirname_total = 1;

    if (argc == 1) {
        stack_push(&stack, ".");
    } else {
        for (int i = 1; i < argc; i++) {
            stack_push(&stack, argv[i]);
        }
    }

    char *dir;
    while ((dir = stack_pop(&stack)) != NULL) {
        print_directory_contents(dir, &stack, &dirnames, print_dirname_total);
        free(dir);
        if (print_dirname_total && stack.top > -1) {
            printf("\n");
        }
    }

    free(stack.data);
    free(dirnames.data);

    return 0;
}

