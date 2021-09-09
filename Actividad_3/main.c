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
    size_t cantidad;

};

size_t max(size_t val1, size_t val2){
    return val1 > val2 ? val1: val2;
}

void recursiveReaderFalso(char *path, struct Vector* listaTamaños){
    struct Archivo* archivo1 = malloc(sizeof(struct Archivo));
    archivo1->cantidad = 1;
    push_back(listaTamaños, archivo1);
    struct Archivo* archivo2 = malloc(sizeof(struct Archivo));
    archivo2->cantidad = 2;
    push_back(listaTamaños, archivo2);
    struct Archivo* archivo3 = malloc(sizeof(struct Archivo));
    archivo3->cantidad = 90;
    push_back(listaTamaños, archivo3);
    struct Archivo* archivo4 = malloc(sizeof(struct Archivo));
    archivo4->cantidad = 2;
    push_back(listaTamaños, archivo4);
    struct Archivo* archivo5 = malloc(sizeof(struct Archivo));
    archivo5->cantidad = 2;
    push_back(listaTamaños, archivo5);
    struct Archivo* archivo6 = malloc(sizeof(struct Archivo));
    archivo6->cantidad = 11;
    push_back(listaTamaños, archivo6);
    struct Archivo* archivo7 = malloc(sizeof(struct Archivo));
    archivo7->cantidad = 19;
    push_back(listaTamaños, archivo7);
    listaTamaños->maxElement = 90;
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
            struct Archivo* archivo = malloc(sizeof(struct Archivo));
            archivo->cantidad = file_attr.st_size;
            push_back(listaTamaños, archivo);
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

void llenarBuckets(int* buckets, struct Vector* listaTamaños, size_t rango){
    struct Nodo* nodoInicial = begin(listaTamaños);
    while(nodoInicial != NULL){
        size_t fileSize = *((size_t*) nodoInicial->valor);
        size_t indiceBucket = fileSize / rango;
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
    recursiveReaderFalso(dvalue, &listaTamaños);
    printf("Hay %zu archivos y el tamaño máximo es %zu\n", listaTamaños.length, listaTamaños.maxElement);
    size_t anchuraUrna = 15;
    size_t numberOfBuckets = listaTamaños.maxElement / anchuraUrna + 1;
    int* buckets = (int*) malloc(sizeof(int) * numberOfBuckets);
    llenarBuckets(buckets, &listaTamaños, anchuraUrna);
    for(int i = 0; i < 10; i++)
      printf("%d ", buckets[i]);
    puts("");

    return 0;
}






