#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define ZERO_ARG 1
#define ONE_ARG 2
#define DATE_CHAR_SIZE 13

int main (int argc, char *argv[]) {
    struct dirent *p_dirent;
    DIR *p_dir;

    if (argc == ZERO_ARG) {
        char *p_current_directory = ".";
        p_dir = opendir(p_current_directory);
    } else if(argc == ONE_ARG) {
        p_dir = opendir(argv[1]);
    }

    if (p_dir == NULL) {
        printf("opendir ERROR: cannot open directory '%s'\n", argv[1]);
        return 1;
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        int file;
        char *p_file_name = p_dirent->d_name;
        if((file=open(p_file_name, O_RDONLY)) < -1) {
            printf("open ERROR: null file descriptor\n");
            close(file); 
            closedir(p_dir);
            return 1;
        }
        struct stat fileStat;
        errno = 0;
        if(fstat(file, &fileStat) < 0){
            printf("fstat ERROR: [%s]\n", strerror(errno)); 
            close(file); 
            closedir(p_dir);
            return 1;
        }
        struct passwd *pws;
        if((pws=getpwuid(fileStat.st_uid)) == NULL) {
            printf("getpwuid ERROR: cannot get user name\n");
            close(file); 
            closedir(p_dir);
            return 1;
        }
        struct group *grp;
        if ((grp = getgrgid(fileStat.st_gid)) == NULL){
            printf("getgrgid ERROR: cannot get group name\n");
            close(file); 
            closedir(p_dir);
            return 1;
        }
        char date[DATE_CHAR_SIZE];
        strftime(date, DATE_CHAR_SIZE, "%b %d %H:%M", localtime(&fileStat.st_mtime));
        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf(" ");
        printf("%d ", fileStat.st_nlink);
        printf("%s ", pws->pw_name);
        printf("%s ", grp->gr_name);
        printf("%lld ", fileStat.st_size);
        printf("%s ", date);
        printf("%s\n", p_file_name);
        close(file); 
    }
    closedir(p_dir);
    return 0;
}
