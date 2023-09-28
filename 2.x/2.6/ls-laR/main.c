#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void print_directory_contents(const char *dirpath, int print_dirname_total) {
  struct dirent *dp;
  DIR *dirp = opendir(dirpath);

  if (dirp == NULL) {
    perror("opendir failed");
    return;
  }

  if (print_dirname_total) {
    printf("%s:\n", dirpath);
    printf("total 0\n");
  }

  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
      continue;
    }

    struct stat statbuf;
    char fullpath[2048];
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

    if (S_ISDIR(statbuf.st_mode)) {
      print_directory_contents(fullpath, 1);
    }
  }

  closedir(dirp);
}

int main(int argc, char *argv[]) {
  int print_dirname_total = 1;

  if (argc == 1) {
    print_directory_contents(".", print_dirname_total);
  } else {
    for (int i = 1; i < argc; i++) {
      print_directory_contents(argv[i], print_dirname_total);
    }
  }
  return 0;
}
