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

void recursiveReader(char *path, struct Vector* listaTamaños){
    DIR* currentOpenedDirectory = opendir(path);
    if (currentOpenedDirectory == NULL)
        return;
    
    struct dirent* directoryEntry = readdir(currentOpenedDirectory);
    int directoryPathLength = strlen(path);
    
    while (directoryEntry != NULL){
        struct stat fileStats;
        stat(directoryEntry->d_name, &fileStats);
        if(S_ISDIR(fileStats.st_mode)){
            int isParentDirectory = !strcmp(directoryEntry->d_name, "..");
            int isCurrentDirectory = !strcmp(directoryEntry->d_name, ".");
            if (!(isParentDirectory || isCurrentDirectory)) {
                int subdirectoryNameLength = strlen(directoryEntry->d_name);
                size_t subdirectoryLength = subdirectoryNameLength + directoryPathLength + 2;
                char* subdirectoryPath = (char*) malloc(sizeof(char) * subdirectoryLength);
                for(int i = 0; i < subdirectoryLength; i++)
                    subdirectoryPath[i] = 0;
                strcat(subdirectoryPath, path);
                strcat(subdirectoryPath, "/");
                strcat(subdirectoryPath, directoryEntry->d_name);
                recursiveReader(subdirectoryPath, listaTamaños);
                free(subdirectoryPath);
            }
        } else {
            printf("File: '%s' Size: %lld bytes\n",directoryEntry->d_name, fileStats.st_size);
            struct Archivo* archivo = malloc(sizeof(struct Archivo));
            archivo->cantidad = (size_t) fileStats.st_size;
            push_back(listaTamaños, archivo);
            listaTamaños->maxElement = max(listaTamaños->maxElement, archivo->cantidad);
        }
        directoryEntry = readdir(currentOpenedDirectory);
    }
    closedir(currentOpenedDirectory);
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

void printBuckets(int* buckets, size_t rango, size_t numberOfBuckets, int bucketMasGrande){
    puts("Urna          Número de archivos Histograma");
    int inicioRango = 0;
    int finRango = rango - 1;

    int archivosPorAsterisco = 1;
    if(bucketMasGrande > 50)
        archivosPorAsterisco = bucketMasGrande / 50 + 1;

    int* final = buckets + numberOfBuckets;
    for(int* aux = buckets; aux < final; aux++){
        char txtRango[14] = {};
        sprintf(txtRango, "%d-%d", inicioRango, finRango);
        fillWithSpaces(txtRango, 13);

        char txtNumFiles[19] = {};
        sprintf(txtNumFiles, "%d", *aux);
        fillWithSpaces(txtNumFiles, 18);

        char txtGrafica[51] = {};
        int numAsteriscos = *aux / archivosPorAsterisco;
        for(int j = 0;j < numAsteriscos; j++)
          strcat(txtGrafica, "*");

        printf("%s %s %s\n", txtRango, txtNumFiles, txtGrafica);
        inicioRango+=rango;
        finRango+=rango;
    }
}

void cleanBuckets(int* bucketsSucios, size_t numberOfBuckets){
    for (size_t i = 0; i < numberOfBuckets; i++)
        bucketsSucios[i] = 0;
}

int main(int argc, char*  argv[]){
    size_t anchuraUrna;
    char* dvalue = NULL;
    if(argc > 2){
        anchuraUrna = (size_t) atoi(argv[1]);
        dvalue = argv[2];
    } else if(argc > 1){
        anchuraUrna = (size_t) atoi(argv[1]);
        dvalue = ".";
    } else {
        puts("Indica el rango de urna");
        return 1;
    }

    struct Vector listaTamaños;
    recursiveReader(dvalue, &listaTamaños);
    printf("Hay %zu archivos y el tamaño máximo es %zu\n", listaTamaños.length, listaTamaños.maxElement);
    size_t numberOfBuckets = listaTamaños.maxElement / anchuraUrna + 1;
    int* buckets = (int*) malloc(sizeof(int) * numberOfBuckets);
    cleanBuckets(buckets, numberOfBuckets);
    int bucketMasGrande = 0;
    llenarBuckets(buckets, &listaTamaños, anchuraUrna, &bucketMasGrande);
    printBuckets(buckets, anchuraUrna, numberOfBuckets, bucketMasGrande);

    return 0;
}






