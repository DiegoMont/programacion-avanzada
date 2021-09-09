#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>


struct Archivo{
    int cantidad;

};
struct Vector{
    struct Archivo* first, last;

};


void recursiveReader(char *path){
    char* tempPath = (char*)malloc(sizeof(char)*1000);
    struct dirent* dirp;
    DIR* dp = opendir(path);
    if (!dp)
        return;

    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0)
        {
            struct stat file_attr;
            stat(dirp->d_name, &file_attr);
            printf("File: '%s' Size: %lld bytes\n",dirp->d_name, file_attr.st_size);

            strcpy(tempPath, path);
            strcat(tempPath, "/");
            strcat(tempPath, dirp->d_name);
            recursiveReader(tempPath);
        }
    }
    closedir(dp);
    free(tempPath);
}


int main(int argc, char * const * argv){
    char* dvalue = NULL;
    int opt;

    while((opt = getopt(argc, argv, "d:")) != -1){
        switch(opt)
        {
            case 'd' :
            dvalue = optarg;
            break;
            case '?' :
                printf("error");
                return 1;
            default:
                abort();

        }
    }

    recursiveReader(dvalue);



    return 0;
}






