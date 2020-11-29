/*
    LABORATORIO II - OpenACC
    GRUPO: Optimus Tech
Carolina da Silva Sancho 214376
Gabriella Carlos do Nascimento 260587
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define posicao(I, J, COLUNAS) ((I) * (COLUNAS) + (J))

void escreveArquivo(float *matriz, int a, int b, char arq[]){
    int i, j;
    FILE *file = fopen(arq, "w");
    for (i = 0; i < a; i++){
        for (j = 0; j < b; j++){
            matriz[posicao(i,j,b)] = rand() % 21 - 10;
            fprintf(file, "%.2f\n",matriz[posicao(i,j,b)]);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]){
    int y, w, v;
    int i, j, k;
    char arqA[50];
    char arqB[50];
    char arqC[50];
    float *matrizA, *matrizB, *matrizC;
    srand(time(NULL)); //varia a rand    

    //tratamento para verificar se a quantidade de parametros foi digitado corretamete
    if(argc!=7){
        printf("Você digitou a quantidade de parâmetros errado!\n");
        exit(1);
    }

    y = atoi(argv[1]); //converte para int e armazena em y
    w = atoi(argv[2]);
    v = atoi(argv[3]);
    strcpy(arqA, argv[4]);
    strcpy(arqB, argv[5]);
    strcpy(arqC, argv[6]);

    //alocacao das matrizes
    //disponivel em: https://gradvohl.github.io/alocaMatrizes/
    matrizA = (float *)malloc(sizeof(float) * y * w); //matrizA[y][w]
    matrizB = (float *)malloc(sizeof(float) * w * v); //matrizB[w][v]
    matrizC = (float *)malloc(sizeof(float) * v * 1); //matrizC[v][1]

    if (matrizA == NULL || matrizB == NULL || matrizC == NULL){
        printf("NÃo alocou\n");
        exit(1);
    }

    //chama a funcao escreveArquivo para gravar os valores aleatorios nos arquivos
    escreveArquivo(matrizA, y, w, arqA);
    escreveArquivo(matrizB, w, v, arqB);
    escreveArquivo(matrizC, v, 1, arqC);

    return 0;
}