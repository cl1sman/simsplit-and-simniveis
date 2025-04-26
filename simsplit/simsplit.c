#include <stdio.h>
#include <stdlib.h>
#include "cache.h"  // Inclui o cabeçalho com a definição de tCache

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s arquivoConfiguracao arquivoTrace\n", argv[0]);
        return 1;
    }

    FILE *configFile = fopen(argv[1], "r");
    FILE *traceFile = fopen(argv[2], "r");
    if (!configFile || !traceFile) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    int nBlocosI1, associatividadeI1, nPalavrasBlocoI1;
    int nBlocosD1, associatividadeD1, nPalavrasBlocoD1;

    // Ler configurações da cache de instruções (I1)
    fscanf(configFile, "%d %d %d", &nBlocosI1, &associatividadeI1, &nPalavrasBlocoI1);
    // Ler configurações da cache de dados (D1)
    fscanf(configFile, "%d %d %d", &nBlocosD1, &associatividadeD1, &nPalavrasBlocoD1);

    // Declarar variáveis tipo tCache
    tCache I1, D1;

    // Alocar caches (preenche as estruturas)
    alocaCache(&I1, nBlocosI1, associatividadeI1, nPalavrasBlocoI1);
    alocaCache(&D1, nBlocosD1, associatividadeD1, nPalavrasBlocoD1);

    // Estatísticas
    int acessosI = 0, falhasI = 0;
    int acessosD = 0, falhasD = 0;

    char tipo;
    unsigned int endereco;

    // Ler e processar cada linha do trace
    while (fscanf(traceFile, " %c %u", &tipo, &endereco) == 2) {
        if (tipo == 'I') {
            acessosI++;
            int resultado = buscaInsereCache(&I1, endereco);
            if (resultado != 0) {
                falhasI++;
            }
        } else if (tipo == 'L' || tipo == 'S') {
            acessosD++;
            int resultado = buscaInsereCache(&D1, endereco);
            if (resultado != 0) {
                falhasD++;
            }
        }
    }

    // Exibir resultados
    printf("Cache de Instrucoes (I1):\n");
    printf("  Acessos: %d\n", acessosI);
    printf("  Falhas: %d\n", falhasI);
    printf("  Taxa de falhas: %.2f%%\n", (acessosI > 0) ? 100.0 * falhasI / acessosI : 0);

    printf("Cache de Dados (D1):\n");
    printf("  Acessos: %d\n", acessosD);
    printf("  Falhas: %d\n", falhasD);
    printf("  Taxa de falhas: %.2f%%\n", (acessosD > 0) ? 100.0 * falhasD / acessosD : 0);

    // Liberar as caches
    liberaCache(&I1);
    liberaCache(&D1);

    fclose(configFile);
    fclose(traceFile);

    return 0;
}

