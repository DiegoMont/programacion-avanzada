#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>

#include "Vector.c"

struct Archivo{
    int cantidad;

};

int max(int val1, int val2){
    return val1 > val2 ? val1: val2;
}

void recursiveReader(char *path, struct Vector* listaTamaños){
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
            //printf("File: '%s' Size: %lld bytes\n",dirp->d_name, file_attr.st_size);
            struct Archivo* archivo = malloc(sizeof(struct Archivo));
            archivo->cantidad = file_attr.st_size;
            push_back(listaTamaños, &archivo);
            listaTamaños->maxElement = max(listaTamaños->maxElement, archivo->cantidad);
            strcpy(tempPath, path);
            strcat(tempPath, "/");
            strcat(tempPath, dirp->d_name);
            recursiveReader(tempPath, listaTamaños);
        }
    }
    closedir(dp);
    free(tempPath);
}

void llenarBuckets(int* buckets, struct Vector* listaTamaños){
    struct Nodo* nodoInicial = begin(listaTamaños);
    int rango = listaTamaños->maxElement / 10;
    while(nodoInicial != NULL){
        int size = *((int*) nodoInicial->valor);
        int indiceBucket = size / rango;
        buckets[indiceBucket]++;
        nodoInicial = next(nodoInicial);
    }
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

    struct Vector listaTamaños;
    recursiveReader(dvalue, &listaTamaños);
    printf("Hay %zu archivos y el tamaño máximo es %i", listaTamaños.length, listaTamaños.maxElement);
    int buckets[10] = {};
    llenarBuckets(buckets, &listaTamaños);
    for(int i = 0; i < 10; i++)
      printf("%d ", buckets[i]);
    puts("");

    return 0;
}






