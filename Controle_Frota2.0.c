#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <unistd.h> //biblioteca para usar a fun��o 'sleep'
#define MAX_USUARIOS 30
#define MAX_TENTATIVAS 3
#define TEMPO_DE_ESPERA 120 // tempo de block caso haja 3 tentativas falhas de login seguidas (em segundos) 

// checklist dos 7 requisitos solicitados pelo professor Alexandre Moreno (algoritmos II) para a segunda entrega DP:
// struct/estrutura                         X
// vari�veis                                    X
// comandos de entrada e sa�da   X
// la�os de repeti��o                     X
// desvios condicionais (if)            X
// vetores e matrizes                     X
// sistema de login que bloqueia ap�s 3 tentativas falhas X

struct USUARIO{
    char username[30];
    char password[20];
    int horario; //vai armazenar o hor�rio do block (quando excede o numero de tentativas de login) ou o hor�rio do ultimo login
} usuarios[MAX_USUARIOS];

struct elemento { // elemento da matriz
    char elem[255]; 
} matriz[2000][6]; // aqui � declarada uma matriz de strings (vetores de caracteres) utilizando uma estrutura
                            // checklist 2/6: struct e vetores e matrizes
int item = -1;       // a vari�vel item armazena a quantidade de ve�culos cadastrados, � inicializada com -1 porque antes de cadastrar
                            // um ve�culo novo na matriz essa vari�vel � incrementada, sendo assim o primeiro item ocupa a posi��o 0 
                            // se item = 0 h� apenas um ve�culo cadastrado // checklist 3/6: vari�vel

FILE *arq;

int login();
int verificaExistencia(int x, char* s); 
void atualizaLogin(int quantUsuarios);
void inserirVeiculo();
void imprimeTabela();
void modificarVeiculo();
void escreveNoArquivo();
void leDoArquivo();

int login() {

    int num_usuarios = 0, escolha = 0, achou_usuario = 0, i, j;
    int num_tentativas[num_usuarios];
    char username[30];
    char password[20];

    arq = fopen("login.txt","r");
    if (arq == NULL) { // Se n�o encontrou o arquivo
        printf("Arquivo de login n�o encontrado!\n\n"); // sa�da de dados
        printf("Gerando Arquivo de login em branco...\n\n"); 
        arq = fopen("login.txt","w");
    }
    else {
        fscanf(arq, "%d", &num_usuarios);
        for (int a = 0; a < num_usuarios; a++) {
            fscanf(arq, "%s %s %d\n", usuarios[a].username, usuarios[a].password, &usuarios[a].horario);
        }
    }
    fclose(arq);

    for(int x = 0; x < num_usuarios; x++) {
        num_tentativas[x] = 0; // inicializa todos os usu�rios com as tentativas zeradas
    }
    while (1) {
        system("cls");
        printf("\n1. Cadastrar-se\n");
        printf("2. Fazer login\n");
        printf("3. Sair\n");
        printf("\nEscolha uma op��o: ");
        scanf("%d", &escolha);
        fflush(stdin);

        if (escolha == 1) {
            if (num_usuarios == MAX_USUARIOS) {
                printf("N�mero m�ximo de usu�rios atingido\n");
            }
            else {
                printf("\nDigite um nome de usu�rio de sua prefer�ncia: ");
                scanf("%s", usuarios[num_usuarios].username);
                fflush(stdin);
                printf("Digite uma senha de sua prefer�ncia: ");
                scanf("%s", usuarios[num_usuarios].password);
                fflush(stdin);
                usuarios[num_usuarios].horario = time(NULL);
                printf("\nUsu�rio cadastrado com sucesso!\n");
                num_usuarios++;
            }
        }
        else if (escolha == 2) {

            printf("\nDigite o nome de usu�rio: ");
            scanf("%s", username);
            fflush(stdin);
            printf("Digite a senha: ");
            scanf("%s", password);
            fflush(stdin);

            for (i = 0; i < num_usuarios; i++) { //percorre todas as linhas (todos os usu�rios)
                time_t agora = time(NULL);
                if (strcmp(usuarios[i].username, username) == 0 /*Se o usu�rio existe*/ && agora - usuarios[i].horario < 0 /*Se est� bloqueado*/) {
                    printf("\nA conta '%s' est� bloqueada por mais %ld segundos\n", usuarios[i].username, (agora - usuarios[i].horario)*-1);
                        break;
                }
                else {
                    if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) { //se o usu�rio e a senha batem
                        system("cls");
                        printf("\n\nLogin realizado com sucesso!\n");
                        num_tentativas[i] = 0;
                        atualizaLogin(num_usuarios);
                        sleep(3);
                        return 1;
                    }
                }
            }
            if (i == num_usuarios) { // Se percorreu todas as linhas no la�o acima e n�o existe login com o usu�rio e senha correspondente aos digitados
                printf("\nNome de usu�rio ou senha incorretos!\n");
                for (j = 0; j < num_usuarios; j++) {
                    if (strcmp(usuarios[j].username, username) == 0) {
                        achou_usuario = 1;
                        break;
                    }
                }
                if (achou_usuario) { // se o usu�rio existe a senha � que foi digitada errada
                    num_tentativas[j]++;
                    if (num_tentativas[j] >= MAX_TENTATIVAS) {
                        usuarios[j].horario = time(NULL) + TEMPO_DE_ESPERA;
                        printf("\nN�mero m�ximo de tentativas atingido. Conta bloqueada por %d minutos.\n", TEMPO_DE_ESPERA/60);
                    }
                }
            }
            sleep(3);
        }
        else if (escolha == 3) {
            printf("\n\nSaindo...\n");
            atualizaLogin(num_usuarios);
            return 0;
        }
        else {
            system("cls");
            printf("\nOp��o inv�lida!\n");
            sleep(2);
        }
    }
}

void atualizaLogin(int quantUsuarios) {
    arq = fopen("login.txt", "w"); // grava��o
    fprintf(arq,"%d\n", quantUsuarios);
    for (int x = 0; x < quantUsuarios; x++) {
        fprintf(arq,"%s %s %d\n", usuarios[x].username, usuarios[x].password, usuarios[x].horario);
    }
    fclose(arq);
}

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
    fclose(arq);
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
        printf("6) para sair e logar com outra conta\n");
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

            case 6: system("cls"); login(); break;

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
    
    bool sucesso;
    sucesso = login();
    if(!sucesso) { // se n�o logou fecha o programa
        return 0;
    }
    leDoArquivo(); // carrega a tabela com os carros que j� foram cadastrados
    menu(); // a partir do menu o usu�rio realiza todas as fun��es disponibilizadas pelo programa

    return 0;
}