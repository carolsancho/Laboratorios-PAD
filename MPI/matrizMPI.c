/*
    LABORATORIO III - MPI
    GRUPO: Optimus Tech
Carolina da Silva Sancho 214376
Gabriella Carlos do Nascimento 260587
*/

#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define posicao(I, J, COLUNAS) ((I) * (COLUNAS) + (J))

double multiplicacao(float *matrizA, float *matrizB, float *matrizC, float *matrizD, int y, int w, int v, char arqD[], int size){
    int i, j;
    float *matrizTemp;
    matrizTemp = (float *)calloc(y * v, sizeof(float));

    //realiza a multiplicacao da matrizA[y][w] com a matrizB[w][v] e salva em matrizTemp[y][v]
    MPI_Barrier(MPI_COMM_WORLD);
        for (int lin = 0; lin < y; lin++){
            for (int col = 0; col < v; col++){
                matrizTemp[posicao(lin, col, v)] = 0;
                for (int i = 0; i < w; i++){
                    matrizTemp[posicao(lin, col, v)] += matrizA[posicao(lin, i, w)] * matrizB[posicao(i, col, v)];
                }
            }
        } 

    //realiza a multiplicacao da matrizC[v][1] com a matrizTemp[y][v] e salva em matrizD[y][1]
    MPI_Barrier(MPI_COMM_WORLD);
        for (int lin = 0; lin < y; lin++){
            int col = 0;
            matrizD[posicao(lin, col, 1)] = 0;
            for (int i = 0; i < v; i++){
                matrizD[posicao(lin, col, 1)] += matrizTemp[posicao(lin, i, v)] * matrizC[posicao(i, col, 1)];
            }
        }

    double soma = 0;
    //printa a soma de todos os numeros que tem em matrizD
    MPI_Barrier(MPI_COMM_WORLD);
        for (i = 0; i < y; i++){
            for (j = 0; j < 1; j++){
                soma += matrizD[posicao(i,j,1)];
            }
        }
    MPI_Gather(matrizD, y*1/size, MPI_INT, matrizD, y*1/size, MPI_INT, 0, MPI_COMM_WORLD);

    return soma;
}

void leituraMatriz(float *matriz, int a, int b, char arq[]){
    int i, j;
    FILE *file = fopen(arq, "r");

    //tratamento para verificar se os arquivos digitados existem
    if(file==NULL){
        printf("O arquivo %s que você digitou não existe!\n",arq);
        exit(1);
    }
    char buffer[1024];
    for (i = 0; i < a; i++){
        for (j = 0; j < b; j++){
            fgets(buffer,1024,(FILE*)file);
            matriz[posicao(i,j,b)]=atof(buffer);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]){
    int y, w, v;
    int i, j, k;
    int size, rank;
    char arqA[50];
    char arqB[50];
    char arqC[50];
    char arqD[50];
    float *matrizA, *matrizB, *matrizC, *matrizD;
    srand(time(NULL)); //varia a rand    

    //tratamento para verificar se a quantidade de parametros foi digitado corretamete
    if(argc!=8){
        printf("Você digitou a quantidade de parâmetros errado!\n");
        exit(1);
    }

    y = atoi(argv[1]); //converte para int e armazena em y
    w = atoi(argv[2]);
    v = atoi(argv[3]);
    strcpy(arqA, argv[4]);
    strcpy(arqB, argv[5]);
    strcpy(arqC, argv[6]);
    strcpy(arqD, argv[7]);

    //alocacao das matrizes
    //disponivel em: https://gradvohl.github.io/alocaMatrizes/
    matrizA = (float *)malloc(sizeof(float) * y * w); //matrizA[y][w]
    matrizB = (float *)malloc(sizeof(float) * w * v); //matrizB[w][v]
    matrizC = (float *)malloc(sizeof(float) * v * 1); //matrizC[v][1]
    matrizD = (float *)malloc(sizeof(float) * y * 1); //matrizD[y][1]

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Bcast(matrizA, y*w, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrizB, w*v, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrizC, v*1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrizD, y*1, MPI_INT, 0, MPI_COMM_WORLD);

    if (matrizA == NULL || matrizB == NULL || matrizC == NULL || matrizD == NULL){
        printf("NÃo alocou\n");
        exit(1);
    }

    //chama a funcao leituraMatriz para ver os valores de cada arquivo
    leituraMatriz(matrizA, y, w, arqA);
    leituraMatriz(matrizB, w, v, arqB);
    leituraMatriz(matrizC, v, 1, arqC);

    //clock_t Ticks[2];
    //Ticks[0] = clock();
    double soma;
    //chama a funcao multiplicacao para multiplicacao das matrizes
    soma = multiplicacao(matrizA, matrizB, matrizC, matrizD, y, w, v, arqD, size);
    //Ticks[1] = clock();
    
    //cria o arquivo D e guarda o resultado da multiplicação das matrizes
    FILE *file = fopen(arqD, "w");
    for (i = 0; i < y; i++){
        for (j = 0; j < 1; j++){
            fprintf(file, "%.2f\n", matrizD[posicao(i,j,1)]);
        }
    }
    fclose(file);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    //calculo do tempo de processamento ao realizar a multiplicacao das matrizes + calculo da soma da matriz D
    //float Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    if(rank == 0){
        printf("%lf\n",soma);
        //printf("O tempo de execucao foi de: %.2f ms\n", Tempo);
    }
    

    return 0;
}