#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int numeroConta;
    char nome[50];
    float saldo;
} Cliente;

void cadastrarCliente(FILE *arq) {
    Cliente c;
    int pos;

    printf("Posicao do registro: ");
    scanf("%d", &pos);

    printf("Numero da conta: ");
    scanf("%d", &c.numeroConta);

    printf("Nome: ");
    scanf(" %[^\n]", c.nome);

    printf("Saldo: ");
    scanf("%f", &c.saldo);

    fseek(arq, pos * sizeof(Cliente), SEEK_SET);
    fwrite(&c, sizeof(Cliente), 1, arq);

    printf("Cliente cadastrado com sucesso!\n");
}

void consultarCliente(FILE *arq) {
    Cliente c;
    int conta;
    int encontrado = 0;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.numeroConta == conta) {
            printf("\nConta: %d\n", c.numeroConta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: %.2f\n", c.saldo);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Cliente nao encontrado!\n");
}

void atualizarSaldo(FILE *arq) {
    Cliente c;
    int conta;
    float novoSaldo;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.numeroConta == conta) {

            printf("Novo saldo: ");
            scanf("%f", &novoSaldo);

            c.saldo = novoSaldo;

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arq);

            printf("Saldo atualizado!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void encerrarConta(FILE *arq) {
    Cliente vazio = {0, "", 0};
    Cliente c;
    int conta;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arq);

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.numeroConta == conta) {

            fseek(arq, -sizeof(Cliente), SEEK_CUR);
            fwrite(&vazio, sizeof(Cliente), 1, arq);

            printf("Conta encerrada!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void listarClientes(FILE *arq) {
    Cliente c;

    printf("\n=== LISTA DE CLIENTES ===\n");

    while (fread(&c, sizeof(Cliente), 1, arq) == 1) {
        if (c.numeroConta != 0) {
            printf("Conta: %d\n", c.numeroConta);
            printf("Nome : %s\n", c.nome);
            printf("Saldo: %.2f\n\n", c.saldo);
        }
    }
}

int main() {
    FILE *arq;
    int opcao;

    arq = fopen("clientes.dat", "r+b");

    if (arq == NULL)
        arq = fopen("clientes.dat", "w+b");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    do {
        printf("\n===== MENU =====\n");
        printf("1. Cadastrar cliente\n");
        printf("2. Consultar cliente\n");
        printf("3. Atualizar saldo\n");
        printf("4. Encerrar conta\n");
        printf("5. Listar clientes\n");
        printf("6. Repetir listagem (rewind)\n");
        printf("7. Encerrar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarCliente(arq);
                break;

            case 2:
                consultarCliente(arq);
                break;

            case 3:
                atualizarSaldo(arq);
                break;

            case 4:
                encerrarConta(arq);
                break;

            case 5:
                rewind(arq);
                listarClientes(arq);
                break;

            case 6:
                rewind(arq);
                printf("Arquivo reposicionado no inicio.\n");
                listarClientes(arq);
                break;

            case 7:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 7);

    fclose(arq);

    return 0;
}