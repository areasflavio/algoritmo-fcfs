/*
TRABALHO N2 - ECOS01 - SISTEMAS OPERACIONAIS - 2021.2

Grupo:
Flávio Augusto Arêas Vieira - 2019011942
João Henrique Pulini Tonet - 2019003584
Luiz Emanoel Batista Moreira - 2019001202

Tema: 5. FCFS (First-Come, First-Served)

Observação: 
O arquivo de entrada deve estar no mesmo diretório do arquivo executável com o nome de "stdin.txt"

*/

#include <stdio.h>

typedef struct // Struct que representa um processo.
{
    int id;             // Identificação do processo.
    int tempoCriacao;   // Tempo de chegada do processo.
    int duracao;        // Duração do processo.
    int prioridade;     // Prioridade do processo
    int inicio;         // Tempo de inicio da execução do processo.
    int fim;            // Tempo de finalizacao do processo.
} Processo;

// Função para ordenar os processos por tempo de chegada. (Utiliza o algoritmo bubble sort)
void ordenarVetor(Processo vetor[], int tam)  
{
    Processo aux;
    for(int i = 0; i < tam; i++)
        for(int j = 0; j < tam - 1 - i; j ++)
        {
            if(vetor[j+1].tempoCriacao < vetor[j].tempoCriacao)
            {
                aux = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = aux;
            }
        }
}

int main()
{
    FILE *arqStdin;           // Arquivo de entrada.
    FILE *arqStdout;          // Arquivo de saida.
    int totalProcessos = 0;   // Variável que armazena a quantidade de processos presentes no arquivo.
    char c;                   // Variável auxiliar para verificação da quantidade de processos.
    int i = 0;                // Variável auxiliar para controle de loop.
    int auxiliar = 0;         // Variável auxiliar para cálculo do tempo de início/finalização do processo.
    float turnaround = 0;     // Tempo médio de espera.
    float tempoMedioVida = 0; // Tempo médio de vida.

    arqStdin = fopen("stdin.txt", "r");  // Abrir arquivo de entrada.

    if(arqStdin == NULL) // Verificar se foi possível abrir o arquivo de entrada.
    {
        printf("Não foi possível abrir o arquivo de entrada.\n");
        return 0;
    }

    // Loop para contar a quantidade de linhas (processos) contidos no arquivo.
    for (c = getc(arqStdin); c != EOF; c = getc(arqStdin))
        if (c == '\n') 
            totalProcessos++;

    Processo processos[totalProcessos]; // Vetor para armazenar todos os processos.
    fseek(arqStdin, 0, SEEK_SET);       // Voltar para o início do arquivo de leitura.
    
    // Loop para leitura do arquivo e para preencher o vetor "processos".
    while(fscanf(arqStdin, "%d %d %d", &processos[i].tempoCriacao, &processos[i].duracao, &processos[i].prioridade) != EOF) 
    {
        processos[i].id = i+1;
        i++;
    }   

    fclose(arqStdin); // Fechar arquivo de leitura.

    ordenarVetor(processos, totalProcessos); // Ordenar o vetor de processos por tempo de chegada (menor para o maior).
    
    // Loop para calcular o tempo de ínicio/finalização de cada um dos processos.
    auxiliar = processos[0].tempoCriacao;
    for(i = 0; i < totalProcessos; i++)
    {

        if(processos[i].tempoCriacao > auxiliar)
            auxiliar = processos[i].tempoCriacao;

        processos[i].inicio = auxiliar;
        processos[i].fim = auxiliar + processos[i].duracao;
        auxiliar = processos[i].fim;
    }

    arqStdout = fopen("stdout.txt","w"); // Criação do arquivo de saída

    if(arqStdout == NULL) // Verificar se foi possível criar o arquivo de saída.
    {
        printf("Erro ao criar o arquivo de saida.\n");
        return 0;
    }

    // Escrita do diagrama de tempo da execução
    fprintf(arqStdout, "  Tempo\t    Processo\n");
    for(i = 0; i < totalProcessos; i++)
    {
        fprintf(arqStdout, "%3d - %3d        %d\n", processos[i].inicio , processos[i].fim, processos[i].id);
        turnaround += processos[i].inicio - processos[i].tempoCriacao;
        tempoMedioVida += processos[i].fim - processos[i].tempoCriacao;
    }
    turnaround = turnaround/totalProcessos;
    tempoMedioVida = tempoMedioVida/totalProcessos;

    // Escrita do tempo médio de espera, do tempo médio de vida e o número de trocas de contexto
    fprintf(arqStdout, "\nTempo medio de espera (turnaround): %f segundos\n", turnaround);
    fprintf(arqStdout, "Tempo medio de vida: %f segundos\n", tempoMedioVida);
    fprintf(arqStdout, "Numero de trocas de contexto: %d\n", totalProcessos);

    fclose(arqStdout); // Fechar arquivo de saída.
    
    return 0;
}
