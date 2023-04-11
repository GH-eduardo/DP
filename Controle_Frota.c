#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>

// checklist dos 6 requisitos solicitados pelo professor Alexandre Moreno (algoritmos II) para a primeira entrega DP:
// struct/estrutura                          x
// vari�veis                                     x
// comandos de entrada e sa�da   x
// la�os de repeti��o                     x
// desvios condicionais (if)            x
// vetores e matrizes                     x

struct elemento // elemento da matriz
{
    char elem[255]; 
} matriz[2000][6]; // aqui � declarada uma matriz de strings (vetores de caracteres) utilizando uma estrutura
                            // checklist 2/6: struct e vetores e matrizes
int item = -1;       // a vari�vel item armazena a quantidade de ve�culos cadastrados, � inicializada com -1 porque antes de cadastrar
                            // um ve�culo novo na matriz essa vari�vel � incrementada, sendo assim o primeiro item ocupa a posi��o 0 
                            // se item = 0 h� apenas um ve�culo cadastrado // checklist 3/6: v�riavel

int verificaExistencia(int x, char* s); 
void inserirVeiculo ();
void imprimeTabela();
void modificarVeiculo();
void escreveNoArquivo();
void leDoArquivo();

    FILE *arq;

void leDoArquivo() { // carrega a tabela com os carros que j� foram cadastrados e est�o salvos em um arquivo txt

    char bigString[341];
    char *ponteiro;
    
    arq = fopen("Listagem_de_Ve�culos.txt", "r"); // leitura
    if (arq == NULL) // Se n�o encontrou o arquivo // checklist 4/6: desvios condicionais
        printf("Ainda n�o h� nenhum ve�culos registrado/ arquivo n�o encontrado!\n\n"); // sa�da de dados
    else {

        fscanf(arq, "%d", &item); // entrada de dados (de um arquivo) // checklist 5/6: comandos de entrada e sa�da

        for(int x = 0; x <= item; x++) { // percorre o arquivo gravando as informa��es obtidas na matriz // checklist 6/6: la�os de repeti��o
            fscanf(arq,"\n%[^\n]s",bigString);
            ponteiro = strtok(bigString, ";"); // a fun��o strtok permite dividir uma string maior em "tokens" que s�o partes menores (substrings)
            for(int y = 0; y < 6; y++) {           // como no arquivo txt os campos est�o separados por ';' esse � o segundo argumento passado
                strcpy(matriz[x][y].elem,ponteiro);  //para que a fun��o separe e posteriormente a inser��o na matriz seja feita de forma correta 
                ponteiro = strtok(NULL, ";");
            }
        }
    }
}

void escreveNoArquivo() { // salva a tabela de ve�culos no arquivo txt

    arq = fopen("Listagem_de_Ve�culos.txt", "w"); // grava��o
	if (arq == NULL)  // Se houve erro na cria��o do arquivo
    	printf("Problemas na cria��o do arquivo\n");
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
        printf("\nSeja bem vindo ao menu! por favor escolha uma op��o:\n\n");
        printf("1) para inserir um novo ve�culo\n");
        printf("2) para modificar alguma informa��o de algum ve�culo\n");
        printf("3) para imprimir a tabela de ve�culos cadastrados\n");
        printf("4) para salvar dados\n");
        printf("5) para sair do programa\n");
        printf("\n--> ");
        scanf("%d",&opcao);
        fflush(stdin);

        switch(opcao) {

            case 1: inserirVeiculo(); break;

            case 2: modificarVeiculo(); break;

            case 3: if (item < 0){
                 printf("\nAinda n�o h� nenhum ve�culo registrado!\n\n");
                 }
                 else {
                imprimeTabela(); } break;

            case 4: escreveNoArquivo(); break;

            case 5: printf("\nSaindo...\n"); break;

            default: printf("\nop��o inv�lida!\n"); getchar();
        }
    } while (opcao != 5); // repete o menu at� que o usu�rio escolha o n�mero da 'op��o sair' 

}

int verificaExistencia(int x, char* s) {
    for (int a = 0; a < item; a++) {
        if (strcmp(matriz[a][x].elem, s) == 0) {
            return a;
        }
    }
    return -1;
}

void inserirVeiculo () { // cadastra um novo ve�culo

    item++; 
    char entrada[255];
    int ondeTem = -1, x;

    printf("\nVe�culo: ");
    scanf("%15s", matriz[item][0].elem);
    fflush(stdin);
    x = 1;
    do {
        printf("\nPlaca: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\nO ve�culo da linha [%d] j� est� cadastrado com esta placa! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][1].elem, entrada);
        }
    } while (ondeTem != -1);          // preven��o de erro de digita��o/redund�ncia n�o deixando que
    x = 2;                                        // insira um novo ve�culo com informa��es j� cadastradas 
    do {                                          
        printf("\nChassi: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\nO ve�culo da linha [%d] j� est� cadastrado com este chassi! por favor confira e digite novamente\n",ondeTem);
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
            printf("\nO ve�culo da linha [%d] j� est� cadastrado com este renavan! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][3].elem, entrada);
        }
    } while (ondeTem != -1);
    printf("\nAquisi��o: ");
    scanf("%15s", matriz[item][4].elem);
    fflush(stdin);
    printf("\nSetor: ");
    scanf("%15s", matriz[item][5].elem);
    fflush(stdin);
}

void menuModificar(int *opcao,int *modificar) { // altera algum campo (dos "alter�veis") de um ve�culo

    do {
        system("cls");
        printf("Qual o n�mero do ve�culo voc� deseja modificar? digite a linha correspondente da tabela: \n");
        scanf("%d",modificar);
        fflush(stdin);
        if(*modificar < 0 || *modificar > item) {
            printf("\nOp��o inv�lida!\n");
            getchar();
        }
    } while(*modificar < 0 || *modificar > item); // repete enquanto o usu�rio n�o digitar uma op��o v�lida (preven��o de erros)

    do {
        system("cls");
        printf("Qual campo voc� deseja modificar? Digite a op��o correspondente: \n");
        printf("1) Placa\n");
        printf("2) Setor\n");
        printf("\n--> ");
        scanf("%d",opcao);
        fflush(stdin);
        if(*opcao < 1 || *opcao > 2) {
            printf("\nOp��o inv�lida!\n");
            getchar();
        }
    } while(*opcao < 1 || *opcao > 2); // repete o menu enquanto o usu�rio n�o digitar uma op��o v�lida
}

void modificarVeiculo() {

    int opcao, modificar; 

    menuModificar(&opcao,&modificar);

    printf("Digite a nova informa��o para substituir a anterior: ");

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
        printf("\nAinda n�o h� nenhum ve�culo registrado!\n");
        getchar();
    }

    printf("\n    |%-12s|%-12s|%-12s|%-12s|%-12s|%-12s\n", "Ve�culo", "Placa", "Chassi", "Renavan", "Aquisi��o", "Setor");

    for (int i = 0; i < item; i++) {
        printf("%4d", i);  // imprime o n�mero da linha
        for (int j = 0; j < 6; j++) {
            printf("|");
            for (int z = 0; z < 12; z++) { // imprime o conte�do de algum elemento da matriz limitando a z caracteres
                printf("%c", matriz[i][j].elem[z]);
            }
            tam = strlen(matriz[i][j].elem);
            if (tam < 12) {
                for (int s = 1; s <= (12 - tam); s++) { // se o campo em quest�o n�o tiver z caracteres completa 
                    printf(" ");                                       // com espa�os a fim de manter uma boa formata��o
                }
            }
        }
        printf("\n");
    }
    getchar();
}

int main() {

    setlocale(LC_ALL, "Portuguese");

    leDoArquivo(); // carrega a tabela com os carros que j� foram cadastrados
    menu(); // a partir do menu o usu�rio realiza todas as fun��es disponibilizadas pelo programa

    return 0;
}