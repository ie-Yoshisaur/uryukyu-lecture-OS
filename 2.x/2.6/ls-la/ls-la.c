#include <stdio.h>
#include <stdlib.h>
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
#define PERMISSION_CHARS_LENGTH 11
#define MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH 6
#define MAXIMUM_USER_NAME_CHARS_LENGTH 256
#define MAXIMUM_GROUP_NAME_CHARS_LENGTH 256
#define MAXIMUM_BLOCK_SIZE_CHARS_LENGTH 20
#define DATE_CHARS_LENGTH 13
#define MAXIMUM_FILE_NAME_CHARS_LENGTH 256

struct DirectoryInfo {
    int num_of_files;
    int index_of_file;
    int total_size;
    int max_num_of_hard_links_chars_length;
    int max_user_name_chars_length;
    int max_group_name_chars_length;
    int max_block_size_chars_length;
};

struct FileInfo {
    char permissions[PERMISSION_CHARS_LENGTH];
    char num_of_hard_links[MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH];
    char user_name[MAXIMUM_USER_NAME_CHARS_LENGTH];
    char group_name[MAXIMUM_GROUP_NAME_CHARS_LENGTH];
    char block_size[MAXIMUM_BLOCK_SIZE_CHARS_LENGTH];
    char last_modified_date[DATE_CHARS_LENGTH];
    char file_name[MAXIMUM_FILE_NAME_CHARS_LENGTH];
};

void print_space(int num_of_space) {
    // print spaces to adjust the indentation of the output

    for (int i_space=0; i_space<num_of_space; i_space++) {
        printf(" ");
    }
}

void print_file_info_array(struct DirectoryInfo directory_info, struct FileInfo *file_info_array) {
    int num_of_space;
    for (int i_file=0; i_file<directory_info.num_of_files; i_file++) {
        printf("%s ", file_info_array[i_file].permissions);
        // the number of spaces before print num_of_hard_links
        num_of_space = directory_info.max_num_of_hard_links_chars_length - strlen(file_info_array[i_file].num_of_hard_links) + 1;
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].num_of_hard_links);
        // the number of spaces before print user_name
        num_of_space = directory_info.max_user_name_chars_length - strlen(file_info_array[i_file].user_name);
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].user_name);
        // the number of spaces before print group_name
        num_of_space = directory_info.max_group_name_chars_length - strlen(file_info_array[i_file].group_name) + 1;
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].group_name);
        // the number of spaces before print block_size
        num_of_space = directory_info.max_block_size_chars_length - strlen(file_info_array[i_file].block_size) + 1;
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].block_size);
        printf("%s ", file_info_array[i_file].last_modified_date);
        printf("%s\n", file_info_array[i_file].file_name);
    }
}

int main (int argc, char *argv[]) {
    for (int i_dir=0; i_dir<argc; i_dir++) {
        struct dirent *dirent;
        DIR *dir;
        if (argc == ZERO_ARG) {
            char *current_directory = ".";
            dir = opendir(current_directory);
        } else if (i_dir == 0) {
            continue;
        } else {
            dir = opendir(argv[i_dir]);
        }

        if (dir == NULL) {
            printf("opendir ERROR: cannot open directory '%s'\n", argv[1]);
            return 1;
        }
        struct DirectoryInfo directory_info = {0, 0, 0, 0, 0, 0, 0};
        struct FileInfo *file_info_array = malloc(0); // free memory at <1>

        // { a while loop to get the files in the directory
        while ((dirent = readdir(dir)) != NULL) {

            // { read file
            int file;
            char *p_file_name = dirent->d_name;
            int length_of_path = sizeof(argv[i_dir]) + sizeof(p_file_name) + 1;
            char *path_of_file[length_of_path];
            if (argc == ZERO_ARG) {
                sprintf(*path_of_file, "%s/%s", ".", p_file_name);
            } else {
                sprintf(*path_of_file, "%s/%s", argv[i_dir], p_file_name);
            }
            if((file=open(*path_of_file, O_RDONLY)) < -1) {
                printf("open ERROR: null file descriptor\n");
                close(file);
                closedir(dir);
                return 1;
            }
            struct stat fileStat;
            errno = 0;
            if(fstat(file, &fileStat) < 0){
                printf("fstat ERROR: [%s]\n", strerror(errno));
                close(file);
                closedir(dir);
                return 1;
            }
            // } read file

            // { get permissions
            char permissions[PERMISSION_CHARS_LENGTH];
            sprintf(
                permissions,
                "%s%s%s%s%s%s%s%s%s%s",
                (S_ISDIR(fileStat.st_mode)) ? "d" : "-",
                (fileStat.st_mode & S_IRUSR) ? "r" : "-",
                (fileStat.st_mode & S_IWUSR) ? "w" : "-",
                (fileStat.st_mode & S_IXUSR) ? "x" : "-",
                (fileStat.st_mode & S_IRGRP) ? "r" : "-",
                (fileStat.st_mode & S_IWGRP) ? "w" : "-",
                (fileStat.st_mode & S_IXGRP) ? "x" : "-",
                (fileStat.st_mode & S_IROTH) ? "r" : "-",
                (fileStat.st_mode & S_IWOTH) ? "w" : "-",
                (fileStat.st_mode & S_IXOTH) ? "x" : "-"
            );
            // } get permissions

            // { get number of hard links
            char num_of_hard_links[MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH];
            sprintf(num_of_hard_links, "%d", fileStat.st_nlink);
            int num_of_hard_links_chars_length = strlen(num_of_hard_links);
            if (num_of_hard_links_chars_length > directory_info.max_num_of_hard_links_chars_length) {
                directory_info.max_num_of_hard_links_chars_length = num_of_hard_links_chars_length;
            }
            // } get number of hard links

            // { get user name
            struct passwd *pws;
            if((pws=getpwuid(fileStat.st_uid)) == NULL) {
                printf("getpwuid ERROR: cannot get user name\n");
                close(file);
                closedir(dir);
                return 1;
            }
            char user_name[MAXIMUM_USER_NAME_CHARS_LENGTH];
            sprintf(user_name, "%s", pws->pw_name);
            int user_name_chars_length = strlen(user_name);
            if (user_name_chars_length > directory_info.max_user_name_chars_length) {
                directory_info.max_user_name_chars_length = user_name_chars_length;
            }
            // } get user name

            // { get group name
            struct group *grp;
            if ((grp = getgrgid(fileStat.st_gid)) == NULL){
                printf("getgrgid ERROR: cannot get group name\n");
                close(file);
                closedir(dir);
                return 1;
            }
            char group_name[MAXIMUM_GROUP_NAME_CHARS_LENGTH];
            sprintf(group_name, "%s", grp->gr_name);
            int group_name_chars_length = strlen(group_name);
            if (group_name_chars_length > directory_info.max_group_name_chars_length) {
                directory_info.max_group_name_chars_length = group_name_chars_length;
            }
            // } get group name

            // { get block size
            char block_size[MAXIMUM_BLOCK_SIZE_CHARS_LENGTH];
            sprintf(block_size, "%lld", fileStat.st_size);
            int block_size_chars_length = strlen(block_size);
            if (block_size_chars_length > directory_info.max_block_size_chars_length) {
                directory_info.max_block_size_chars_length = block_size_chars_length;
            }
            // } get block size

            // { get last modified date
            char last_modified_date[DATE_CHARS_LENGTH];
            strftime(last_modified_date, DATE_CHARS_LENGTH, "%b %d %H:%M", localtime(&fileStat.st_mtime));
            // } get last modified date

            // { get file name
            char file_name[MAXIMUM_FILE_NAME_CHARS_LENGTH];
            sprintf(file_name, "%s", p_file_name);
            // } get file name

            // { update total size and number of files
            directory_info.total_size += fileStat.st_blocks;
            directory_info.num_of_files += 1;
            // } update total size and number of files

            // { load file information to file_info
            struct FileInfo file_info;
            memcpy(file_info.permissions, permissions, PERMISSION_CHARS_LENGTH);
            memcpy(file_info.num_of_hard_links, num_of_hard_links, MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH);
            memcpy(file_info.user_name, user_name, MAXIMUM_USER_NAME_CHARS_LENGTH);
            memcpy(file_info.group_name, group_name, MAXIMUM_GROUP_NAME_CHARS_LENGTH);
            memcpy(file_info.block_size, block_size, MAXIMUM_BLOCK_SIZE_CHARS_LENGTH);
            memcpy(file_info.last_modified_date, last_modified_date, DATE_CHARS_LENGTH);
            memcpy(file_info.file_name, file_name, MAXIMUM_FILE_NAME_CHARS_LENGTH);
            file_info_array = realloc(file_info_array, sizeof(file_info) * directory_info.num_of_files);
            file_info_array[directory_info.index_of_file] = file_info;
            // } load file information to file_info

            // { update index of file
            directory_info.index_of_file += 1;
            // } update index of file

            close(file);
        }
        // } a while loop to get the files in the directory

        closedir(dir);

        // { print files with long information
        if (argc > ONE_ARG) {
            printf("%s:\n", argv[i_dir]);
        }
        printf("total %d\n", directory_info.total_size);
        print_file_info_array(directory_info, file_info_array);
        if (i_dir != argc - 1) {
            printf("\n");
        }
        // } print files with long information

        free(file_info_array); // free memory <1>
    }
    return 0;
}
