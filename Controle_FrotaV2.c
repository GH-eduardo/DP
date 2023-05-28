#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <unistd.h> //biblioteca para usar a fun��o 'sleep'
#define MAX_USUARIOS 5
#define MAX_TENTATIVAS 3
#define TEMPO_DE_ESPERA 120 // tempo de block caso haja 3 tentativas falhas de login seguidas (em segundos) 
#define CARAC 

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
} matriz[900][6]; // aqui � declarada uma matriz de strings (vetores de caracteres) utilizando uma estrutura
                            // checklist 2/6: struct e vetores e matrizes
int item = -1;       // a vari�vel item armazena a quantidade de ve�culos cadastrados, � inicializada com -1 porque antes de cadastrar
                            // um ve�culo novo na matriz essa vari�vel � incrementada, sendo assim o primeiro item ocupa a posi��o 0 
                            // se item = 0 h� apenas um ve�culo cadastrado checklist 3/6: vari�vel

FILE *arq;

int login();
void atualizaLogin(int quantUsuarios);
void leDoArquivo();
void escreveNoArquivo();
void menu();
int verificaExistencia(int x, char* s); 
void inserirVeiculo();
void menuModificar(int *opcao,int *modificar); // Declara��o das fun��es na ordem em que est�o dispostas
void modificarVeiculo();
void imprimeTabela();
void pesquisa();
void troca(int a, int b);
void troca2(int a, int b);
int particiona( int inicio, int fim);
void quickSort();
void ordenaPorIdade();

time_t agora; //usada na fun��o login (escolha2) e tamb�m na fun��o ordenaPorIdade
time_t idadeEmSegundos[847]; // usado na fun��o particiona caso ordenaPorIdade seja chamada

int login() {

    int num_usuarios = 0, num_tentativas[num_usuarios], escolha = 0, achou_usuario = 0, i, j;
    char username[30], password[20];

    arq = fopen("login.txt","r");
    if (arq == NULL) { // Se n�o encontrou o arquivo
        printf("\t\tArquivo de login n�o encontrado!\n\n"); // sa�da de dados
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
        num_tentativas[x] = 0; // inicializa todos os usu�rios com as tentativas zeradas
    }
    while (1) {
        system("cls");
        printf("\n\n\n\t\t1. Cadastrar-se\n");
        printf("\t\t2. Fazer login\n");
        printf("\t\t3. Sair\n");
        printf("\n\t\tEscolha uma op��o: ");
        scanf("%d", &escolha); fflush(stdin);

        if (escolha == 1) {
            if (num_usuarios == MAX_USUARIOS) {
                system("cls");
                printf("\n\n\n\t\tN�mero m�ximo de usu�rios atingido");
                sleep(2);
            }
            else {
                printf("\n\t\tDigite um nome de usu�rio de sua prefer�ncia: ");
                scanf("%s", usuarios[num_usuarios].username); fflush(stdin);
                printf("\t\tDigite uma senha de sua prefer�ncia: ");
                scanf("%s", usuarios[num_usuarios].password); fflush(stdin);
                usuarios[num_usuarios].horario = time(NULL);
                printf("\n\t\tUsu�rio cadastrado com sucesso!\n");
                num_usuarios++;
            }
        }
        else if (escolha == 2) {

            printf("\n\t\tDigite o nome de usu�rio: ");
            scanf("%s", username); fflush(stdin);
            printf("\t\tDigite a senha: ");
            scanf("%s", password); fflush(stdin);

            for (i = 0; i < num_usuarios; i++) { //percorre todas as linhas (todos os usu�rios)
                agora = time(NULL);
                if (strcmp(usuarios[i].username, username) == 0 /*Se o usu�rio existe*/ && agora - usuarios[i].horario < 0 /*Se est� bloqueado*/) {
                    printf("\n\t\tA conta '%s' est� bloqueada por mais %ld segundos", usuarios[i].username, (agora - usuarios[i].horario)*-1);
                    sleep(2);
                    break;
                }
                else {
                    if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) { //se o usu�rio e a senha batem
                        system("cls");
                        printf("\n\n\n\t\tLogin realizado com sucesso!");
                        num_tentativas[i] = 0;
                        atualizaLogin(num_usuarios);
                        sleep(1); usleep(400000);//pausa por 1,4 segundos
                        return 1;
                    }
                }
            }
            if (i == num_usuarios) { // Se percorreu todas as linhas no la�o acima e n�o existe login com o usu�rio e senha correspondente aos digitados
                printf("\n\t\tNome de usu�rio ou senha incorretos!");
                sleep(1); usleep(400000);//pausa por 1,4 segundos
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
                        printf("\n\t\tN�mero m�ximo de tentativas atingido. Conta bloqueada por %d minutos.\n", TEMPO_DE_ESPERA/60);
                        sleep(2);
                    }
                }
            }
        }
        else if (escolha == 3) {
            printf("\n\n\t\tSaindo...\n");
            atualizaLogin(num_usuarios);
            return 0;
        }
        else {
            system("cls");
            printf("\n\n\n\t\tOp��o inv�lida!");
            sleep(1);
        }
    }
}

void atualizaLogin(int quantUsuarios) {
    arq = fopen("login.txt", "w"); // grava��o
    fprintf(arq,"%d\n", quantUsuarios);
    for (int x = 0; x < quantUsuarios; x++) {
        fprintf(arq,"%s %s %d", usuarios[x].username, usuarios[x].password, usuarios[x].horario);
        if (x != quantUsuarios-1) fprintf(arq,"\n");
    }
    fclose(arq);
}

void leDoArquivo() { // carrega a tabela com os carros que j� foram cadastrados e est�o salvos em um arquivo txt

    char bigString[341], *ponteiro;

    arq = fopen("Listagem_de_Ve�culos.txt", "r"); // leitura
    if (arq == NULL) // Se n�o encontrou o arquivo // checklist 4/6: desvios condicionais
        printf("\t\tAinda n�o h� nenhum ve�culos registrado/ arquivo n�o encontrado!\n\n"); // sa�da de dados
    else {

        fscanf(arq, "%d", &item); // entrada de dados (de um arquivo) // checklist 5/6: comandos de entrada e sa�da

        for(int x = 0; x < item; x++) { // percorre o arquivo gravando as informa��es obtidas na matriz // checklist 6/6: la�os de repeti��o
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
    	printf("\t\tProblemas na cria��o do arquivo\n");
	else  {

        fprintf(arq, "%d\n", item);
        for(int x = 0; x < item; x++) {
            for(int y = 0; y < 6; y++) {

                fprintf(arq, "%s", matriz[x][y].elem);
                fprintf(arq,";");
            }
            if (x != item-1) fprintf(arq,"\n");
        }
    }
	fclose(arq);
}

bool antiguidade, inverso, placa; // ordenar por antiguidade?, ordenar de forma decrescente? e pesquisar pela placa? respectivamente

void menu() {

    int  opcao;
   
 do {
        antiguidade = inverso = placa = false; 
        system("cls");
        printf("\n\n\t\tBEM-VINDO AO MENU! POR FAVOR ESCOLHA UMA OP��O:\n\n");
        printf("\t\t1) para inserir um novo ve�culo\n");
        printf("\t\t2) para modificar alguma informa��o de algum ve�culo\n");
        printf("\t\t3) para imprimir a tabela de ve�culos cadastrados\n");
        printf("\t\t4) para salvar dados\n");
        printf("\t\t5) para pesquisar pelo nome do ve�culo\n");
        printf("\t\t6) para pesquisar pela placa do ve�culo\n");
        printf("\t\t7) para ordenar a tabela em ordem alfab�tica (a-z)\n");
        printf("\t\t8) para ordenar a tabela em ordem alfab�tica (z-a)\n");
        printf("\t\t9) para ordenar por ordem de aquisi��o (novo-antigo)\n");
        printf("\t\t10) para ordenar por ordem de aquisi��o (antigo-novo)\n");
        printf("\t\t11) para sair e logar com outra conta\n");
        printf("\t\t12) para sair do programa\n");
        printf("\n\t\t--> ");
        scanf("%d",&opcao); fflush(stdin);

        switch(opcao) {

            case 1: inserirVeiculo(); break;

            case 2: modificarVeiculo(); break;

            case 3: imprimeTabela(); break;

            case 4: escreveNoArquivo(); break;

            case 5: pesquisa(); break;

            case 6: placa = true; pesquisa(); break;

            case 7: quickSort(0, item-1); imprimeTabela(); break;

            case 8: inverso = true; quickSort(0, item-1); imprimeTabela(); break;

            case 9: antiguidade = true; ordenaPorIdade(); quickSort(0, item-1); imprimeTabela(); break;

            case 10: antiguidade = true; inverso = true; ordenaPorIdade(); quickSort(0, item-1); imprimeTabela(); break;

            case 11: system("cls"); login(); break;

            case 12: printf("\n\t\tSaindo...\n"); break;

            default: printf("\n\t\top��o inv�lida!\n"); getchar();
        }
    } while (opcao != 12); // repete o menu at� que o usu�rio escolha o n�mero da 'op��o sair' 
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

    char entrada[255];
    int ondeTem = -1, x;

    printf("\n\t\tVe�culo: ");
    scanf("%[^\n]53s", matriz[item][0].elem); fflush(stdin);
    matriz[item][0].elem[sizeof(matriz[item][0].elem) - 1] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
    
    x = 1;
    do {
        printf("\n\t\tPlaca: ");
        scanf("%7s", entrada); fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\tO ve�culo da linha [%d] j� est� cadastrado com esta placa! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][1].elem, entrada);
            matriz[item][1].elem[sizeof(matriz[item][1].elem) - 1] = '\0'; 
        }
    } while (ondeTem != -1);          // preven��o de erro de digita��o/redund�ncia n�o deixando que
    x = 2;                                        // insira um novo ve�culo com informa��es j� cadastradas 
    do {                                          
        printf("\n\t\tChassi: ");
        scanf("%17s", entrada); fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\tO ve�culo da linha [%d] j� est� cadastrado com este chassi! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {   
            strcpy(matriz[item][2].elem, entrada);
            matriz[item][2].elem[sizeof(matriz[item][2].elem) - 1] = '\0'; 
        }
    } while (ondeTem != -1);
    x = 3;
    do {
        printf("\n\t\tRenavan: ");
        scanf("%9s", entrada); fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\tO ve�culo da linha [%d] j� est� cadastrado com este renavan! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][3].elem, entrada);
            matriz[item][3].elem[sizeof(matriz[item][3].elem) - 1] = '\0'; 
        }
    } while (ondeTem != -1);
    printf("\n\t\tAquisi��o: ");
    scanf("%10s", matriz[item][4].elem); fflush(stdin);
    matriz[item][4].elem[sizeof(matriz[item][4].elem) - 1] = '\0'; 

    printf("\n\t\tSetor: ");
    scanf("%[^\n]49s", matriz[item][5].elem); fflush(stdin);
    matriz[item][5].elem[sizeof(matriz[item][5].elem) - 1] = '\0'; 

    item++; 
}

void menuModificar(int *opcao,int *modificar) { // altera algum campo (dos "alter�veis") de um ve�culo

    do {
        system("cls");
        printf("\n\n\t\tQual o n�mero do ve�culo voc� deseja modificar? digite a linha correspondente da tabela: ");
        scanf("%d",modificar); fflush(stdin);
        if(*modificar < 0 || *modificar > item) {
            printf("\n\t\tOp��o inv�lida!\n");
            getchar();
        }
    } while(*modificar < 0 || *modificar > item); // repete enquanto o usu�rio n�o digitar uma op��o v�lida (preven��o de erros)

    do {
        system("cls");
        printf("\n\n\t\tQual campo voc� deseja modificar? Digite a op��o correspondente: \n");
        printf("\n\t\t1) Placa\n");
        printf("\t\t2) Setor\n");
        printf("\n\t\t--> ");
        scanf("%d",opcao); fflush(stdin);
        if(*opcao < 1 || *opcao > 2) {
            printf("\n\t\tOp��o inv�lida!\n");
            getchar();
        }
    } while(*opcao < 1 || *opcao > 2); // repete o menu enquanto o usu�rio n�o digitar uma op��o v�lida
}

void modificarVeiculo() {

    int opcao, modificar; 

    menuModificar(&opcao,&modificar);

    printf("\n\t\tDigite a nova informa��o para substituir a anterior: ");

    switch(opcao) {
        case 1: scanf("%7s", matriz[modificar][1].elem); fflush(stdin); // modificar a placa
            matriz[modificar][1].elem[sizeof(matriz[modificar][1].elem) - 1] = '\0'; break; 
        case 2: scanf("%49[^\n]s", matriz[modificar][5].elem); fflush(stdin); // modificar o setor
            matriz[modificar][5].elem[sizeof(matriz[modificar][5].elem) - 1] = '\0'; break; 
    }
}

void imprimeTabela() {
    int tam, carac;

    if (item < 0) {
        printf("\n\t\tAinda n�o h� nenhum ve�culo registrado!\n");
        getchar();
        return;
    }

    system("cls");
    printf("\n     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "VE�CULO", "PLACA", "CHASSI", "RENAVAN", "AQUISI��O", "SETOR");
    printf("     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "", "", "", "", "", "");

    for (int i = 0; i < item; i++) {
        printf("%4d ", i);  // imprime o n�mero da linha
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
            for (int z = 0; z < carac; z++) { // imprime o conte�do de algum elemento da matriz limitando a 'carac' caracteres
                printf("%c", matriz[i][j].elem[z]);
            }
            tam = strlen(matriz[i][j].elem);
            if (tam < carac && j != 5) {
                for (int s = 1; s <= (carac - tam); s++) { // se o campo em quest�o n�o tiver 'carac' caracteres completa 
                    printf(" ");                                       // com espa�os a fim de manter uma boa formata��o
                }
            }
        }
        printf("\n");
    }
    getchar();
}

void pesquisa() { // quase igual a fun��o imprimeTabela
    int tam, carac, cont = 1;
    char nome[40] = {"zzzzzzzzzzzzzzzz"}, nome2[40] = {"zzzzzzzzzzzzzzzzzzzzz"}, nome3[40] = {"zzzzzzzzzzzzzzzzz"}, variante;

    if (item < 0) {
        printf("\n\t\tAinda n�o h� nenhum ve�culo registrado!\n");
        getchar();
    }

    if (placa) {
        printf("\n\t\tDigite a placa do ve�culo que deseja buscar (em caps lock): ");
        scanf("%40[^\n]s", nome); fflush(stdin);
    }
    else {
        printf("\n\t\tDigite o nome do ve�culo que deseja buscar (em caps lock e sem acentua��o): ");
        scanf("%40[^\n]s", nome); fflush(stdin);

        printf("\n\t\tExiste alguma varia��o do nome (ex: sem acento/letra min�scula), digite S/N: ");
        scanf("%c", &variante); fflush(stdin);

        if (variante == 's' || variante == 'S') {
            printf("\n\t\tDigite a variante: ");
            scanf("%40[^\n]s", nome2); fflush(stdin);

            variante = 'n';

            printf("\n\t\tDeseja adicionar mais uma variante? S/N: ");
            scanf("%c", &variante); fflush(stdin);

            if (variante == 's' || variante == 'S') {
                printf("\n\t\tDigite a variante:");
                scanf("%40[^\n]s", nome3); fflush(stdin);
            }
        }
    }

    system("cls");
    printf("\n     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "VE�CULO", "PLACA", "CHASSI", "RENAVAN", "AQUISI��O", "SETOR");
    printf("     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "", "", "", "", "", "");

    for (int i = 0; i < item; i++) {
        if ( strstr(matriz[i][0].elem, nome) || strstr(matriz[i][0].elem, nome2) || strstr(matriz[i][0].elem, nome3) || strstr(matriz[i][1].elem, nome) ) { 
            // somente se achar o nome/placa pesquisado(a) imprime a linha da matriz
            printf("%4d ", cont);  // imprime o n�mero da linha
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
                for (int z = 0; z < carac; z++) { // imprime o conte�do de algum elemento da matriz limitando a 'carac' caracteres
                    printf("%c", matriz[i][j].elem[z]);
                }
                tam = strlen(matriz[i][j].elem);
                if (tam < carac) {
                    for (int s = 1; s <= (carac - tam); s++) { // se o campo em quest�o n�o tiver 'carac' caracteres completa 
                        printf(" ");                                       // com espa�os a fim de manter uma boa formata��o
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
} linhaAux[6]; // essa linha auxiliar � usada para fazer a troca de duas linhas da matriz
    
// Fun��o para trocar duas linhas da matriz de posi��o
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

// Fun��o para trocar duas linhas do vetor paralelo
void troca2(int a, int b) {
    time_t aux;

    aux = idadeEmSegundos[a];
    idadeEmSegundos[a] = idadeEmSegundos[b];
    idadeEmSegundos[b] = aux;
}

// Fun��o para encontrar o piv� e particionar o vetor
int particiona( int inicio, int fim) {
    char pivo[255];
    strcpy(pivo, matriz[fim][0].elem); // escolhendo o �ltimo elemento como piv�
    time_t PIVO;
    PIVO = idadeEmSegundos[fim];
    int i = (inicio - 1); // �ndice do menor elemento

    for (int j = inicio; j < fim; j++) {
        if (antiguidade) {
            if (inverso) {
                if ( idadeEmSegundos[j] >= PIVO ) { // s� troca o sinal
                    i++; // incrementa o �ndice do menor elemento
                    if (i != j) {
                        troca(i, j);
                        troca2(i, j);
                    }
                }
            }
            else {
                if ( idadeEmSegundos[j] <= PIVO ) { // Se o elemento atual for menor ou igual ao piv�
                    i++; // incrementa o �ndice do menor elemento
                    if (i != j) {
                        troca(i, j);
                        troca2(i, j);
                    }
                }
            }
        }
        else {
            if (inverso) {
                if ( strcmp(matriz[j][0].elem, pivo) >= 0 ) { // s� troca o sinal
                    i++; // incrementa o �ndice do menor elemento
                    if (i != j) {
                        troca(i, j);
                    }
                }
            }
            else {
                if ( strcmp(matriz[j][0].elem, pivo) <= 0 ) { // Se o elemento atual for menor ou igual ao piv�
                    i++; // incrementa o �ndice do menor elemento
                    if (i != j) {
                        troca(i, j);
                    }
                }
            }
        }
    }
    troca(i+1, fim);
    if (antiguidade) {
        troca2(i+1, fim); }
    return (i + 1);
}

// Fun��o principal que implementa o Quicksort
void quickSort(int inicio, int fim) {
    if (inicio < fim) {
        // Encontra o piv� e particiona o vetor
        int indicePivo = particiona(inicio, fim);

        // Classifica recursivamente as duas metades
        quickSort(inicio, indicePivo - 1);
        quickSort(indicePivo + 1, fim);
    }
}

void ordenaPorIdade() {
    
    struct tm aquisicao;
    int dia, mes, ano;
    char *ponteiro, cDia[3], cMes[3], cAno[5], strAux[12];

    time(&agora);
    aquisicao = *localtime(&agora); // isso registra o dia de hoje no formato: DD/MM/AAAA HH:SS, ent�o temos que adaptar para a data de aquisi��o do ve�culo

    for(int i = 0; i < item; i++) {
    
        strncpy(strAux, matriz[i][4].elem, sizeof(strAux)); // armazena o valor da original pois a fun��o strtok vai bagun�ar com ela
        strAux[sizeof(strAux) - 1] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente

        ponteiro = strtok(matriz[i][4].elem, "/"); // a fun��o strtok permite dividir uma string maior em "tokens" que s�o partes menores (substrings)
        strcpy(cDia,ponteiro);
        ponteiro = strtok(NULL, "/");
        strcpy(cMes,ponteiro);
        ponteiro = strtok(NULL, ";");
        strcpy(cAno,ponteiro);
        strncpy(matriz[i][4].elem, strAux, sizeof(matriz[i][4].elem)); // devolve o valor original
        matriz[i][4].elem[sizeof(matriz[i][4].elem) - 1] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente

        dia = atoi(cDia); 
        mes = atoi(cMes); // transforma as strings para inteiros
        ano = atoi(cAno); 

        aquisicao.tm_mday = dia;
        aquisicao.tm_mon = mes-1; // adaptando para a data em que o ve�culo foi adquirido efetivamente
        aquisicao.tm_year = ano-1900;

        idadeEmSegundos[i] = (agora - mktime(&aquisicao)); // transforma a 'struct tm' 'aquisi��o' em segundos, depois subtrai a data
                                                                                              // atual da data de aquisi��o, obtendo assim a respectiva idade.
                                                                                              // o vetor termina com todas as idades e assim conseguimos ordena-lo
    }
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