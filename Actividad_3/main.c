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
    archivo7->cantidad = 90;
    push_back(listaTamaños, archivo7);
    listaTamaños->maxElement = 90;
}

void recursiveReader(char *path, struct Vector* listaTamaños){
    struct dirent* dirp;
    DIR* dp = opendir(path);
    if (!dp)
        return;

    
    char* tempPath = (char*)malloc(sizeof(char)*1000);
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

void llenarBuckets(int* buckets, struct Vector* listaTamaños, size_t rango, int* bucketMasGrande){
    struct Nodo* nodoInicial = begin(listaTamaños);
    while(nodoInicial != NULL){
        size_t fileSize = *((size_t*) nodoInicial->valor);
        size_t indiceBucket = fileSize / rango;
        buckets[indiceBucket]++;
        if(buckets[indiceBucket] > *bucketMasGrande)
          *bucketMasGrande = buckets[indiceBucket];
        nodoInicial = next(nodoInicial);
    }
}

void fillWithSpaces(char* buffer, int sizeOfString){
    int currentLength = strlen(buffer);
    for(;currentLength < sizeOfString; currentLength++)
        strcat(buffer, " ");
}

void printBuckets(int* buckets, size_t rango, size_t numberOfBuckets, size_t numArchivos){
    puts("Urna          Número de archivos Histograma");
    int* final = buckets + numberOfBuckets;
    int inicioRango = 0;
    int finRango = rango - 1;

    int archivosPorAsterisco = 1;
    if(numberOfBuckets > 50)
        archivosPorAsterisco = numberOfBuckets / 50 + 1;

    for(int* aux = buckets; aux < final; aux++){
        char txtRango[14] = {};
        sprintf(txtRango, "%d-%d", inicioRango, finRango);
        fillWithSpaces(txtRango, 13);

        char txtNumFiles[19] = {};
        sprintf(txtNumFiles, "%d", *aux);
        fillWithSpaces(txtNumFiles, 18);

        char txtGrafica[51] = {};
        int numAsteriscos = *aux / archivosPorAsterisco;
        int j = 0;
        for(;j < numAsteriscos; j++)
          strcat(txtGrafica, "*");

        printf("%s %s %s\n", txtRango, txtNumFiles, txtGrafica);
        inicioRango+=rango;
        finRango+=rango;
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
    int bucketMasGrande = 0;
    llenarBuckets(buckets, &listaTamaños, anchuraUrna, &bucketMasGrande);
    printBuckets(buckets, anchuraUrna, numberOfBuckets, bucketMasGrande);

    return 0;
}






