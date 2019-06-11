#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define MAXSAVES 9
#define MAXEN 5
#define VELOCIDADE 100
#define T_BOMBA 3000
#define VAZIO 32 //32=ESPAÇO 88=X
#define ENEMY 73 //I
typedef struct{
    int x, y, vidas, bombas, chaves;
    char frente, caractere;
    int direcao;
} tipo_jogador;

typedef struct{
    char tamanho[25][62];
    int largura, altura;
} tipo_mapa;

typedef struct{
    int mx, my, tx, ty, acionada;
    clock_t start;
} tipo_bomba;

void reset(tipo_jogador *jogador);
void imprime(tipo_mapa mapa);
void inicializa_mapa(char nome_arquivo[], tipo_mapa *mapa);
void encontra (tipo_jogador *jogador, char personagem, tipo_mapa mapa, int quant);
void acao_jogador(tipo_jogador *jogador, tipo_bomba bombas[], int action, char c, tipo_mapa* mapa);
char teclado();
void menu(int inicio, tipo_mapa *mapa, tipo_jogador *jogador, tipo_jogador inimigos[], int *endgame);
void apaga_linha(int x, int y);
void atualizamapa(char item, tipo_mapa *mapa, tipo_jogador *jogador);
void aguarda_teclado();
void status(tipo_jogador *jogador);
void inicializa_bin(char nome_bin[], tipo_jogador *jogador);
void limpa_loads();
void inicializa_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa);
void movimento_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa, tipo_jogador *jogador, int *endgame/*, int primeirorun*/);
int conta_tempo_inimigos(clock_t start);
int conta_tempo_bomba(clock_t start);
void explode_bomba(tipo_mapa *mapa, tipo_jogador *jogador, tipo_bomba bombas[], int aciona);
void HideCursor();

int main(){
    int action, num_saves=0, endgame = 0, i, auxinimigos=1;
    tipo_jogador jogador = {1,1,3,3,0,'c','J'}, inimigos[MAXEN]= {0,0,0,0,'c','E'}; //INICIALIZA JOGADOR COM VIDAS, BOMBAS, NENHUMA CHAVE E FRENTE DO JOGADOR COMECA PARA CIMA
    char nome_arquivo[30] = "Fase1.txt";
    tipo_mapa mapa;
    tipo_bomba bombas[3];
    srand(time(NULL));

    HideCursor();
    textcolor(WHITE);
    inicializa_mapa(nome_arquivo, &mapa);
    encontra(&jogador, jogador.caractere, mapa,1);
    menu(1, &mapa, &jogador, inimigos, &endgame);
    inicializa_inimigos(inimigos, &mapa);
    imprime(mapa);
    status(&jogador);
    //movimento_inimigos(inimigos, &mapa, &jogador, &endgame, 1);

    while(!endgame){
        //movimento_inimigos(inimigos, &mapa, &jogador, &endgame);
        explode_bomba(&mapa, &jogador, bombas, 0);
        action = teclado();
        if(action != 27){
            acao_jogador(&jogador, bombas, action, jogador.caractere, &mapa);

        }else{
            menu(0, &mapa, &jogador, inimigos, &endgame);

        }
    }
    gotoxy(1, 28);
    system("pause");
    return 0;
}
void reset(tipo_jogador *jogador){
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
void menu(int inicio, tipo_mapa *mapa, tipo_jogador *jogador, tipo_jogador inimigos[], int *endgame) /*ENDGAME = 1 SE JOGO DEVE ACABAR*/{
    FILE *save_mapa, *save_status;
    char c, save_num, nome_save[20] = "save_0.txt", lista_saves[9][20];
    int i;
    static int num_saves = 0;
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
        do  //TESTA QUANTOS SAVES JA EXISTEM
        {
            i++;
            save_num = i + '0';
            nome_save[5] = save_num;
            save_mapa = fopen(nome_save, "r");
            if(save_mapa != NULL)
            {
                strcpy(lista_saves[i-1], nome_save); //COPIA O NOME DO SAVE PARA UMA STRING
                num_saves++;
                fclose(save_mapa);
            }
        }
        while(i<=MAXSAVES);
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
        switch(teclado())
        {
        case 'N':
        case 'n':
            reset(jogador);
            clrscr();
            inicializa_mapa("mapabomber.txt", mapa);
            encontra(jogador, jogador->caractere, *mapa,1);
            inicializa_inimigos(inimigos, mapa);
            imprime(*mapa);
            status(jogador);

            break;

        case 'L':
        case 'l':
            for(i=0; i<=MAXSAVES; i++)
            {
                save_num = i + '1';
                nome_save[5] = save_num;
                save_mapa = fopen(nome_save, "r");
                if(save_mapa != NULL)
                {
                    strcpy(lista_saves[i], nome_save); //COPIA O NOME DO SAVE PARA UMA STRING
                    fclose(save_mapa);

                }
                else
                {
                    strcpy(lista_saves[i], "EMPTY");

                }
            }
            if(num_saves > 0)
            {
                textcolor(LIGHTCYAN);
                for(i=0; i<MAXSAVES; i++)  //IMPRIME A LISTA DE SAVES A DIRETIA DA TELA DO JOGO
                {
                    gotoxy(63, 3*i+1);
                    printf("(%d)%s", i+1, lista_saves[i]);
                }
                gotoxy(63, 27);
                textcolor(WHITE);
                printf("(0)DELETE SAVE");

                aguarda_teclado();
                save_num = teclado();

                if(save_num == '0')
                {
                    gotoxy(63, 27);
                    textbackground(RED);
                    printf("(0)DELETE SAVE");
                    textbackground(BLACK);
                    aguarda_teclado();
                    save_num = teclado();

                    if(save_num>'0' && (save_num - '0')<=MAXSAVES)
                    {
                        strcpy(nome_save, "save_0.txt");
                        nome_save[5] = save_num;

                        if(remove(nome_save) != 0)
                        {
                            gotoxy(1,1);
                            textcolor(LIGHTGREEN);
                            printf("\r                    ERRO AO EXCLUIR MAPA\t\t\t\t");
                            textcolor(WHITE);
                            break;

                        }
                        nome_save[7] = 'b';
                        nome_save[8] = 'i';
                        nome_save[9] = 'n';

                        if(remove(nome_save) != 0)
                        {
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
                        num_saves--;
                        //break;

                    }
                    else
                    {
                        apaga_linha(1,1);
                    }

                }
                else if(save_num>'0' && (save_num - '0')<=MAXSAVES)
                {
                    strcpy(nome_save, lista_saves[save_num-'1']);
                    nome_save[5] = save_num;
                    if(nome_save[0] == 'E')
                    {
                        gotoxy(1,1);
                        textcolor(LIGHTGREEN);
                        printf("\r                       SAVE INEXISTENTE\t\t\t\t");
                        textcolor(WHITE);

                    }
                    else
                    {

                        clrscr();
                        inicializa_mapa(nome_save, mapa);
                        encontra(jogador, jogador->caractere, *mapa, 1);
                        inicializa_inimigos(inimigos, mapa);

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

                }
                else
                {
                    apaga_linha(1,1);

                }

            }
            else
            {
                gotoxy(1,1);
                textcolor(LIGHTGREEN);
                printf("\r                       NO SAVED GAMES\t\t\t\t");
                textcolor(WHITE);

            }
            limpa_loads();
            break;

        case 'S':
        case 's':
            if(num_saves<=MAXSAVES)
            {
                i=0;
                do  //TESTA OS SAVES QUE JA EXISTEM
                {
                    i++;
                    save_num = i + '0';
                    nome_save[5] = save_num;
                    save_mapa = fopen(nome_save, "r");
                    if(save_mapa != NULL)
                    {
                        fclose(save_mapa);
                    }
                }
                while(save_mapa != NULL);

                save_mapa = fopen(nome_save, "w");
                nome_save[7] = 'b';
                nome_save[8] = 'i';
                nome_save[9] = 'n';
                save_status = fopen(nome_save, "wb");

                if(fwrite(jogador, sizeof(tipo_jogador), 1, save_status) != 1)
                {
                    gotoxy(1,1);
                    textcolor(LIGHTRED);
                    printf("ERRO AO SALVAR STATUS DO JOGADOR!");
                    textcolor(WHITE);
                }

                for(i=0; i<mapa->altura; i++)
                {
                    fputs(mapa->tamanho[i], save_mapa);

                }
                fclose(save_mapa);
                fclose(save_status);
                textcolor(LIGHTGREEN);
                printf("\r               GAME SAVED SUCCESSFULLY! %d LEFT\t\t\t\t\t", MAXSAVES-num_saves);
                textcolor(WHITE);
                num_saves++;

            }
            else
            {

                gotoxy(1,1);
                textcolor(LIGHTGREEN);
                printf("\r                      SAVES AT MAXIMUM\t\t\t\t");
                textcolor(WHITE);
            }
            break;

        case 'R':
        case 'r':
            if(jogador->vidas>0)
            {

                apaga_linha(1, 1);
                break;
            }
            else
            {
                goto start;
            }

        case 'Q':
        case 'q':
            textcolor(LIGHTRED);
            printf("\r                        ENDING GAME\t\t\t\t\t\t\n");
            textcolor(WHITE);
            *endgame = 1;
            gotoxy(1,28);
            break;

        default:
            goto start;
        }
        if(!(*endgame))
        {
            gotoxy(jogador->x+1, jogador->y);
        }
    }
}
void limpa_loads(){
    int i;
    for(i=0; i<10; i++)  //APAGA A LISTA DE SAVES A DIRETIA DA TELA E VOLTA AO JOGO
    {
        gotoxy(63, 3*i+1);
        printf("\t\t\t");
    }
    gotoxy(63, 27);
    printf("\t\t\t");
}
void inicializa_bin(char nome_bin[], tipo_jogador *jogador){
    FILE *arquivo_bin;

    arquivo_bin = fopen(nome_bin, "rb");
    if(arquivo_bin == NULL)
    {
        textcolor(LIGHTRED);
        gotoxy(1, 1);
        printf("\r                    ERRO AO ABRIR STATUS!\t\t\t\t\t\n");
        textcolor(WHITE);

    }
    else
    {
        if(fread(jogador, sizeof(tipo_jogador), 1, arquivo_bin) != 1)
        {
            textcolor(LIGHTRED);
            gotoxy(1, 1);
            printf("\r                    ERRO AO ATUALIZAR STATUS!\t\t\t\t\t\n");
            textcolor(WHITE);
        }
        fclose(arquivo_bin);
    }
}
void aguarda_teclado(){
    while(!kbhit()) {}
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
    for(i=0; i<mapa.altura; i++)
    {
        for(j=0; j<mapa.largura; j++)
        {
            switch(mapa.tamanho[i][j])
            {
            case 'W':
                printf("#");
                break;

            case 'D':
                printf("&");
                break;

            case 'K':
            case 'B':
                printf("K");
                break;

            case 'E':
                printf("I");
                break;

            case 'X':
                printf(" ");
                break;

            default:
                printf("%c", mapa.tamanho[i][j]);
            }
        }
        printf("\n");
        //printf("%s", mapa.tamanho[i]);

    }

}
void encontra(tipo_jogador *jogador, char personagem, tipo_mapa mapa,int quant){
    int i=0, j=0, x, y, aux=0;
    do{
        do{
            if(mapa.tamanho[i][j] == personagem){
                x = j+1;//+1 para ficar conforme as coordenadas da tela, que começa em 1,1
                y = i+2;//+2 para ficar conforme a tela (deslocada 1un para baixo)
                aux++;
            }
            j++;
        }while(j<mapa.largura);
        j=0;
        i++;
    }while(i<mapa.altura && aux<=quant);
    jogador->x = x;
    jogador->y = y;
}
void acao_jogador(tipo_jogador *jogador, tipo_bomba bombas[], int action, char c, tipo_mapa* mapa){
    int x, y;
    clock_t start;
    x = jogador->x-1;
    y = jogador->y-2;
    switch(action){
    case 119://W
    case 72: //PARA CIMA
        jogador->frente = 'c';
        if(mapa->tamanho[y-1][x]==VAZIO){   //Comparo com o mapa, mas tiro um de cada para passar da coordenada 1,1 para 0,0. A subtração adicional é para ir para a proxima posição solicitadoa
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y-1, c);
            atualizamapa(c, mapa, jogador); //Dentro dessa função nao precisou do & antes de mapa e jogar pois nesse escopo eles já são ponteiros
            jogador->y--;

        }
        break;
    case 115://S
    case 80: //PARA BAIXO
        jogador->frente = 'b';
        if(mapa->tamanho[y+1][x]==VAZIO){
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y+1,c);
            atualizamapa(c, mapa, jogador);
            jogador->y++;

        }
        break;
    case 97: //A
    case 75: //PARA ESQUERDA
        jogador->frente = 'e';
        if(mapa->tamanho[y][x-1]==VAZIO){
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x-1,jogador->y,c);
            atualizamapa(c, mapa, jogador);
            jogador->x--;

        }
        break;
    case 100://D
    case 77: //PARA DIREITA
        jogador->frente = 'd';
        if(mapa->tamanho[y][x+1]==VAZIO){
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x+1,jogador->y,c);
            atualizamapa(c, mapa, jogador);
            jogador->x++;
        }
        break;

    case 'b'://BOMBA
    case 'B':
        if(jogador->bombas==3){
            switch(jogador->frente){
                case 'c':
                    if(mapa->tamanho[y-1][x]==VAZIO){
                        putchxy(jogador->x, jogador->y-1, '@');
                        jogador->bombas--;
                        bombas[0].tx = jogador->x;
                        bombas[0].ty = jogador->y-1;
                        bombas[0].mx = jogador->x-1;
                        bombas[0].my = jogador->y-3;
                        explode_bomba(mapa, jogador, bombas, 1);
                        atualizamapa('@', mapa, jogador);
                    }
                    break;

                case 'b':
                    if(mapa->tamanho[y+1][x]==VAZIO){
                        putchxy(jogador->x, jogador->y+1, '@');
                        jogador->bombas--;
                        bombas[0].tx = jogador->x;
                        bombas[0].ty = jogador->y+1;
                        bombas[0].mx = jogador->x-1;
                        bombas[0].my = jogador->y-1;
                        explode_bomba(mapa, jogador, bombas, 1);
                        atualizamapa('@', mapa, jogador);
                    }
                    break;

                case 'e':
                    if(mapa->tamanho[y][x-1]==VAZIO){
                        putchxy(jogador->x-1, jogador->y, '@');
                        jogador->bombas--;
                        bombas[0].tx = jogador->x-1;
                        bombas[0].ty = jogador->y;
                        bombas[0].mx = jogador->x-2;
                        bombas[0].my = jogador->y-2;
                        explode_bomba(mapa, jogador, bombas, 1);
                        atualizamapa('@', mapa, jogador);
                    }
                    break;

                case 'd':
                    if(mapa->tamanho[y][x+1]==VAZIO){
                        putchxy(jogador->x+1, jogador->y, '@');
                        jogador->bombas--;
                        bombas[0].tx = jogador->x+1;
                        bombas[0].ty = jogador->y;
                        bombas[0].mx = jogador->x;
                        bombas[0].my = jogador->y-2;
                        explode_bomba(mapa, jogador, bombas, 1);
                        atualizamapa('@', mapa, jogador);
                    }
                    break;

                }
            status(jogador);
        }
        break;
    }
}
void atualizamapa(char item, tipo_mapa *mapa, tipo_jogador *jogador){
    int x, y;
    x = jogador->x-1;
    y = jogador->y-2;
    mapa->tamanho[y][x]=VAZIO;
    switch(jogador->frente){
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
void inicializa_inimigos(tipo_jogador inimigos[],tipo_mapa* mapa){
    int i;
    tipo_jogador padrao= {0,0,0,0,0,'c','E',0};
    for(i=0; i<MAXEN; i++){
        inimigos[i] = padrao;
        encontra(&inimigos[i], inimigos[i].caractere, *mapa, i);
        inimigos[i].direcao=rand()%4;
    }
}
void movimento_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa, tipo_jogador *jogador, int *endgame/*, int primeirorun*/){
    int i, x, y, aux=0, k, primeirorun=1;
    clock_t start;
    if(k!=0 && k!=1 && primeirorun){  //Filtro para confimar que é a prrimeira vez, tentei colocar k=0 no inicio mas dai nunca sai do laço
        k=0;
        primeirorun=0;
    }
    if(k==0){
        start=clock();
        k=1;
    }
    if(conta_tempo_inimigos(start)==1){
        k=0;
        for(i=0; i<MAXEN; i++){
            x = inimigos[i].x-1;
            y = inimigos[i].y-2;
            aux++;
            switch (inimigos[i].direcao){
                case 0: //PARA CIMA
                    inimigos[i].frente = 'c';
                    if(mapa->tamanho[y-1][x]==jogador->caractere){
                        jogador->vidas--;
                        status(jogador);
                        inimigos[i].direcao=rand()%4;;
                        if(jogador->vidas==0){
                            gotoxy(26 ,13);
                            textcolor(LIGHTRED);
                            printf("YOU DIED");
                            textcolor(WHITE);
                            *endgame = 1;
                            return;
                        }
                    }
                    if(mapa->tamanho[y-1][x]==VAZIO){   //Comparo com o mapa, mas tiro um de cada para passar da coordenada 1,1 para 0,0. A subtração adicional é para ir para a proxima posição solicitadoa
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x,inimigos[i].y-1, ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]); //Dentro dessa função nao precisou do & antes de mapa e jogar pois nesse escopo eles já são ponteiros
                        inimigos[i].y--;

                    }else{
                        inimigos[i].direcao=rand()%4;
                    }
                    break;
                case 1: //PARA BAIXO
                    inimigos[i].frente = 'b';
                    if(mapa->tamanho[y+1][x]==jogador->caractere){
                        jogador->vidas--;
                        status(jogador);
                        inimigos[i].direcao=rand()%4;
                        if(jogador->vidas==0){
                            gotoxy(26 ,13);
                            textcolor(LIGHTRED);
                            printf("YOU DIED");
                            textcolor(WHITE);
                            *endgame = 1;
                            break;

                        }
                    }
                    if(mapa->tamanho[y+1][x]==VAZIO){
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x,inimigos[i].y+1,ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]);
                        inimigos[i].y++;

                    }else{
                        inimigos[i].direcao=rand()%4;
                    }
                    break;
                case 2: //PARA ESQUERDA
                    inimigos[i].frente = 'e';
                    if(mapa->tamanho[y][x-1]==jogador->caractere){
                        jogador->vidas--;
                        status(jogador);
                        inimigos[i].direcao=rand()%4;
                        if(jogador->vidas==0){
                            gotoxy(26 ,13);
                            textcolor(LIGHTRED);
                            printf("YOU DIED");
                            textcolor(WHITE);
                            *endgame = 1;
                            break;
                        }
                    }
                    if(mapa->tamanho[y][x-1]==VAZIO){
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x-1,inimigos[i].y,ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]);
                        inimigos[i].x--;

                    }else{
                        inimigos[i].direcao=rand()%4;
                    }
                    break;
                case 3: //PARA DIREITA
                    if(mapa->tamanho[y][x+1]==jogador->caractere){
                        jogador->vidas--;
                        status(jogador);
                        inimigos[i].direcao=rand()%4;
                        if(jogador->vidas==0){
                            gotoxy(26 ,13);
                            textcolor(LIGHTRED);
                            printf("YOU DIED");
                            textcolor(WHITE);
                            *endgame = 1;
                            break;
                        }
                    }
                    inimigos[i].frente = 'd';
                    if(mapa->tamanho[y][x+1]== VAZIO){
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x+1,inimigos[i].y,ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]);
                        inimigos[i].x++;
                    }else{
                        inimigos[i].direcao=rand()%4;
                    }
                    break;
            }
        }
    }
    if(aux>5){
        for(i=0; i<MAXEN; i++){
            inimigos[i].direcao=rand()%4;
        }
        aux=0;
    }
}
int conta_tempo_inimigos(clock_t start){
    clock_t atual;
    float set = (float)10000/VELOCIDADE;
    double dif;
    int esgotou = 0;

    atual=clock();
    dif=1000*((double)(atual-start)/CLOCKS_PER_SEC);
    if(dif >= set){
        esgotou = 1;
    }

    return esgotou;
}
int conta_tempo_bomba(clock_t start){
    clock_t atual;
    double dif;
    int esgotou = 0;

    atual=clock();
    dif=1000*((double)(atual-start)/CLOCKS_PER_SEC);
    if(dif >= (float)T_BOMBA){
        esgotou = 1;
    }
    gotoxy(1,29);
    printf("%d", clock());
    return esgotou;
}
void explode_bomba(tipo_mapa *mapa, tipo_jogador *jogador, tipo_bomba bombas[], int aciona){
    int aux;
    if(jogador->bombas < 3 && aciona){
        bombas[0].start = clock();
        gotoxy(1,28);
        printf("%d", bombas[0].start);
    }
    if(jogador->bombas<3 && conta_tempo_bomba(bombas[0].start) == 1){
        putchxy(bombas[0].tx, bombas[0].ty, VAZIO);
        mapa->tamanho[bombas[0].my][bombas[0].mx] = VAZIO;
        jogador->bombas++;
        status(jogador);
    }
}
void HideCursor(){
  CONSOLE_CURSOR_INFO cursor = {1, FALSE};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
