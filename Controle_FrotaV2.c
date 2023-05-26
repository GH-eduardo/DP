#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <unistd.h> //biblioteca para usar a função 'sleep'
#define MAX_USUARIOS 30
#define MAX_TENTATIVAS 3
#define TEMPO_DE_ESPERA 120 // tempo de block caso haja 3 tentativas falhas de login seguidas (em segundos) 
#define CARAC 

// checklist dos 7 requisitos solicitados pelo professor Alexandre Moreno (algoritmos II) para a segunda entrega DP:
// struct/estrutura                         X
// variáveis                                    X
// comandos de entrada e saída   X
// laços de repetição                     X
// desvios condicionais (if)            X
// vetores e matrizes                     X
// sistema de login que bloqueia após 3 tentativas falhas X

struct USUARIO{
    char username[30];
    char password[20];
    int horario; //vai armazenar o horário do block (quando excede o numero de tentativas de login) ou o horário do ultimo login
} usuarios[MAX_USUARIOS];

struct elemento { // elemento da matriz
    char elem[255]; 
} matriz[2000][6]; // aqui é declarada uma matriz de strings (vetores de caracteres) utilizando uma estrutura
                            // checklist 2/6: struct e vetores e matrizes
int item = -1;       // a variável item armazena a quantidade de veículos cadastrados, é inicializada com -1 porque antes de cadastrar
                            // um veículo novo na matriz essa variável é incrementada, sendo assim o primeiro item ocupa a posição 0 
                            // se item = 0 há apenas um veículo cadastrado // checklist 3/6: variável

FILE *arq;

int login();
int verificaExistencia(int x, char* s); 
int particiona( int inicio, int fim);
void atualizaLogin(int quantUsuarios);
void inserirVeiculo();
void imprimeTabela();
void modificarVeiculo();
void escreveNoArquivo();
void leDoArquivo();
void pesquisaPorNome();
void quickSort();
void troca(int a, int b);

int login() {

    int num_usuarios = 0, escolha = 0, achou_usuario = 0, i, j;
    int num_tentativas[num_usuarios];
    char username[30];
    char password[20];

    arq = fopen("login.txt","r");
    if (arq == NULL) { // Se não encontrou o arquivo
        printf("\t\tArquivo de login não encontrado!\n\n"); // saída de dados
        printf("\t\tGerando Arquivo de login em branco...\n\n"); 
        arq = fopen("login.txt","w");
        sleep(2);
    }
    else {
        fscanf(arq, "%d", &num_usuarios);
        for (int a = 0; a < num_usuarios; a++) {
            fscanf(arq, "%s %s %d\n", usuarios[a].username, usuarios[a].password, &usuarios[a].horario);
        }
    }
    fclose(arq);

    for(int x = 0; x < num_usuarios; x++) {
        num_tentativas[x] = 0; // inicializa todos os usuários com as tentativas zeradas
    }
    while (1) {
        system("cls");
        printf("\n\t\t1. Cadastrar-se\n");
        printf("\t\t2. Fazer login\n");
        printf("\t\t3. Sair\n");
        printf("\n\t\tEscolha uma opção: ");
        scanf("%d", &escolha);
        fflush(stdin);

        if (escolha == 1) {
            if (num_usuarios == MAX_USUARIOS) {
                printf("\t\tNúmero máximo de usuários atingido\n");
            }
            else {
                printf("\n\t\tDigite um nome de usuário de sua preferência: ");
                scanf("%s", usuarios[num_usuarios].username);
                fflush(stdin);
                printf("\t\tDigite uma senha de sua preferência: ");
                scanf("%s", usuarios[num_usuarios].password);
                fflush(stdin);
                usuarios[num_usuarios].horario = time(NULL);
                printf("\n\t\tUsuário cadastrado com sucesso!\n");
                num_usuarios++;
            }
        }
        else if (escolha == 2) {

            printf("\n\t\tDigite o nome de usuário: ");
            scanf("%s", username);
            fflush(stdin);
            printf("\t\tDigite a senha: ");
            scanf("%s", password);
            fflush(stdin);

            for (i = 0; i < num_usuarios; i++) { //percorre todas as linhas (todos os usuários)
                time_t agora = time(NULL);
                if (strcmp(usuarios[i].username, username) == 0 /*Se o usuário existe*/ && agora - usuarios[i].horario < 0 /*Se está bloqueado*/) {
                    printf("\n\t\tA conta '%s' está bloqueada por mais %ld segundos\n", usuarios[i].username, (agora - usuarios[i].horario)*-1);
                        break;
                }
                else {
                    if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) { //se o usuário e a senha batem
                        system("cls");
                        printf("\n\n\t\tLogin realizado com sucesso!\n");
                        num_tentativas[i] = 0;
                        atualizaLogin(num_usuarios);
                        sleep(2);
                        return 1;
                    }
                }
            }
            if (i == num_usuarios) { // Se percorreu todas as linhas no laço acima e não existe login com o usuário e senha correspondente aos digitados
                printf("\n\t\tNome de usuário ou senha incorretos!\n");
                for (j = 0; j < num_usuarios; j++) {
                    if (strcmp(usuarios[j].username, username) == 0) {
                        achou_usuario = 1;
                        break;
                    }
                }
                if (achou_usuario) { // se o usuário existe a senha é que foi digitada errada
                    num_tentativas[j]++;
                    if (num_tentativas[j] >= MAX_TENTATIVAS) {
                        usuarios[j].horario = time(NULL) + TEMPO_DE_ESPERA;
                        printf("\n\t\tNúmero máximo de tentativas atingido. Conta bloqueada por %d minutos.\n", TEMPO_DE_ESPERA/60);
                    }
                }
            }
            sleep(3);
        }
        else if (escolha == 3) {
            printf("\n\n\t\tSaindo...\n");
            atualizaLogin(num_usuarios);
            return 0;
        }
        else {
            system("cls");
            printf("\n\t\tOpção inválida!\n");
            sleep(2);
        }
    }
}

void atualizaLogin(int quantUsuarios) {
    arq = fopen("login.txt", "w"); // gravação
    fprintf(arq,"%d\n", quantUsuarios);
    for (int x = 0; x < quantUsuarios; x++) {
        fprintf(arq,"%s %s %d\n", usuarios[x].username, usuarios[x].password, usuarios[x].horario);
    }
    fclose(arq);
}

void leDoArquivo() { // carrega a tabela com os carros que já foram cadastrados e estão salvos em um arquivo txt

    char bigString[341];
    char *ponteiro;
    
    arq = fopen("Listagem_de_Veículos.txt", "r"); // leitura
    if (arq == NULL) // Se não encontrou o arquivo // checklist 4/6: desvios condicionais
        printf("\t\tAinda não há nenhum veículos registrado/ arquivo não encontrado!\n\n"); // saída de dados
    else {

        fscanf(arq, "%d", &item); // entrada de dados (de um arquivo) // checklist 5/6: comandos de entrada e saída

        for(int x = 0; x < item; x++) { // percorre o arquivo gravando as informações obtidas na matriz // checklist 6/6: laços de repetição
            fscanf(arq,"\n%[^\n]s",bigString);
            ponteiro = strtok(bigString, ";"); // a função strtok permite dividir uma string maior em "tokens" que são partes menores (substrings)
            for(int y = 0; y < 6; y++) {           // como no arquivo txt os campos estão separados por ';' esse é o segundo argumento passado
                strcpy(matriz[x][y].elem,ponteiro);  //para que a função separe e posteriormente a inserção na matriz seja feita de forma correta 
                ponteiro = strtok(NULL, ";");
            }
        }
    }
    fclose(arq);
}

void escreveNoArquivo() { // salva a tabela de veículos no arquivo txt

    arq = fopen("Listagem_de_Veículos.txt", "w"); // gravação
	if (arq == NULL)  // Se houve erro na criação do arquivo
    	printf("\t\tProblemas na criação do arquivo\n");
	else  {

        fprintf(arq, "%d\n", item);
        for(int x = 0; x <= item; x++) {
            for(int y = 0; y < 6; y++) {

                fprintf(arq, "%s ", matriz[x][y].elem);
            }
            fprintf(arq,"\n");
        }
    }
	fclose(arq);
}

bool inverso = false;

void menu() {

    int  opcao;
    inverso = false;
   
 do {
        system("cls");
        printf("\n\t\tSeja bem vindo ao menu! por favor escolha uma opção:\n\n");
        printf("\t\t1) para inserir um novo veículo\n");
        printf("\t\t2) para modificar alguma informação de algum veículo\n");
        printf("\t\t3) para imprimir a tabela de veículos cadastrados\n");
        printf("\t\t4) para salvar dados\n");
        printf("\t\t5) para pesquisar pelo nome do veículo\n");
        printf("\t\t6) para ordenar a tabela em ordem alfabética (a-z)\n");
        printf("\t\t7) para ordenar a tabela em ordem alfabética (z-a)\n");
        printf("\t\t8) para sair do programa\n");
        printf("\t\t9) para sair e logar com outra conta\n");
        printf("\n\t\t--> ");
        scanf("%d",&opcao);
        fflush(stdin);

        switch(opcao) {

            case 1: inserirVeiculo(); break;

            case 2: modificarVeiculo(); break;

            case 3: imprimeTabela(); break;

            case 4: escreveNoArquivo(); break;

            case 5: pesquisaPorNome(); break;

            case 6: quickSort(0, item-1); imprimeTabela(); break;

            case 7: inverso = true; quickSort(0, item-1); imprimeTabela(); break;

            case 8: printf("\n\t\tSaindo...\n"); break;

            case 9: system("cls"); login(); break;

            default: printf("\n\t\topção inválida!\n"); getchar();
        }
    } while (opcao != 8); // repete o menu até que o usuário escolha o número da 'opção sair' 
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

    printf("\n\t\tVeículo: ");
    scanf("%15s", matriz[item][0].elem);
    fflush(stdin);
    x = 1;
    do {
        printf("\n\t\tPlaca: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\tO veículo da linha [%d] já está cadastrado com esta placa! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][1].elem, entrada);
        }
    } while (ondeTem != -1);          // prevenção de erro de digitação/redundância não deixando que
    x = 2;                                        // insira um novo veículo com informações já cadastradas 
    do {                                          
        printf("\n\tChassi: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\tO veículo da linha [%d] já está cadastrado com este chassi! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {   
            strcpy(matriz[item][2].elem, entrada);
        }
    } while (ondeTem != -1);
    x = 3;
    do {
        printf("\n\tRenavan: ");
        scanf("%15s", entrada);
        fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\tO veículo da linha [%d] já está cadastrado com este renavan! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][3].elem, entrada);
        }
    } while (ondeTem != -1);
    printf("\n\t\tAquisição: ");
    scanf("%15s", matriz[item][4].elem);
    fflush(stdin);
    printf("\n\t\tSetor: ");
    scanf("%15s", matriz[item][5].elem);
    fflush(stdin);
}

void menuModificar(int *opcao,int *modificar) { // altera algum campo (dos "alteráveis") de um veículo

    do {
        system("cls");
        printf("\t\tQual o número do veículo você deseja modificar? digite a linha correspondente da tabela: \n");
        scanf("%d",modificar);
        fflush(stdin);
        if(*modificar < 0 || *modificar > item) {
            printf("\n\t\tOpção inválida!\n");
            getchar();
        }
    } while(*modificar < 0 || *modificar > item); // repete enquanto o usuário não digitar uma opção válida (prevenção de erros)

    do {
        system("cls");
        printf("\t\tQual campo você deseja modificar? Digite a opção correspondente: \n");
        printf("\t\t1) Placa\n");
        printf("\t\t2) Setor\n");
        printf("\n\t\t--> ");
        scanf("%d",opcao);
        fflush(stdin);
        if(*opcao < 1 || *opcao > 2) {
            printf("\n\t\tOpção inválida!\n");
            getchar();
        }
    } while(*opcao < 1 || *opcao > 2); // repete o menu enquanto o usuário não digitar uma opção válida
}

void modificarVeiculo() {

    int opcao, modificar; 

    menuModificar(&opcao,&modificar);

    printf("\t\tDigite a nova informação para substituir a anterior: ");

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
    int tam, carac;

    if (item < 0) {
        printf("\n\t\tAinda não há nenhum veículo registrado!\n");
        getchar();
        return;
    }

    system("cls");
    printf("\n     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "VEÍCULO", "PLACA", "CHASSI", "RENAVAN", "AQUISIÇÃO", "SETOR");
    printf("     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "", "", "", "", "", "");

    for (int i = 0; i < item; i++) {
        printf("%4d ", i);  // imprime o número da linha
        for (int j = 0; j < 6; j++) {
            switch (j) {
                case 0: carac = 55; break;
                case 1: carac = 9; break;
                case 2: carac = 19; break;
                case 3: carac = 11; break;
                case 4: carac = 12; break;
                case 5: carac = 55; break;
            }
            printf("|  ");
            for (int z = 0; z < carac; z++) { // imprime o conteúdo de algum elemento da matriz limitando a 'carac' caracteres
                printf("%c", matriz[i][j].elem[z]);
            }
            tam = strlen(matriz[i][j].elem);
            if (tam < carac && j != 5) {
                for (int s = 1; s <= (carac - tam); s++) { // se o campo em questão não tiver 'carac' caracteres completa 
                    printf(" ");                                       // com espaços a fim de manter uma boa formatação
                }
            }
        }
        printf("\n");
    }
    getchar();
}

void pesquisaPorNome() { // quase igual a função imprimeTabela
    int tam, carac, cont = 1;
    char nome[40], nome2[40], nome3[40], variante;

    if (item < 0) {
        printf("\n\t\tAinda não há nenhum veículo registrado!\n");
        getchar();
    }

    printf("\n\t\tDigite o nome do veículo que deseja buscar (em caps lock e sem acentuação): ");
    scanf("%40[^\n]s", nome);
    fflush(stdin);

    printf("\n\t\tExiste alguma variação do nome (ex: sem acento/letra minúscula), digite S/N: ");
    scanf("%c", &variante);
    fflush(stdin);

    if (variante == 's' || variante == 'S') {
        printf("\n\t\tDigite a variante: ");
        scanf("%40[^\n]s", nome2);
        fflush(stdin);

        variante = 'n';

        printf("\n\t\tDeseja adicionar mais uma variante? S/N: ");
        scanf("%c", &variante);
        fflush(stdin);

        if (variante == 's' || variante == 'S') {
            printf("\n\t\tDigite a variante:");
            scanf("%40[^\n]s", nome3);
            fflush(stdin);
        }
    }

    system("cls");
    printf("\n     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "VEÍCULO", "PLACA", "CHASSI", "RENAVAN", "AQUISIÇÃO", "SETOR");
    printf("     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "", "", "", "", "", "");

    for (int i = 0; i < item; i++) {
        if ( strstr(matriz[i][0].elem, nome) || strstr(matriz[i][0].elem, nome2) || strstr(matriz[i][0].elem, nome3) ) { // somente se achar o nome pesquisado imprime a linha da matriz
            printf("%4d ", cont);  // imprime o número da linha
            for (int j = 0; j < 6; j++) {
                switch (j) {
                    case 0: carac = 55; break;
                    case 1: carac = 9; break;
                    case 2: carac = 19; break;
                    case 3: carac = 11; break;
                    case 4: carac = 12; break;
                    case 5: carac = 55; break;
                }
                printf("|  ");
                for (int z = 0; z < carac; z++) { // imprime o conteúdo de algum elemento da matriz limitando a 'carac' caracteres
                    printf("%c", matriz[i][j].elem[z]);
                }
                tam = strlen(matriz[i][j].elem);
                if (tam < carac) {
                    for (int s = 1; s <= (carac - tam); s++) { // se o campo em questão não tiver 'carac' caracteres completa 
                        printf(" ");                                       // com espaços a fim de manter uma boa formatação
                    }
                }
            }
            cont++;
            printf("\n");
        }
    }
    getchar();
}

struct linha {
    char col[255];
} linhaAux[6]; // essa linha auxiliar é usada para fazer a troca de duas linhas da matriz
    
// Função para trocar duas linhas da matriz de posição
void troca(int a, int b) {
    for(int j = 0; j < 6; j++) {
        strncpy(linhaAux[j].col, matriz[a][j].elem, sizeof(linhaAux[j].col)); // aux = a
        linhaAux[j].col[sizeof(linhaAux[j].col) - 1] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
        strncpy(matriz[a][j].elem, matriz[b][j].elem, sizeof(matriz[a][j].elem)); // a = b
        matriz[a][j].elem[sizeof(matriz[a][j].elem) - 1] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
        strncpy(matriz[b][j].elem, linhaAux[j].col, sizeof(matriz[b][j].elem)); // b = aux
        matriz[b][j].elem[sizeof(matriz[b][j].elem) - 1] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
    }
}

// Função para encontrar o pivô e particionar o vetor
int particiona( int inicio, int fim) {
    char pivo[255];
    strcpy(pivo, matriz[fim][0].elem); // escolhendo o último elemento como pivô
    int i = (inicio - 1); // índice do menor elemento

    for (int j = inicio; j < fim; j++) {
        // Se o elemento atual for menor ou igual ao pivô

        if (inverso) {
            if ( strcmp(matriz[j][0].elem, pivo) >= 0 ) { // só troca o sinal
                i++; // incrementa o índice do menor elemento
                troca(i, j);
            }
        }
        else {
            if ( strcmp(matriz[j][0].elem, pivo) <= 0 ) {
                i++; // incrementa o índice do menor elemento
                troca(i, j);
            }
        }
    }
    troca( i + 1, fim);
    return (i + 1);
}

// Função principal que implementa o Quicksort
void quickSort(int inicio, int fim) {
    if (inicio < fim) {
        // Encontra o pivô e particiona o vetor
        int indicePivo = particiona(inicio, fim);

        // Classifica recursivamente as duas metades
        quickSort(inicio, indicePivo - 1);
        quickSort(indicePivo + 1, fim);
    }
}

int main() {
    
    setlocale(LC_ALL, "Portuguese");
    
    bool sucesso;
    sucesso = login();
    if(!sucesso) { // se não logou fecha o programa
        return 0;
    }
    leDoArquivo(); // carrega a tabela com os carros que já foram cadastrados
    menu(); // a partir do menu o usuário realiza todas as funções disponibilizadas pelo programa

    return 0;
}