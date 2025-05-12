#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

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
    int nBlocosL2, associatividadeL2, nPalavrasBlocoL2;

    // Lê configuração das três caches
    fscanf(configFile, "%d %d %d", &nBlocosI1, &associatividadeI1, &nPalavrasBlocoI1);
    fscanf(configFile, "%d %d %d", &nBlocosD1, &associatividadeD1, &nPalavrasBlocoD1);
    fscanf(configFile, "%d %d %d", &nBlocosL2, &associatividadeL2, &nPalavrasBlocoL2);

    // Aloca as três caches
    tCache I1, D1, L2;
    alocaCache(&I1, nBlocosI1, associatividadeI1, nPalavrasBlocoI1);
    alocaCache(&D1, nBlocosD1, associatividadeD1, nPalavrasBlocoD1);
    alocaCache(&L2, nBlocosL2, associatividadeL2, nPalavrasBlocoL2);

    // Estatísticas
    int acessosI = 0, falhasI = 0;
    int acessosD = 0, falhasD = 0;
    int acessosL2 = 0, falhasL2 = 0;

    char tipo;
    unsigned int endereco;

    // Loop principal
    while (fscanf(traceFile, " %c %u", &tipo, &endereco) == 2) {
        if (tipo == 'I') {
            acessosI++;
            int resultado = buscaInsereCache(&I1, endereco);
            if (resultado != 0) {
                falhasI++;
                acessosL2++;
                int r2 = buscaInsereCache(&L2, endereco);
                if (r2 != 0) {
                    falhasL2++;
                }
            }
        } else if (tipo == 'L' || tipo == 'S') {
            acessosD++;
            int resultado = buscaInsereCache(&D1, endereco);
            if (resultado != 0) {
                falhasD++;
                acessosL2++;
                int r2 = buscaInsereCache(&L2, endereco);
                if (r2 != 0) {
                    falhasL2++;
                }
            }
        }
    }

    // Impressão dos resultados
    printf("Cache de Instrucoes (I1):\n");
    printf("  Acessos: %d\n", acessosI);
    printf("  Falhas: %d\n", falhasI);
    printf("  Taxa de falhas: %.2f%%\n", (acessosI > 0) ? 100.0 * falhasI / acessosI : 0);

    printf("Cache de Dados (D1):\n");
    printf("  Acessos: %d\n", acessosD);
    printf("  Falhas: %d\n", falhasD);
    printf("  Taxa de falhas: %.2f%%\n", (acessosD > 0) ? 100.0 * falhasD / acessosD : 0);

    printf("Cache de Segundo Nivel (L2):\n");
    printf("  Acessos: %d\n", acessosL2);
    printf("  Falhas: %d\n", falhasL2);
    printf("  Taxa de falhas: %.2f%%\n", (acessosL2 > 0) ? 100.0 * falhasL2 / acessosL2 : 0);

    // Liberação de memória
    liberaCache(&I1);
    liberaCache(&D1);
    liberaCache(&L2);

    fclose(configFile);
    fclose(traceFile);

    return 0;
}
