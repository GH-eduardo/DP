#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <unistd.h> //biblioteca para usar a fun��o 'sleep'
#define MAX_USUARIOS 5
#define MAX_TENTATIVAS 3
#define TEMPO_BLOCK 120 // tempo de block caso haja 3 tentativas falhas de login seguidas (em segundos) 
#define LINHAS_DA_MATRIZ 2190

struct USUARIO{
    char username[30];
    char password[20];
    int horario; //vai armazenar o hor�rio do block (quando excede o numero de tentativas de login) ou o hor�rio do ultimo login
} usuarios[MAX_USUARIOS];

struct elemento { // elemento da matriz
    char elem[255]; 
} matriz[LINHAS_DA_MATRIZ+10][6]; // aqui � declarada uma matriz de strings (vetores de caracteres) utilizando uma estrutura
                            
int item = -1;       // a vari�vel item armazena a quantidade de ve�culos cadastrados, � inicializada com -1 porque antes de cadastrar
                            // um ve�culo novo na matriz essa vari�vel � incrementada, sendo assim o primeiro item ocupa a posi��o 0 
                            // se item = 0 h� apenas um ve�culo cadastrado

FILE *arq;

int login();
void atualizaLogin(int quantUsuarios);
void leDoArquivo();
void escreveNoArquivo();
void menu();
int verificaExistencia(int x, char* s); 
void inserirVeiculo();
void menuModificar(); // Declara��o das fun��es na ordem em que est�o dispostas
void modificarVeiculo(int o, int l);
void imprimeTabela();
void pesquisa(char* termo, char* termo2, char* termo3, int col);
void bpNome();
void bpPlaca();
void bpAquisicao();
void bpSetor();
void troca(int a, int b);
void troca2(int a, int b);
int particiona( int inicio, int fim);
void quickSort();
void mensagemOrdenamento();
void ordenaPorIdade();

time_t agora; //usada na fun��o login (escolha2) e tamb�m na fun��o ordenaPorIdade
time_t idadeEmSegundos[LINHAS_DA_MATRIZ+10]; // usado na fun��o particiona caso ordenaPorIdade seja chamada

int login() {

    int num_usuarios = 0, num_tentativas[MAX_USUARIOS], escolha = 0, achou_usuario = 0, i, j;
    char username[30], password[20];
    bool jaExiste;

    arq = fopen("login.txt","r");
    if (arq == NULL) { // Se n�o encontrou o arquivo
        printf("\n\n\t\tArquivo de login n�o encontrado!\n\n"); 
        printf("\t\tGerando Arquivo de login em branco...\n\n"); 
        arq = fopen("login.txt","w");
        sleep(2); // pausa por 2 segundos
    }
    else {
        fscanf(arq, "%d", &num_usuarios);
        for (int a = 0; a < num_usuarios; a++) {
            fscanf(arq, "%s %s %d\n", usuarios[a].username, usuarios[a].password, &usuarios[a].horario);
        }
    }
    fclose(arq);

    for(int x = 0; x < MAX_USUARIOS; x++) {
        num_tentativas[x] = 0; // inicializa todos os usu�rios com as tentativas zeradas
    }
    while (1) {
        system("cls");
        printf("\n\n\n\t\t1. Fazer cadastro\n");
        printf("\t\t2. Fazer login\n");
        printf("\t\t3. Sair\n");
        printf("\n\t\tEscolha uma op��o: ");
        scanf("%d", &escolha); fflush(stdin);

        if (escolha == 1) {
            if (num_usuarios == MAX_USUARIOS) {
                system("cls");
                printf("\n\n\n\t\tN�mero m�ximo de usu�rios atingido");
                sleep(2); // pausa por 2 segundos
            }
            else {
                do {
                    jaExiste = false;
                    printf("\n\t\tDigite um nome de usu�rio de sua prefer�ncia: ");
                    scanf("%s", usuarios[num_usuarios].username); fflush(stdin);
                    for (int x = 0; x < num_usuarios; x++) {
                        if (strcmp(usuarios[num_usuarios].username, usuarios[x].username) == 0) {
                        printf("\n\t\tNome de usu�rio inv�lido! (j� cadastrado)\n");
                        jaExiste = true; }
                    }
                } while (jaExiste);

                printf("\t\tDigite uma senha de sua prefer�ncia: ");
                scanf("%s", usuarios[num_usuarios].password); fflush(stdin);
                usuarios[num_usuarios].horario = time(NULL);
                printf("\n\t\tUsu�rio cadastrado com sucesso!"); sleep(2);
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
                    printf("\n\t\tA conta '%s' est� bloqueada por mais %ld minuto(s) e %ld segundos", usuarios[i].username, ((agora - usuarios[i].horario)*-1)/60, ((agora - usuarios[i].horario)*-1)%60);
                    sleep(2);
                    break;
                }
                else {
                    if (strcmp(usuarios[i].username, username) == 0 && strcmp(usuarios[i].password, password) == 0) { //se o usu�rio e a senha batem
                        system("cls");
                        printf("\n\n\n\t\tLogin realizado com sucesso!");
                        num_tentativas[i] = 0;
                        atualizaLogin(num_usuarios);
                        sleep(2);
                        return 1;
                    }
                }
            }
            if (i == num_usuarios) { // Se percorreu todas as linhas no la�o acima e n�o existe login com o usu�rio e senha correspondente aos digitados
                printf("\n\t\tNome de usu�rio ou senha incorretos!");
                sleep(2);
                for (j = 0; j < num_usuarios; j++) {
                    if (strcmp(usuarios[j].username, username) == 0) {
                        achou_usuario = 1;
                        break;
                    }
                }
                if (achou_usuario) { // se o usu�rio existe a senha � que foi digitada errada
                    num_tentativas[j]++;
                    if (num_tentativas[j] >= MAX_TENTATIVAS) {
                        usuarios[j].horario = time(NULL) + TEMPO_BLOCK;
                        system("cls"); printf("\n\n\n\n\t\tN�mero m�ximo de tentativas atingido. Conta bloqueada por %d minutos!", TEMPO_BLOCK/60); sleep(2); 
                        atualizaLogin(num_usuarios); 
                        return 0;
                    }
                }
            }
        }
        else if (escolha == 3) {
            printf("\n\n\t\tSaindo...\n\n\n");
            atualizaLogin(num_usuarios);
            return 0;
        }
        else {
            system("cls");
            printf("\n\n\n\t\tOp��o inv�lida!"); sleep(1);
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

    if (arq == NULL) // Se n�o encontrou o arquivo
        printf("\n\n\t\tAinda n�o h� nenhum ve�culos registrado/ arquivo n�o encontrado!\n\n"); // sa�da de dados
    else {
        fscanf(arq, "%d", &item); 
        for(int x = 0; x < item; x++) { // percorre o arquivo gravando as informa��es obtidas na matriz 
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
    	printf("\n\n\t\tProblemas na cria��o do arquivo\n");
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
	fprintf(arq,"\n\nPREFEITURA DO MUNICIPIO DE MARINGA\nListagem de Ve�culos Gerado em: 02/06/2023 13:40 com 2190 registros."); fclose(arq);
}

bool antiguidade, inverso, sucesso; // ordenar por antiguidade?, ordenar de forma decrescente? respectivamente

void menu() {

    int  opcao;
   
 do {
        antiguidade = inverso = false; 
        system("cls");
        printf("\n\n\t\tBEM-VINDO AO MENU! POR FAVOR ESCOLHA UMA OP��O:\n\n");
        printf("\t\t1) para inserir um novo ve�culo\n");
        printf("\t\t2) para alterar alguma informa��o de algum ve�culo\n");
        printf("\t\t3) para imprimir a tabela de ve�culos cadastrados\n");
        printf("\t\t4) para salvar dados\n");
        printf("\t\t5) para pesquisar pelo nome do ve�culo\n");
        printf("\t\t6) para pesquisar pela placa do ve�culo\n");
        printf("\t\t7) para pesquisar pela data de aquisi��o do ve�culo\n");
        printf("\t\t8) para filtrar a tabela por algum setor espec�fico\n");
        printf("\t\t9) para ordenar a tabela em ordem alfab�tica (a-z)\n");
        printf("\t\t10) para ordenar a tabela em ordem alfab�tica (z-a)\n");
        printf("\t\t11) para ordenar por ordem de aquisi��o (novo-antigo)\n");
        printf("\t\t12) para ordenar por ordem de aquisi��o (antigo-novo)\n");
        printf("\t\t13) para sair e logar com outra conta\n");
        printf("\t\t14) para sair do programa\n");
        printf("\n\t\t--> ");
        scanf("%d",&opcao); fflush(stdin);

        switch(opcao) {

            case 1: inserirVeiculo(); break;

            case 2: menuModificar(); break;

            case 3: imprimeTabela(); break;

            case 4: escreveNoArquivo(); break;

            case 5: bpNome(); break;

            case 6: bpPlaca(); break;

            case 7: bpAquisicao(); break;

            case 8: bpSetor(); break;

            case 9: quickSort(0, item-1); mensagemOrdenamento(); break;

            case 10: inverso = true; quickSort(0, item-1); mensagemOrdenamento(); break;

            case 11: antiguidade = true; ordenaPorIdade(); quickSort(0, item-1); mensagemOrdenamento(); break;

            case 12: antiguidade = true; inverso = true; ordenaPorIdade(); quickSort(0, item-1); mensagemOrdenamento(); break;

            case 13: sucesso = login(); if(!sucesso) /*se n�o logou fecha o programa*/return; break;

            case 14: printf("\n\t\tSaindo...\n"); break;

            default: printf("\n\t\top��o inv�lida!\n"); getchar();
        }
    } while (opcao != 14); // repete o menu at� que o usu�rio escolha o n�mero da 'op��o sair' 
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
    int ondeTem = -1, x, cancelar = 1;

    printf("\n\t\tTem certeza que deseja inserir um novo ve�culo? digite 1 para continuar e 0 para cancelar: ");
    scanf("%d",&cancelar); fflush(stdin);

    if (cancelar == 0) return; // se cancelou a inser��o do ve�culo termina a fun��o e volta para o menu

    printf("\n\t\tVe�culo: ");
    scanf("%[^\n]s", matriz[item][0].elem); fflush(stdin);
    matriz[item][0].elem[strlen(matriz[item][0].elem)] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
    
    x = 1;
    do {
        printf("\n\t\tPlaca: ");
        scanf("%7s", entrada); fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\t\tO ve�culo da linha [%d] j� est� cadastrado com esta placa! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][1].elem, entrada);
            matriz[item][1].elem[strlen(matriz[item][1].elem)] = '\0'; 
        }
    } while (ondeTem != -1);          // preven��o de erro de digita��o/redund�ncia n�o deixando que
    x = 2;                                        // insira um novo ve�culo com informa��es j� cadastradas 
    do {                                          
        printf("\n\t\tChassi: ");
        scanf("%17s", entrada); fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\t\tO ve�culo da linha [%d] j� est� cadastrado com este chassi! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {   
            strcpy(matriz[item][2].elem, entrada);
            matriz[item][2].elem[strlen(matriz[item][2].elem)] = '\0'; 
        }
    } while (ondeTem != -1);
    x = 3;
    do {
        printf("\n\t\tRenavam: ");
        scanf("%9s", entrada); fflush(stdin);
        ondeTem = verificaExistencia(x,entrada);  
        if (ondeTem != -1) {
            printf("\n\t\tO ve�culo da linha [%d] j� est� cadastrado com este renavam! por favor confira e digite novamente\n",ondeTem);
            getchar();
        }
        else {
            strcpy(matriz[item][3].elem, entrada);
            matriz[item][3].elem[strlen(matriz[item][3].elem)] = '\0'; 
        }
    } while (ondeTem != -1);
    printf("\n\t\tAquisi��o: ");
    scanf("%10s", matriz[item][4].elem); fflush(stdin);
    matriz[item][4].elem[strlen(matriz[item][4].elem)] = '\0'; 

    printf("\n\t\tSetor: ");
    scanf("%[^\n]49s", matriz[item][5].elem); fflush(stdin);
    matriz[item][5].elem[strlen(matriz[item][5].elem)] = '\0'; 

    item++; 
}

void menuModificar() { // altera algum campo (dos "alter�veis") de um ve�culo

    int opcao, modificar; 

    do {
        system("cls");
        printf("\n\n\n\t\tQual campo voc� deseja alterar? Digite a op��o correspondente: \n\n");
        printf("\t\t1) Nome\n");
        printf("\t\t2) Placa\n");
        printf("\t\t3) Setor\n");
        printf("\t\t4) Voltar ao Menu\n");
        printf("\n\t\t--> ");
        scanf("%d", &opcao); fflush(stdin);
        if(opcao < 1 || opcao > 4) {
            printf("\n\t\tOp��o inv�lida!\n");
            getchar();
        }
    } while(opcao < 1 || opcao > 4); // repete o menu enquanto o usu�rio n�o digitar uma op��o v�lida

    if (opcao == 4) return;

    do {
        printf("\n\t\tQual � a linha do ve�culo que voc� deseja alterar: ");
        scanf("%d", &modificar); fflush(stdin);
        if(modificar < 0 || modificar > item) {
            printf("\n\t\tOp��o inv�lida!\n");
            getchar();
        }
    } while(modificar < 0 || modificar > item); // repete enquanto o usu�rio n�o digitar uma op��o v�lida (preven��o de erros)

    modificarVeiculo(opcao, modificar);
}

void modificarVeiculo(int o, int l) { // o = op��o  l = linha

    char campo[10], entrada[255];
    int c, cancelar = 1, ondeTem = -1, tam; // c de coluna

    printf("\n     |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "NOME/DESCRI��O", "PLACA", "CHASSI", "RENAVAM", "AQUISI��O", "SETOR");
    printf("     |  ");
    tam = strlen(matriz[l][0].elem);
    for (int z = 0; z < tam && z < 55; z++) { // imprime o conte�do de algum elemento da matriz limitando a 55 caracteres
        printf("%c", matriz[l][0].elem[z]);
    }
    if (tam < 55) {
        for (int s = 1; s <= (55 - tam); s++) { // se o campo em quest�o n�o tiver 55 caracteres completa 
            printf(" ");                                       // com espa�os a fim de manter uma boa formata��o
        }
    }
    printf("|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", matriz[l][1].elem, matriz[l][2].elem, matriz[l][3].elem, matriz[l][4].elem, matriz[l][5].elem);

    if (o == 1) {
        c = 0;
        strcpy(campo, "NOME/DESCRI��O");
    }
    else if (o == 2) {
        c = 1;
        strcpy(campo, "PLACA");
    }
    else {
        c = 5;
        strcpy(campo, "SETOR");
    }

    printf("\n\t\tTem certeza que deseja substituir %s?\n\n\t\t\"%s\"\n\n\t\tDigite 1 em caso afirmativo ou 0 para cancelar: ", campo, matriz[l][c].elem);
    scanf("%d", &cancelar); fflush(stdin);
    if (cancelar == 0) return;

    do {
        printf("\n\t\tDigite o(a) novo(a) %s: ", campo);
        scanf("%[^\n]s", entrada); fflush(stdin); // modifica o campo escolhido
        if (c == 1) {ondeTem = verificaExistencia(c,entrada); }
            if (ondeTem != -1) {
                printf("\n\t\tO ve�culo da linha [%d] j� est� cadastrado com esta informa��o! por favor confira e digite novamente\n",ondeTem);
                getchar();
            }
            else {
                strcpy(matriz[l][c].elem, entrada);
                tam = strlen(matriz[l][c].elem);
                matriz[l][c].elem[tam] = '\0';
            }
    } while (ondeTem != -1);          // preven��o de erro de digita��o/redund�ncia n�o deixando que

    menuModificar();
}

void imprimeTabela() {

    int tam, carac, verTudo;

    if (item < 0) {
        printf("\n\t\tAinda n�o h� nenhum ve�culo registrado!\n");
        getchar();
        return;
    }

    system("cls");
    printf("\n       |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "NOME/DESCRI��O", "PLACA", "CHASSI", "RENAVAM", "AQUISI��O", "SETOR");
    printf("       |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "", "", "", "", "", "");

    for (int i = 0; i < item; i++) {
        printf("%6d ", i);  // imprime o n�mero da linha
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
            tam = strlen(matriz[i][j].elem);
            for (int z = 0; z < tam && z < carac; z++) { // imprime o conte�do de algum elemento da matriz limitando a 'carac' caracteres
                printf("%c", matriz[i][j].elem[z]);
            }
            if (tam < carac && j != 5) {
                for (int s = 1; s <= (carac - tam); s++) { // se o campo em quest�o n�o tiver 'carac' caracteres completa 
                    printf(" ");                                       // com espa�os a fim de manter uma boa formata��o
                }
            }
        }
        printf("\n");
    }
    do {
        printf("\n\t\tDeseja ver o nome completo de algum ve�culo? se sim digite a linha do ve�culo, se n�o digite -1: ");
        scanf("%d", &verTudo);
        if (verTudo >= 0 && verTudo < item) printf("\n\t\t%s\n", matriz[verTudo][0].elem);
    } while (verTudo >= 0 && verTudo < item);
}

void pesquisa(char* termo, char* termo2, char* termo3, int col) { // quase igual a fun��o imprimeTabela

    int carac, cont = 0, tam, verTudo;

    system("cls");
    printf("\n       |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "NOME/DESCRI��O", "PLACA", "CHASSI", "RENAVAM", "AQUISI��O", "SETOR");
    printf("       |  %-55s|  %-9s|  %-19s|  %-11s|  %-12s|  %-51s\n", "", "", "", "", "", "");

    for (int i = 0; i < item; i++) {
        if ( strstr(matriz[i][col].elem, termo) || strstr(matriz[i][col].elem, termo2) || strstr(matriz[i][col].elem, termo3) ) { 
            // somente se achar o nome/placa pesquisado(a) imprime a linha da matriz
            printf("%6d ", i);  // imprime o n�mero da linha
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
                tam = strlen(matriz[i][j].elem);
                for (int z = 0; z < tam && z < carac; z++) { // imprime o conte�do de algum elemento da matriz limitando a 'carac' caracteres
                    printf("%c", matriz[i][j].elem[z]);
                }
                if (tam < carac && j != 5) {
                    for (int s = 1; s <= (carac - tam); s++) { // se o campo em quest�o n�o tiver 'carac' caracteres completa 
                        printf(" ");                                       // com espa�os a fim de manter uma boa formata��o
                    }
                }
            }
            cont++; printf("\n");
        }
    }
    printf("\n\t\tForam encontrados %d ve�culos de acordo com o que foi digitado!\n", cont);
    do {
        printf("\n\t\tDeseja ver o nome completo de algum ve�culo? se sim digite a linha do ve�culo, se n�o digite -1: ");
        scanf("%d", &verTudo);
        if (verTudo >= 0 && verTudo < item) printf("\n\t\t%s\n", matriz[verTudo][0].elem);
    } while (verTudo >= 0 && verTudo < item);
}

void bpNome() {

    char nome[40] = {"zzzzzzzzzzzzzzz"}, nome2[40] = {"zzzzzzzzzzzzzzz"}, nome3[40] = {"zzzzzzzzzzzzzzz"}, variante;

    system("cls");
    printf("\n\n\n\n\t\tDigite o nome do ve�culo que deseja buscar (em caps lock e sem acentua��o): ");
    scanf("%40[^\n]s", nome); fflush(stdin);

    printf("\n\t\tDeseja buscar por outro nome/termo? digite S/N: ");
    scanf("%c", &variante); fflush(stdin);

    if (variante == 's' || variante == 'S') {
        printf("\n\t\tDigite a o outro nome/termo: ");
        scanf("%40[^\n]s", nome2); fflush(stdin);

        variante = 'n';

        printf("\n\t\tDeseja adicionar mais um nome a se buscar? S/N: ");
        scanf("%c", &variante); fflush(stdin);

        if (variante == 's' || variante == 'S') {
            printf("\n\t\tDigite o outro nome/termo: ");
            scanf("%40[^\n]s", nome3); fflush(stdin);
        }
    }
    pesquisa(nome, nome2, nome3, 0);
}

void bpPlaca() {

    char placa[7], placa2[7] = "zzzzzzz", placa3[7] = "zzzzzzz";

    system("cls");
    printf("\n\n\n\n\t\tDigite a placa do ve�culo que deseja buscar (em caps lock): ");
    scanf("%7s", placa); fflush(stdin);

    pesquisa(placa, placa2, placa3, 1);
}

void bpAquisicao() {

    char data[10], data2[10] = "zzzzzzzzzz", data3[10] = "zzzzzzzzzz", variante;

    system("cls");
    printf("\n\n\n\n\t\tDigite a data de aquisi��o do ve�culo que deseja buscar: ");
    scanf("%10s", data); fflush(stdin);

    printf("\n\t\tDeseja buscar por outra data? digite S/N: ");
    scanf("%c", &variante); fflush(stdin);

    if (variante == 's' || variante == 'S') {
        printf("\n\t\tDigite a outra data: ");
        scanf("%10s", data2); fflush(stdin);

        variante = 'n';

        printf("\n\t\tDeseja adicionar mais uma data a se buscar? S/N: ");
        scanf("%c", &variante); fflush(stdin);

        if (variante == 's' || variante == 'S') {
            printf("\n\t\tDigite a outra data: ");
            scanf("%10s", data3); fflush(stdin);
        }
    }
    pesquisa(data, data2, data3, 4);
}

void bpSetor() {

    char setor[40], setor2[40] = "zzzzzzzzzz", setor3[40] = "zzzzzzzzzz";

    system("cls");
    printf("\n\n\n\n\t\tFiltrar pelo setor: ");
    scanf("%40[^\n]s", setor); fflush(stdin);

    pesquisa(setor, setor2, setor3, 5);
}

struct linha {
    char col[255];
} linhaAux[6]; // essa linha auxiliar � usada para fazer a troca de duas linhas da matriz
    
void troca(int a, int b) {// Fun��o para trocar duas linhas da matriz de posi��o 

    for(int j = 0; j < 6; j++) {
        strncpy(linhaAux[j].col, matriz[a][j].elem, sizeof(linhaAux[j].col)); // aux = a
        linhaAux[j].col[strlen(linhaAux[j].col)] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
        strncpy(matriz[a][j].elem, matriz[b][j].elem, sizeof(matriz[a][j].elem)); // a = b
        matriz[a][j].elem[strlen(matriz[a][j].elem)] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
        strncpy(matriz[b][j].elem, linhaAux[j].col, sizeof(matriz[b][j].elem)); // b = aux
        matriz[b][j].elem[strlen(matriz[b][j].elem)] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente
    }
}

void troca2(int a, int b) { // Fun��o para trocar duas linhas do vetor paralelo
    
    time_t aux;

    aux = idadeEmSegundos[a];
    idadeEmSegundos[a] = idadeEmSegundos[b];
    idadeEmSegundos[b] = aux;
}

int particiona( int inicio, int fim) { // Fun��o para encontrar o piv� e particionar o vetor

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

void quickSort(int inicio, int fim) { // Fun��o principal que implementa o Quicksort

    if (inicio < fim) {
        int indicePivo = particiona(inicio, fim); // Encontra o piv� e particiona o vetor
        quickSort(inicio, indicePivo - 1); // Classifica recursivamente as duas metades
        quickSort(indicePivo + 1, fim);
    }
}

void mensagemOrdenamento() {

    system("cls");
    if (antiguidade && inverso) {
        printf("\n\n\n\t\tTabela ordenada do ve�culo mais antigo para o mais novo!\n");
    }
    else if (antiguidade && !inverso) {
        printf("\n\n\n\t\tTabela ordenada do ve�culo mais novo para o mais antigo!\n");
    }
    else if (inverso && !antiguidade) {
        printf("\n\n\n\t\tTabela em ordem alfab�tica de Z - A!\n");
    }
    else {
        printf("\n\n\n\t\tTabela em ordem alfab�tica de A - Z!\n");
    }
    sleep(2);
}

void ordenaPorIdade() {
    
    struct tm aquisicao;
    int dia, mes, ano;
    char *ponteiro, cDia[3], cMes[3], cAno[5], strAux[12];

    time(&agora);
    aquisicao = *localtime(&agora); // isso registra o dia de hoje no formato: DD/MM/AAAA HH:SS, ent�o temos que adaptar para a data de aquisi��o do ve�culo

    for(int i = 0; i < item; i++) {
    
        strncpy(strAux, matriz[i][4].elem, sizeof(strAux)); // armazena o valor da original pois a fun��o strtok vai bagun�ar com ela
        strAux[strlen(strAux)] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente

        ponteiro = strtok(matriz[i][4].elem, "/"); // a fun��o strtok permite dividir uma string maior em "tokens" que s�o partes menores (substrings)
        strcpy(cDia,ponteiro);
        ponteiro = strtok(NULL, "/");
        strcpy(cMes,ponteiro);
        ponteiro = strtok(NULL, ";");
        strcpy(cAno,ponteiro);
        strncpy(matriz[i][4].elem, strAux, sizeof(matriz[i][4].elem)); // devolve o valor original
        matriz[i][4].elem[strlen(matriz[i][4].elem)] = '\0'; // Certifica-se de que a string de destino seja terminada corretamente

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
    
    sucesso = login();
    if(!sucesso) { // se n�o logou fecha o programa
        return 0;
    }
    leDoArquivo(); // carrega a tabela com os carros que j� foram cadastrados
    menu(); // a partir do menu o usu�rio realiza todas as fun��es disponibilizadas pelo programa

    return 0;
}