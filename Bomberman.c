#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
#define MAXSAVES 9

typedef struct
{
    int x, y, vidas, bombas, chaves;
    char frente;
} tipo_jogador;

typedef struct
{
    char tamanho[25][62];
    int largura, altura;
} tipo_mapa;

void reset_jogador(tipo_jogador *jogador);
void imprime(tipo_mapa mapa);
void inicializa_mapa(char nome_arquivo[], tipo_mapa *mapa);
void encontra (tipo_jogador *jogador, char personagem, tipo_mapa mapa);
void acao_jogador(tipo_jogador *jogador, int direcao, char c, tipo_mapa* mapa);
char teclado();
int menu(int inicio, tipo_mapa *mapa, tipo_jogador *jogador, int *num_saves);
void apaga_linha(int x, int y);
void atualizamapa(char item, tipo_mapa *mapa, tipo_jogador *jogador);
void aguarda_teclado();
void status(tipo_jogador *jogador);
void inicializa_bin(char nome_bin[], tipo_jogador *jogador);
void limpa_loads();

int main()
{
    int action, num_saves=0, endgame = 0;
    tipo_jogador jogador = {1,1,3,3,0,'c'}; //INICIALIZA JOGADOR COM VIDAS, BOMBAS, NENHUMA CHAVE E FRENTE DO JOGADOR COMECA PARA CIMA
    char nome_arquivo[30] = "mapa_fase1.txt", c = 'j';
    tipo_mapa mapa;

    textcolor(WHITE);
    inicializa_mapa(nome_arquivo, &mapa);
    encontra(&jogador, c, mapa);
    menu(1, &mapa, &jogador, &num_saves);
    imprime(mapa);
    status(&jogador);

    //printf("\nO jogador se encontra em %d x e %d y\n", jogador.x, jogador.y);

    while(!endgame)
    {
        action = teclado();//getch();
        if(action != 27)
        {
            acao_jogador(&jogador, action, c, &mapa);

        }
        else
        {
            endgame = menu(0, &mapa, &jogador, &num_saves);

        }
    }
    system("pause");
    return 0;
}

void reset_jogador(tipo_jogador *jogador){
    jogador->x = 1;
    jogador->y = 1;
    jogador->vidas = 3;
    jogador->bombas = 3;
    jogador->chaves = 0;
    jogador->frente = 'c';
}

void status(tipo_jogador *jogador){
    gotoxy(1,27);
    textcolor(LIGHTGREEN);
    printf("VIDAS:%d                   BOMBAS:%d                  CHAVES:%d", jogador->vidas, jogador->bombas, jogador->chaves);
    textcolor(WHITE);
}

int menu(int inicio, tipo_mapa *mapa, tipo_jogador *jogador, int *num_saves) /*RETORNA 1 SE JOGO DEVE ACABAR*/{
    FILE *save_mapa, *save_status;
    char c, save_num, nome_save[20] = "save_0.txt", lista_saves[9][20];
    int i, endgame = 0;
    if(inicio)
    {
        textcolor(LIGHTRED);
        printf("\n############################################################");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                         BOMBERMAN                        #");
        printf("\n#                        Beta Version                      #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                                                          #");
        printf("\n#                  Press any key to continue               #");
        printf("\n############################################################");
        textcolor(WHITE);
        i=0;
        do{ //TESTA QUANTOS SAVES JA EXISTEM
            i++;
            save_num = i + '0';
            nome_save[5] = save_num;
            save_mapa = fopen(nome_save, "r");
            if(save_mapa != NULL){
                strcpy(lista_saves[i-1], nome_save); //COPIA O NOME DO SAVE PARA UMA STRING
                (*num_saves)++;
                fclose(save_mapa);
            }
        }while(i<=MAXSAVES);
        aguarda_teclado();
        clrscr();

    }
    else
    {
start:
        gotoxy(1,1);
        textcolor(LIGHTGREEN);
        printf("(N)New Game  (L)Load Game  (S)Save  (Q)Quit Game   (R)Return");
        textcolor(WHITE);

        aguarda_teclado();
        switch(teclado()){
            case 'N':
            case 'n':
                reset_jogador(jogador);
                clrscr();
                inicializa_mapa("mapa_fase1.txt", mapa);
                encontra(jogador, 'j', *mapa);
                imprime(*mapa);
                status(jogador);

                break;

            case 'L':
            case 'l':
                for(i=0; i<=MAXSAVES; i++){
                    save_num = i + '1';
                    nome_save[5] = save_num;
                    save_mapa = fopen(nome_save, "r");
                    if(save_mapa != NULL){
                        strcpy(lista_saves[i], nome_save); //COPIA O NOME DO SAVE PARA UMA STRING
                        fclose(save_mapa);

                    }else{
                        strcpy(lista_saves[i], "EMPTY");

                    }
                }
                if((*num_saves) > 0){
                    textcolor(LIGHTCYAN);
                    for(i=0; i<MAXSAVES; i++){ //IMPRIME A LISTA DE SAVES A DIRETIA DA TELA DO JOGO
                        gotoxy(63, 3*i+1);
                        printf("(%d)%s", i+1, lista_saves[i]);
                    }
                    gotoxy(63, 27);
                    textcolor(WHITE);
                    printf("(0)DELETE SAVE");

                    aguarda_teclado();
                    save_num = teclado();

                    if(save_num == '0'){
                        gotoxy(63, 27);
                        textbackground(RED);
                        printf("(0)DELETE SAVE");
                        textbackground(BLACK);
                        aguarda_teclado();
                        save_num = teclado();

                        if(save_num>'0' && (save_num - '0')<=MAXSAVES){
                            strcpy(nome_save, "save_0.txt");
                            nome_save[5] = save_num;

                            if(remove(nome_save) != 0){
                                gotoxy(1,1);
                                textcolor(LIGHTGREEN);
                                printf("\r                    ERRO AO EXCLUIR MAPA\t\t\t\t");
                                textcolor(WHITE);
                                break;

                            }
                            nome_save[7] = 'b';
                            nome_save[8] = 'i';
                            nome_save[9] = 'n';

                            if(remove(nome_save) != 0){
                                gotoxy(1,1);
                                textcolor(LIGHTGREEN);
                                printf("\r                   ERRO AO EXCLUIR STATUS\t\t\t\t");
                                textcolor(WHITE);
                                break;

                            }
                            nome_save[7] = 't';
                            nome_save[8] = 'x';
                            nome_save[9] = 't';
                            gotoxy(1,1);
                            textcolor(LIGHTGREEN);
                            printf("\r                    %s EXCLUIDO\t\t\t\t", nome_save);
                            textcolor(WHITE);
                            (*num_saves)--;
                            //break;

                        }else{
                            apaga_linha(1,1);
                        }

                    }else if(save_num>'0' && (save_num - '0')<=MAXSAVES){
                        strcpy(nome_save, lista_saves[save_num-'1']);
                        nome_save[5] = save_num;
                        if(nome_save[0] == 'E'){
                            gotoxy(1,1);
                            textcolor(LIGHTGREEN);
                            printf("\r                       SAVE INEXISTENTE\t\t\t\t");
                            textcolor(WHITE);
                            //break;

                        }else{
                            clrscr();
                            inicializa_mapa(nome_save, mapa);
                            encontra(jogador, 'j', *mapa);

                            nome_save[7] = 'b';
                            nome_save[8] = 'i';
                            nome_save[9] = 'n';

                            inicializa_bin(nome_save, jogador);

                            imprime(*mapa);
                            status(jogador);

                            gotoxy(1,1);
                            textcolor(LIGHTGREEN);
                            printf("\r                         %s\t\t\t\t", nome_save);
                            textcolor(WHITE);
                        }

                    }else{
                        apaga_linha(1,1);

                    }

                }else{
                    gotoxy(1,1);
                    textcolor(LIGHTGREEN);
                    printf("\r                       NO SAVED GAMES\t\t\t\t");
                    textcolor(WHITE);

                }
                limpa_loads();
                break;

            case 'S':
            case 's':
                if((*num_saves)<=MAXSAVES){
                    i=0;
                    do{ //TESTA OS SAVES QUE JA EXISTEM
                        i++;
                        save_num = i + '0';
                        nome_save[5] = save_num;
                        save_mapa = fopen(nome_save, "r");
                        if(save_mapa != NULL){
                            fclose(save_mapa);
                        }
                    }while(save_mapa != NULL);

                    save_mapa = fopen(nome_save, "w");
                    nome_save[7] = 'b';
                    nome_save[8] = 'i';
                    nome_save[9] = 'n';
                    save_status = fopen(nome_save, "wb");

                    if(fwrite(jogador, sizeof(tipo_jogador), 1, save_status) != 1){
                        gotoxy(1,1);
                        textcolor(LIGHTRED);
                        printf("ERRO AO SALVAR STATUS DO JOGADOR!");
                        textcolor(WHITE);
                    }

                    for(i=0; i<mapa->altura; i++){
                        fputs(mapa->tamanho[i], save_mapa);

                    }
                    fclose(save_mapa);
                    fclose(save_status);
                    textcolor(LIGHTGREEN);
                    printf("\r               GAME SAVED SUCCESSFULLY! %d LEFT\t\t\t\t\t", MAXSAVES-(*num_saves));
                    textcolor(WHITE);
                    (*num_saves)++;

                }else{

                    gotoxy(1,1);
                    textcolor(LIGHTGREEN);
                    printf("\r                      SAVES AT MAXIMUM\t\t\t\t");
                    textcolor(WHITE);
                }
                break;

            case 'R':
            case 'r':
                apaga_linha(1, 1);
                break;

            case 'Q':
            case 'q':
                textcolor(LIGHTRED);
                printf("\r                        ENDING GAME\t\t\t\t\t\t\n");
                textcolor(WHITE);
                endgame = 1;
                gotoxy(1,28);
                break;

            default:
                goto start;
        }
        if(!endgame){
            gotoxy(jogador->x+1, jogador->y);
        }
    }

    return endgame;
}

void limpa_loads(){
    int i;
    for(i=0; i<10; i++){ //APAGA A LISTA DE SAVES A DIRETIA DA TELA E VOLTA AO JOGO
        apaga_linha(63, 3*i+1);
    }
    gotoxy(63, 27);
    printf("\t\t\t");
}

void inicializa_bin(char nome_bin[], tipo_jogador *jogador){
    FILE *arquivo_bin;

    arquivo_bin = fopen(nome_bin, "rb");
    if(arquivo_bin == NULL){
        textcolor(LIGHTRED);
        gotoxy(1, 1);
        printf("\r                    ERRO AO ABRIR STATUS!\t\t\t\t\t\n");
        textcolor(WHITE);

    }else{
        if(fread(jogador, sizeof(tipo_jogador), 1, arquivo_bin) != 1){
            textcolor(LIGHTRED);
            gotoxy(1, 1);
            printf("\r                    ERRO AO ATUALIZAR STATUS!\t\t\t\t\t\n");
            textcolor(WHITE);
        }
        fclose(arquivo_bin);
    }
}

void aguarda_teclado(){
    while(!kbhit()){}
}

void apaga_linha(int x, int y){
    gotoxy(x, y);
    printf("\t\t\t\t\t\t\t\t");
}

char teclado(){
    char c;
    if(kbhit())
    {
        c = getch();
    }
    //printf("%c", c);
    return c;
}

void inicializa_mapa(char nome_arquivo[], tipo_mapa *mapa){
    int i=0;
    FILE *arquivo_mapa;
    char texto[62];
    arquivo_mapa = fopen(nome_arquivo, "r");
    if(arquivo_mapa == NULL)
    {
        textcolor(LIGHTRED);
        gotoxy(1, 1);
        printf("\r                    ERRO AO ABRIR MAPA!\t\t\t\t\t\n");
        textcolor(WHITE);
    }
    else
    {
        while(!feof(arquivo_mapa))
        {
            fgets(texto, 62, arquivo_mapa);
            strcpy(mapa->tamanho[i], texto);
            i++;
        }
    }

    fclose(arquivo_mapa);
    mapa->largura = strlen(mapa->tamanho[0]) - 1;
    mapa->altura = i;
    //printf("Largura: %d\tAltura: %d", mapa->largura, mapa->altura);
}

void imprime(tipo_mapa mapa){
    int i, j;
    gotoxy(1,2);
    for(i=0; i<mapa.altura; i++){
        for(j=0; j<mapa.largura; j++){
            switch(mapa.tamanho[i][j]){
                case 'w':
                    printf("#");
                    break;

                case 'd':
                    printf("&");
                    break;

                case 'k':
                case 'b':
                    printf("k");
                    break;

                case 'e':
                    printf("i");
                    break;

                default:
                    printf("%c", mapa.tamanho[i][j]);
            }
        }
        printf("\n");
        //printf("%s", mapa.tamanho[i]);

    }

}

void encontra(tipo_jogador *jogador, char personagem, tipo_mapa mapa){
    int i=0, j=0, x, y;
    //for(i=0; i<mapa.altura; i++){
    //for(j=0; j<mapa.largura; j++){
    do
    {
        do
        {
            if(mapa.tamanho[i][j] == personagem)
            {
                x = j+1;//+1 para ficar conforme as coordenadas da tela, que come�a em 1,1
                y = i+2;//+2 para ficar conforme a tela (deslocada 1un para baixo)
                //printf("\t%d %d\n", posicao.x, posicao.y);
                break;
            }
            j++;
        }
        while(j<mapa.largura);
        j=0;
        i++;
    }
    while(i<mapa.altura);
    jogador->x = x;
    jogador->y = y;
}

void acao_jogador(tipo_jogador *jogador, int action, char c, tipo_mapa* mapa){
    int x, y;
    x = jogador->x-1;
    y = jogador->y-2;
    switch (action)
    {
        case 119://W
        case 72: //PARA CIMA
            jogador->frente = 'c';
            if(mapa->tamanho[y-1][x]==' ')   //Comparo com o mapa, mas tiro um de cada para passar da coordenada 1,1 para 0,0. A subtra��o adicional � para ir para a proxima posi��o solicitadoa
            {
                putchxy(jogador->x,jogador->y,' ');
                putchxy(jogador->x,jogador->y-1, c);
                atualizamapa(c, mapa, jogador); //Dentro dessa fun��o nao precisou do & antes de mapa e jogar pois nesse escopo eles j� s�o ponteiros
                jogador->y--;

            }
            break;
        case 115://S
        case 80: //PARA BAIXO
            jogador->frente = 'b';
            if(mapa->tamanho[y+1][x]==' ')
            {
                putchxy(jogador->x,jogador->y,' ');
                putchxy(jogador->x,jogador->y+1,c);
                atualizamapa(c, mapa, jogador);
                jogador->y++;

            }
            break;
        case 97: //A
        case 75: //PARA ESQUERDA
            jogador->frente = 'e';
            if(mapa->tamanho[y][x-1]==' ')
            {
                putchxy(jogador->x,jogador->y,' ');
                putchxy(jogador->x-1,jogador->y,c);
                atualizamapa(c, mapa, jogador);
                jogador->x--;

            }
            break;
        case 100://D
        case 77: //PARA DIREITA
            jogador->frente = 'd';
            if(mapa->tamanho[y][x+1]==' ')
            {
                putchxy(jogador->x,jogador->y,' ');
                putchxy(jogador->x+1,jogador->y,c);
                atualizamapa(c, mapa, jogador);
                jogador->x++;
            }
            break;

        case 'b'://BOMBA
        case 'B':
            if(jogador->bombas>0){
                switch(jogador->frente){
                    case 'c':
                        if(mapa->tamanho[y-1][x]==' '){
                            putchxy(jogador->x, jogador->y-1, '@');
                            jogador->bombas--;
                        }
                        break;

                    case 'b':
                        if(mapa->tamanho[y+1][x]==' '){
                            putchxy(jogador->x, jogador->y+1, '@');
                            jogador->bombas--;
                        }
                        break;

                    case 'e':
                        if(mapa->tamanho[y][x-1]==' '){
                            putchxy(jogador->x-1, jogador->y, '@');
                            jogador->bombas--;
                        }
                        break;

                    case 'd':
                        if(mapa->tamanho[y][x+1]==' '){
                            putchxy(jogador->x+1, jogador->y, '@');
                            jogador->bombas--;
                        }
                        break;

                }
                atualizamapa('@', mapa, jogador);
                status(jogador);
            }
        break;
    }
}

void atualizamapa(char item, tipo_mapa *mapa, tipo_jogador *jogador){
    int x, y;
    x = jogador->x-1;
    y = jogador->y-2;
    mapa->tamanho[y][x]=' ';
    switch(jogador->frente)
    {
    case 'c':
        mapa->tamanho[y-1][x]=item;
        break;

    case 'b':
        mapa->tamanho[y+1][x]=item;
        break;

    case 'e':
        mapa->tamanho[y][x-1]=item;
        break;

    case 'd':
        mapa->tamanho[y][x+1]=item;
        break;
    }
}
