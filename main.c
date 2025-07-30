#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "obras.dat"

typedef struct {
    int id;
    int anoCriacao;
    float valorEstimado;
    char titulo[100];
    char artista[100];
} ObraArte;

ObraArte* obras = NULL;
int totalObras = 0;
int proximoId = 1;

// Salva todas as obras no arquivo
void salvarEmArquivo() {
    FILE* f = fopen(ARQUIVO, "wb");
    if (f == NULL) {
        printf("Erro ao salvar arquivo!\n");
        return;
    }
    fwrite(&proximoId, sizeof(int), 1, f);
    fwrite(&totalObras, sizeof(int), 1, f);
    fwrite(obras, sizeof(ObraArte), totalObras, f);
    fclose(f);
}

// Carrega as obras do arquivo (se existir)
void carregarDoArquivo() {
    FILE* f = fopen(ARQUIVO, "rb");
    if (f == NULL) return;

    fread(&proximoId, sizeof(int), 1, f);
    fread(&totalObras, sizeof(int), 1, f);
    obras = (ObraArte*)malloc(totalObras * sizeof(ObraArte));
    fread(obras, sizeof(ObraArte), totalObras, f);
    fclose(f);
}

// Adiciona uma nova obra
void adicionarObra() {
    obras = realloc(obras, (totalObras + 1) * sizeof(ObraArte));

    ObraArte nova;
    nova.id = proximoId++;

    printf("Título da obra: ");
    getchar(); // limpar buffer
    fgets(nova.titulo, 100, stdin);
    nova.titulo[strcspn(nova.titulo, "\n")] = '\0';

    printf("Artista: ");
    fgets(nova.artista, 100, stdin);
    nova.artista[strcspn(nova.artista, "\n")] = '\0';

    printf("Ano de criação: ");
    scanf("%d", &nova.anoCriacao);

    printf("Valor estimado: ");
    scanf("%f", &nova.valorEstimado);

    obras[totalObras++] = nova;
    printf("Obra adicionada com sucesso!\n");
}

// Lista todas as obras
void listarObras() {
    if (totalObras == 0) {
        printf("Nenhuma obra cadastrada.\n");
        return;
    }

    printf("\nLista de Obras:\n");
    for (int i = 0; i < totalObras; i++) {
        printf("ID: %d | Título: %s | Artista: %s | Ano: %d | Valor: R$ %.2f\n",
            obras[i].id, obras[i].titulo, obras[i].artista,
            obras[i].anoCriacao, obras[i].valorEstimado);
    }
}

// Remove uma obra por id
void removerObra() {
    int id;
    printf("Digite o ID da obra a remover: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < totalObras; i++) {
        if (obras[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Obra não encontrada.\n");
        return;
    }

    for (int i = index; i < totalObras - 1; i++) {
        obras[i] = obras[i + 1];
    }

    totalObras--;
    obras = realloc(obras, totalObras * sizeof(ObraArte));
    printf("Obra removida com sucesso!\n");
}

// Editar uma obra
void editarObra() {
    int id;
    printf("Digite o ID da obra que deseja editar: ");
    scanf("%d", &id);

    for (int i = 0; i < totalObras; i++) {
        if (obras[i].id == id) {
            printf("Editando obra: %s (ID %d)\n", obras[i].titulo, id);

            printf("Novo título: ");
            getchar();
            fgets(obras[i].titulo, 100, stdin);
            obras[i].titulo[strcspn(obras[i].titulo, "\n")] = '\0';

            printf("Novo artista: ");
            fgets(obras[i].artista, 100, stdin);
            obras[i].artista[strcspn(obras[i].artista, "\n")] = '\0';

            printf("Novo ano de criação: ");
            scanf("%d", &obras[i].anoCriacao);

            printf("Novo valor estimado: ");
            scanf("%f", &obras[i].valorEstimado);

            printf("Obra editada com sucesso!\n");
            return;
        }
    }

    printf("Obra com ID %d não encontrada.\n", id);
}

void menu() {
    int opcao;

    do {
        printf("\n--- MENU OBRAS DE ARTE ---\n");
        printf("1. Listar obras\n");
        printf("2. Adicionar nova obra\n");
        printf("3. Remover obra\n");
        printf("4. Editar obra\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: listarObras(); break;
            case 2: adicionarObra(); break;
            case 3: removerObra(); break;
            case 4: editarObra(); break;
            case 5: salvarEmArquivo(); printf("Encerrando...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 5);
}

int main() {
    carregarDoArquivo();
    menu();
    free(obras);
    return 0;
}
