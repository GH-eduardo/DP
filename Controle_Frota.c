#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

// checklist dos 6 requisitos solicitados pelo professor Alexandre Moreno (algoritmos II) para a primeira entrega DP:
// struct/estrutura                          x
// variáveis                                     x
// comandos de entrada e saída   x
// laços de repetição                     x
// desvios condicionais (if)            x
// vetores e matrizes                     x

struct elemento // elemento da matriz
{
    char elem[255]; 
} matriz[2000][6]; // aqui é declarada uma matriz de strings (vetores de caracteres) utilizando uma estrutura
                            // checklist 2/6: struct e vetores e matrizes
int item = -1;       // a variável item armazena a quantidade de veículos cadastrados, é inicializada com -1 porque antes de cadastrar
                            // um veículo novo na matriz essa variável é incrementada, sendo assim o primeiro item ocupa a posição 0 
                            // se item = 0 há apenas um veículo cadastrado // checklist 3/6: váriavel

int verificaExistencia(int x, char* s); 
void inserirVeiculo ();
void imprimeTabela();
void modificarVeiculo();
void escreveNoArquivo();
void leDoArquivo();

    FILE *arq;

void leDoArquivo() { // carrega a tabela com os carros que já foram cadastrados e estão salvos em um arquivo txt

    char bigString[341];
    char *ponteiro;
    
    arq = fopen("Listagem_de_Veículos.txt", "r"); // leitura
    if (arq == NULL) // Se não encontrou o arquivo // checklist 4/6: desvios condicionais
        printf("Ainda não há nenhum veículos registrado/ arquivo não encontrado!\n\n"); // saída de dados
    else {

        fscanf(arq, "%d", &item); // entrada de dados (de um arquivo) // checklist 5/6: comandos de entrada e saída

        for(int x = 0; x <= item; x++) { // percorre o arquivo gravando as informações obtidas na matriz // checklist 6/6: laços de repetição
            fscanf(arq,"\n%[^\n]s",bigString);
            ponteiro = strtok(bigString, ";"); // a função strtok permite dividir uma string maior em "tokens" que são partes menores (substrings)
            for(int y = 0; y < 6; y++) {           // como no arquivo txt os campos estão separados por ';' esse é o segundo argumento passado
                strcpy(matriz[x][y].elem,ponteiro);  //para que a função separe e posteriormente a inserção na matriz seja feita de forma correta 
                ponteiro = strtok(NULL, ";");
            }
        }
    }
}

void escreveNoArquivo() { // salva a tabela de veículos no arquivo txt

    arq = fopen("Listagem_de_Veículos.txt", "w"); // gravação
	if (arq == NULL)  // Se houve erro na criação do arquivo
    	printf("Problemas na criação do arquivo\n");
	else  {

        fprintf(arq, "%d\n", item);
        for(int x = 0; x <= item; x++) {
            for(int y = 0; y < 7; y++) {

                fprintf(arq, "%s ", matriz[x][y].elem);

            }
            fprintf(arq,"\n");
        }
    }
	fclose(arq);


}

void menu() {

    int  opcao;

    do {
        system("cls");
        printf("\nSeja bem vindo ao menu! por favor escolha uma opção:\n\n");
        printf("1) para inserir um novo veículo\n");
        printf("2) para modificar alguma informação de algum veículo\n");
        printf("3) para imprimir a tabela de veículos cadastrados\n");
        printf("4) para salvar dados\n");
        printf("5) para sair do programa\n");
        printf("\n--> ");
        scanf("%d",&opcao);
        fflush(stdin);

        switch(opcao) {

            case 1: inserirVeiculo(); break;

            case 2: modificarVeiculo(); break;

            case 3: if (item < 0){
                 printf("\nAinda não há nenhum veículo registrado!\n\n");
                 }
                 else {
                imprimeTabela(); } break;

            case 4: escreveNoArquivo(); break;

            case 5: printf("\nSaindo...\n"); break;

            default: printf("\nopção inválida!\n"); getchar();
        }
    } while (opcao != 5); // repete o menu até que o usuário escolha o número da 'opção sair' 

}

int verificaExistencia(int x, char* s) {
    for (int a = 0; a < item; a++) {
        if (strcmp(matriz[a][x].elem, s) == 0) {
            return a;
        }
    }
    return -1;
}

void inserirVeiculo () { // cadastra um novo veículo

    item++; 
    char entrada[255];
    int ondeTem = -1, x;

    printf("\nVeículo: ");
    scanf("%15s", matriz[item][0].elem);
    fflush(stdin);
    x = 1;
    do {
        printf("\nPlaca: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\nO veículo da linha [%d] já está cadastrado com esta placa! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][1].elem, entrada);
        }
    } while (ondeTem != -1);          // prevenção de erro de digitação/redundância não deixando que
    x = 2;                                        // insira um novo veículo com informações já cadastradas 
    do {                                          
        printf("\nChassi: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\nO veículo da linha [%d] já está cadastrado com este chassi! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][2].elem, entrada);
        }
    } while (ondeTem != -1);
    x = 3;
    do {
        printf("\nRenavan: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\nO veículo da linha [%d] já está cadastrado com este renavan! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][3].elem, entrada);
        }
    } while (ondeTem != -1);
    printf("\nAquisição: ");
    scanf("%15s", matriz[item][4].elem);
    fflush(stdin);
    printf("\nSetor: ");
    scanf("%15s", matriz[item][5].elem);
    fflush(stdin);
}

void menuModificar(int *opcao,int *modificar) { // altera algum campo (dos "alteráveis") de um veículo

    do {
        system("cls");
        printf("Qual o número do veículo você deseja modificar? digite a linha correspondente da tabela: \n");
        scanf("%d",modificar);
        fflush(stdin);
        if(*modificar < 0 || *modificar > item) {
            printf("\nOpção inválida!\n");
            getchar();
        }
    } while(*modificar < 0 || *modificar > item); // repete enquanto o usuário não digitar uma opção válida (prevenção de erros)

    do {
        system("cls");
        printf("Qual campo você deseja modificar? Digite a opção correspondente: \n");
        printf("1) Placa\n");
        printf("2) Setor\n");
        printf("\n--> ");
        scanf("%d",opcao);
        fflush(stdin);
        if(*opcao < 1 || *opcao > 2) {
            printf("\nOpção inválida!\n");
            getchar();
        }
    } while(*opcao < 1 || *opcao > 2); // repete o menu enquanto o usuário não digitar uma opção válida
}

void modificarVeiculo() {

    int opcao, modificar; 

    menuModificar(&opcao,&modificar);

    printf("Digite a nova informação para substituir a anterior: ");

    switch(opcao) {
        case 1:
            scanf("%15s", matriz[modificar][1].elem);
            fflush(stdin); break;
        case 2:
            scanf("%15[^\n]s", matriz[modificar][5].elem);
            fflush(stdin); break;
    }
}

void imprimeTabela() {
    int tam;

    if (item < 0) {
        printf("\nAinda não há nenhum veículo registrado!\n");
        getchar();
    }

    printf("\n    |%-12s|%-12s|%-12s|%-12s|%-12s|%-12s\n", "Veículo", "Placa", "Chassi", "Renavan", "Aquisição", "Setor");

    for (int i = 0; i < item; i++) {
        printf("%4d", i);  // imprime o número da linha
        for (int j = 0; j < 6; j++) {
            printf("|");
            for (int z = 0; z < 12; z++) { // imprime o conteúdo de algum elemento da matriz limitando a z caracteres
                printf("%c", matriz[i][j].elem[z]);
            }
            tam = strlen(matriz[i][j].elem);
            if (tam < 12) {
                for (int s = 1; s <= (12 - tam); s++) { // se o campo em questão não tiver z caracteres completa 
                    printf(" ");                                       // com espaços a fim de manter uma boa formatação
                }
            }
        }
        printf("\n");
    }
    getchar();
}

int main() {

    setlocale(LC_ALL, "Portuguese");

    leDoArquivo(); // carrega a tabela com os carros que já foram cadastrados
    menu(); // a partir do menu o usuário realiza todas as funções disponibilizadas pelo programa

    return 0;
}