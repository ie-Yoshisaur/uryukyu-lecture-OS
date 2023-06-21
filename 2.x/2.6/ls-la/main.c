#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void print_directory_contents(const char *dirpath, int print_dirname_total) {
    struct dirent *dp;
    DIR *dirp = opendir(dirpath);
    int total = 0;

    if (dirp == NULL) {
        perror("opendir failed");
        return;
    }

    while ((dp = readdir(dirp)) != NULL) {
        struct stat statbuf;
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, dp->d_name);
        if (lstat(fullpath, &statbuf) == -1) {
            perror("lstat failed");
            continue;
        }
        total += statbuf.st_blocks;
    }

    rewinddir(dirp);

    if (print_dirname_total) {
        printf("%s:\ntotal %d\n", dirpath, total);
    }

    while ((dp = readdir(dirp)) != NULL) {
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
    int print_dirname_total = argc > 2;
    if (argc == 1) {
        print_directory_contents(".", print_dirname_total);
    } else {
        for (int i = 1; i < argc; i++) {
            print_directory_contents(argv[i], print_dirname_total);
            if (print_dirname_total && i < argc - 1) {
                printf("\n");
            }
        }
    }
    return 0;
}

