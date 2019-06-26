#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define NUMVIDAS 3
#define NUMBOMBAS 3
#define MAXSAVES 9
#define MAXEN 5
#define MAXFASES 9
#define VELOCIDADE 50
#define T_BOMBA 3000
#define VAZIO 32 //32=ESPAÇO 88=X
#define ENEMY 165
#define RANGE 2
#define CHAVE_TELA 182
#define CHAVE 67
#define PLAYER 167//223
#define BOMBA 164//216 64
#define FDIRECAO 10 //Frequencia com que os inimigos mudam de posicao automaticamente
#define RADAR 5 // Distancia que os inimigos enxergam
typedef struct
{
    int x, y, vidas, bombas, chaves, pontos, direcao, fase;
    char frente, caractere;
    int auxmudanca, nivel;
} tipo_jogador;

typedef struct
{
    char tamanho[25][62];
    int largura, altura;
} tipo_mapa;

typedef struct
{
    clock_t start;
    int mx, my, tx, ty, acionada;
    double t_decorrido;
} tipo_bomba;

void acao_jogador(tipo_jogador *jogador, tipo_jogador inimigos[], tipo_bomba bombas[], int action, tipo_mapa* mapa);
void aguarda_teclado();
void apaga_linha(int x, int y);
void atualizamapa(char item, tipo_mapa *mapa, tipo_jogador *jogador);
int conta_tempo_bomba(tipo_bomba *bomba);
int conta_tempo_inimigos(clock_t* start);
void encontra (tipo_jogador *jogador, tipo_mapa *mapa, int qnt);
void explode_bomba(tipo_mapa *mapa, tipo_jogador *jogador, tipo_bomba bombas[], tipo_jogador inimigos[]);
void HideCursor();
void imprime(tipo_mapa mapa);
void inicializa_bin(char nome_bin[], tipo_jogador *jogador, tipo_jogador inimigos[], tipo_bomba bombas[]);
void inicializa_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa, int first_run);
void inicializa_mapa(char nome_arquivo[], tipo_mapa *mapa, int *endgame);
void invertedirecao(tipo_jogador* inimigos);
void limpa_loads();
void menu(int inicio, tipo_mapa *mapa, tipo_jogador *jogador, tipo_jogador inimigos[], tipo_bomba bombas[], int *endgame);
void mortejogador(tipo_jogador* jogador, tipo_mapa* mapa,tipo_jogador inimigos[]);
void movimento_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa, tipo_jogador *jogador, clock_t *inicio,tipo_bomba bombas[]);
void novadirecaoinimigos(tipo_jogador* inimigos);
void novapos(tipo_jogador* jogador,tipo_mapa* mapa);
void procurabomba(tipo_bomba* bomba,tipo_jogador* inimigos,tipo_mapa *mapa);
void procurajogador(tipo_jogador* jogador,tipo_jogador* inimigos,tipo_mapa* mapa);
void reset(tipo_jogador *jogador, int start);
void status(tipo_jogador *jogador, tipo_jogador inimigos[]);
char teclado();
void tela_inicio(tipo_jogador *jogador, int *endgame);

int main()
{
    char fase[MAXFASES][20] = {"./Fases/Fase1.txt", "./Fases/Fase2.txt", "./Fases/Fase3.txt", "./Fases/Fase4.txt", "./Fases/Fase5.txt", "./Fases/Fase6.txt", "./Fases/Fase7.txt", "./Fases/Fase8.txt", "./Fases/Fase9.txt"};
    int i,action, endgame=0, start = 1;
    tipo_jogador inimigos[MAXEN], jogador;
    tipo_mapa mapa;
    tipo_bomba bombas[NUMBOMBAS];
    clock_t inicio = clock();

    srand(time(NULL));
    HideCursor();
    textcolor(WHITE);

MenuInicial:
    reset(&jogador, start);
    menu(start, &mapa, &jogador, inimigos, bombas, &endgame);

LoadGame:
    inicializa_mapa(fase[jogador.fase], &mapa, &endgame);
    encontra(&jogador, &mapa, 1);
    inicializa_inimigos(inimigos, &mapa, start);
    imprime(mapa);
    status(&jogador, inimigos);
    movimento_inimigos(inimigos, &mapa, &jogador, &inicio,bombas);

    start = 0;

    for(i=0; i<NUMBOMBAS; i++) //Iniciliza as bombas no jogo
    {
        bombas[i].acionada = 0;
    }

    while(!endgame)
    {
        if(jogador.vidas>0 && jogador.chaves<5)
        {
            movimento_inimigos(inimigos, &mapa, &jogador, &inicio,bombas);
            for(i=0; i<MAXEN; i++)
            {
                if(inimigos[i].auxmudanca>FDIRECAO) //Para realizar a mudança aleatoria de direcao do inimigo, utilizamos uma variavel que verifica a quantidade de movimentos que o inimigo já realizou desde a ultima mudança
                {

                    novadirecaoinimigos(&inimigos[i]);

                }
            }
            explode_bomba(&mapa, &jogador, bombas, inimigos); //Verifica se alguma bomba foi colocada no mapa e testa se está na hora de sua explosao
            action = teclado();
            if(action != 27) //ESC
            {
                acao_jogador(&jogador, inimigos, bombas, action, &mapa);

            }
            else
            {
                menu(start, &mapa, &jogador, inimigos, bombas, &endgame);

            }

        }
        else if(jogador.chaves == 5) //MUDANÇA DE FASE
        {
            reset(&jogador, start);
            jogador.fase++;
            start = 1;
            goto LoadGame;

        }
        else //MORTE DO JOGADOR
        {
            start = 1;
            gotoxy(25, 13);
            textcolor(LIGHTRED);
            printf("VOCE MORREU");
            textcolor(WHITE);
            Sleep(1500);
            clrscr();
            goto MenuInicial;
        }
    }
    gotoxy(1, 28);
    system("pause");
    return 0;
}
void reset(tipo_jogador *jogador, int start)
{
    //Função para inicializacao do jogador
    jogador->x = 1;
    jogador->y = 1;
    jogador->vidas = NUMVIDAS;
    jogador->bombas = NUMBOMBAS;
    jogador->chaves = 0;
    jogador->frente = 'c';
    jogador->caractere = 'J';
    if(start)
    {
        jogador->nivel = 1;
        jogador->fase = 0;
        jogador->pontos = 0;
    }
}
void status(tipo_jogador *jogador, tipo_jogador inimigos[])
{
    //Função para atualização dos dados exibidos em tela do jogo
    int i, num_inimigos=0;
    for(i=0; i<MAXEN; i++)
    {
        if(inimigos[i].vidas)
        {
            num_inimigos++; //Verifica quantos inimigos ainda estão vivos
        }
    }
    if(jogador->pontos < 0)
    {
        jogador->pontos = 0;
    }
    gotoxy(1,27);
    if(jogador->nivel == 1){
        textcolor(LIGHTGREEN);
    }else if(jogador->nivel == 2){
        textcolor(LIGHTRED);
    }
    printf("PHASE:%d   LIFES:%d   BOMBS:%d   KEYS:%d   ENEMIES:%d  POINTS:%.3d", jogador->fase+1, jogador->vidas, jogador->bombas, jogador->chaves, num_inimigos, jogador->pontos);
    textcolor(WHITE);
}
void tela_inicio(tipo_jogador *jogador, int *endgame)
{
    int i;
    textcolor(LIGHTRED);
    gotoxy(26, 9);
    printf("*BOMBERMAN");
    gotoxy(22, 13);
    for(i=1; i<=60; i++)
    {
        putchxy(i, 2, '#');
    }
    for(i=3; i<=25; i++)
    {
        putchxy(1, i, '#');
        putchxy(60, i, '#');
    }
    for(i=1; i<=60; i++)
    {
        putchxy(i, 26, '#');
    }
    gotoxy(25, 14);
    printf("SELECT LEVEL");
    gotoxy(27, 16);
    printf("(1)EASY");
    gotoxy(27, 17);
    printf("(2)HARD");
    gotoxy(27, 18);
    printf("(3)QUIT");
    textcolor(WHITE);

    aguarda_teclado();
    do
    {
        jogador->nivel = teclado() - 48;
    }
    while(jogador->nivel != 1 && jogador->nivel != 2 && jogador->nivel != 3);

    clrscr();

    if(jogador->nivel == 3){
        gotoxy(1,13);
        textcolor(LIGHTRED);
        printf("\r                        ENDING GAME\t\t\t\t\t\t\n");
        textcolor(WHITE);
        *endgame = 1;
    }
}
void menu(int inicio, tipo_mapa *mapa, tipo_jogador *jogador, tipo_jogador inimigos[], tipo_bomba bombas[], int *endgame) /*ENDGAME = 1 SE JOGO DEVE ACABAR*/
{
    FILE *save_mapa, *save_status;
    char c, save_num, nome_save[] = "Saves/save_0.txt", lista_saves[9][20];
    int i, j;
    static int num_saves = 0;

    if(inicio)
    {
        tela_inicio(jogador, endgame);
        i=0;
        do  //TESTA QUANTOS SAVES JA EXISTEM
        {
            i++;
            save_num = i + '0';
            nome_save[11] = save_num;
            save_mapa = fopen(nome_save, "r");
            if(save_mapa != NULL)
            {
                strcpy(lista_saves[i-1], nome_save); //COPIA O NOME DO SAVE PARA UMA STRING
                num_saves++;
                fclose(save_mapa);
            }
        }
        while(i<=MAXSAVES);

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
            reset(jogador, 0);
            clrscr();

            inicializa_mapa("Fases/Fase1.txt", mapa, endgame);
            encontra(jogador, mapa, 1);
            inicializa_inimigos(inimigos, mapa, 1);
            tela_inicio(jogador, endgame);

            if(!(*endgame)){
                imprime(*mapa);
                status(jogador, inimigos);
            }
            break;

        case 'L':
        case 'l':
            for(i=0; i<=MAXSAVES; i++)
            {
                save_num = i + '1';
                nome_save[11] = save_num;
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

                    if(save_num>'0' && (save_num - '0')<=MAXSAVES && lista_saves[save_num-'1'][0]!='E')
                    {
                        strcpy(nome_save, "Saves/save_0.txt");
                        nome_save[11] = save_num;
                        gotoxy(1, 28);

                        if(remove(nome_save) != 0)
                        {
                            gotoxy(1,1);
                            textcolor(LIGHTGREEN);
                            printf("\r                    ERRO AO EXCLUIR MAPA\t\t\t\t");
                            textcolor(WHITE);
                            break;

                        }

                        strcpy(lista_saves[save_num-'1'], "EMPTY");

                        nome_save[13] = 'b';
                        nome_save[14] = 'i';
                        nome_save[15] = 'n';

                        if(remove(nome_save) != 0)
                        {
                            gotoxy(1,1);
                            textcolor(LIGHTGREEN);
                            printf("\r                   ERRO AO EXCLUIR STATUS\t\t\t\t");
                            textcolor(WHITE);
                            break;

                        }

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
                    nome_save[11] = save_num;
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
                        inicializa_mapa(nome_save, mapa, endgame);

                        nome_save[13] = 'b';
                        nome_save[14] = 'i';
                        nome_save[15] = 'n';

                        inicializa_bin(nome_save, jogador, inimigos, bombas);

                        imprime(*mapa);
                        status(jogador, inimigos);

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
                    nome_save[11] = save_num;
                    save_mapa = fopen(nome_save, "r");
                    if(save_mapa != NULL)
                    {
                        fclose(save_mapa);
                    }
                }
                while(save_mapa != NULL);

                save_mapa = fopen(nome_save, "w");
                nome_save[13] = 'b';
                nome_save[14] = 'i';
                nome_save[15] = 'n';
                save_status = fopen(nome_save, "wb");

                if(fwrite(jogador, sizeof(tipo_jogador), 1, save_status) != 1)
                {
                    gotoxy(1,1);
                    textcolor(LIGHTRED);
                    printf("ERRO AO SALVAR STATUS DO JOGADOR!");
                    textcolor(WHITE);
                }
                if(fwrite(inimigos, sizeof(tipo_jogador), MAXEN, save_status) != MAXEN)
                {
                    gotoxy(1,1);
                    textcolor(LIGHTRED);
                    printf("ERRO AO SALVAR STATUS DOS INIMIGOS!");
                    textcolor(WHITE);
                }
                if(fwrite(bombas, sizeof(tipo_bomba), NUMBOMBAS, save_status) != NUMBOMBAS)
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
                printf("\r                     SAVES AT MAXIMUM\t\t\t\t");
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
            *endgame = 1;
            gotoxy(1,28);
            break;

        default:
            goto start;
        }
        if(!(*endgame))
        {
            for(j=0; j<NUMBOMBAS; j++)  // DESCONTA O TEMPO DECORRIDO DA BOMBA NO START DA MESMA PARA CONTAGEM APENAS DO TEMPO RESTANTE
            {
                bombas[j].start = clock() - bombas[j].t_decorrido;

            }
        }
    }
}
void limpa_loads()
{
    int i;
    for(i=0; i<10; i++)  //APAGA A LISTA DE SAVES A DIRETIA DA TELA E VOLTA AO JOGO
    {
        gotoxy(63, 3*i+1);
        printf("\t\t\t");
    }
    gotoxy(63, 27);
    printf("\t\t\t");
}
void inicializa_bin(char nome_bin[], tipo_jogador *jogador, tipo_jogador inimigos[], tipo_bomba bombas[])
{
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
            printf("\r                    ERRO AO ATUALIZAR STATUS JOGADOR!\t\t\t\t\t\n");
            textcolor(WHITE);
        }
        if(fread(inimigos, sizeof(tipo_jogador), MAXEN, arquivo_bin) != MAXEN)
        {
            textcolor(LIGHTRED);
            gotoxy(1, 1);
            printf("\r                    ERRO AO ATUALIZAR STATUS INIMIGOS!\t\t\t\t\t\n");
            textcolor(WHITE);
        }
        if(fread(bombas, sizeof(tipo_bomba), NUMBOMBAS, arquivo_bin) != NUMBOMBAS)
        {
            textcolor(LIGHTRED);
            gotoxy(1, 1);
            printf("\r                    ERRO AO ATUALIZAR STATUS BOMBAS!\t\t\t\t\t\n");
            textcolor(WHITE);
        }
        fclose(arquivo_bin);
    }
}
void aguarda_teclado()
{
    while(!kbhit()) {}
}
void apaga_linha(int x, int y)
{
    gotoxy(x, y);
    printf("\t\t\t\t\t\t\t\t");
}
char teclado()
{
    char c;
    if(kbhit())
    {
        c = getch();
    }
    return c;
}
void inicializa_mapa(char nome_arquivo[], tipo_mapa *mapa, int *endgame)
{
    //Le o arquivo do mapa e transfere ele para uma matriz, que será utilizada para verificar colisões e posições w
    int i=0;
    FILE *arquivo_mapa;
    char texto[62];
    arquivo_mapa = fopen(nome_arquivo, "r");
    if(arquivo_mapa == NULL)
    {
        *endgame = 1;
        gotoxy(26 ,13);
        textcolor(LIGHTGREEN);
        printf("GAME OVER");
        gotoxy(25 ,14);
        printf("VOCE VENCEU");
        textcolor(WHITE);
        /*textcolor(LIGHTRED);
        gotoxy(1, 1);
        printf("\r                    ERRO AO ABRIR MAPA!\t\t\t\t\t\n");
        textcolor(WHITE);*/
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
}
void imprime(tipo_mapa mapa)
{
    //Impressão do mapa na tela, com as conversões para cada tipo de item
    int i, j;
    gotoxy(1,2);
    for(i=0; i<mapa.altura; i++)
    {
        for(j=0; j<mapa.largura; j++)
        {
            switch(mapa.tamanho[i][j])
            {
            case 'W':
                textbackground(LIGHTGRAY);
                textcolor(LIGHTGRAY);
                putchxy(j+1, i+2, '#');
                textbackground(BLACK);
                textcolor(WHITE);
                break;

            case 'D':
                textbackground(WHITE);
                putchxy(j+1, i+2, '&');
                textbackground(BLACK);
                break;

            case 'K':
            case 'B':
                textbackground(YELLOW);
                textcolor(YELLOW);
                putchxy(j+1, i+2, 'K');
                textcolor(WHITE);
                textbackground(BLACK);
                break;

            case 'E':
                textcolor(LIGHTRED);
                putchxy(j+1, i+2, ENEMY);
                textcolor(WHITE);
                break;

            case 'X':
                putchxy(j+1, i+2, ' ');
                break;

            case 'J':
                textcolor(LIGHTCYAN);
                putchxy(j+1, i+2, PLAYER);
                textcolor(WHITE);
                break;

            case '@':
                textcolor(LIGHTMAGENTA);
                putchxy(j+1, i+2, BOMBA);
                textcolor(WHITE);
                break;

            case CHAVE:
                textcolor(LIGHTGREEN);
                putchxy(j+1, i+2, CHAVE_TELA);
                textcolor(WHITE);
                break;

            default:
                putchxy(j+1, i+2, mapa.tamanho[i][j]);
            }
            if(mapa.tamanho[i][j]=='W'&&(i==0||j==0||i==mapa.altura-1||j==mapa.largura-1))//Colorir as bordas do jogo conforme os prints do pdf do sor
            {
                textbackground(LIGHTBLUE);
                textcolor(LIGHTBLUE);
                putchxy(j+1, i+2, '#');
                textbackground(BLACK);
                textcolor(WHITE);
            }
        }

        //printf("\n");
        //printf("%s", mapa.tamanho[i]);

    }

}
void encontra(tipo_jogador *jogador, tipo_mapa *mapa, int qnt)
{
    //Encontra o jogador no mapa
    int i=0, j=0, x, y, cont=0;
    do
    {
        do
        {
            if(mapa->tamanho[i][j] == (jogador+cont)->caractere)
            {
                (jogador+cont)->x = j+1;//+1 para ficar conforme as coordenadas da tela, que começa em 1,1
                (jogador+cont)->y = i+2;//+2 para ficar conforme a tela (deslocada 1un para baixo)
                cont++;
            }
            j++;
        }
        while(j<mapa->largura);
        j=0;
        i++;
    }
    while(i<mapa->altura && cont<qnt);

}
void acao_jogador(tipo_jogador *jogador, tipo_jogador inimigos[], tipo_bomba bombas[], int action, tipo_mapa *mapa)
{
    //Função contendo tudo que o usuario pode comandar
    int i, x, y;
    x = jogador->x-1;
    y = jogador->y-2;
    textcolor(LIGHTCYAN);
    //textbackground(LIGHTCYAN);
    switch(action)
    {
    //Inicialmente temos a movimentação do jogador, com todas as atualizações em tela e na matriz necessarias
    case 119://w
    case 87://W
    case 72: //PARA CIMA
        jogador->frente = 'c';
        if(mapa->tamanho[y-1][x]==inimigos->caractere)
        {
            mortejogador(jogador,mapa,inimigos);
        }
        else if(mapa->tamanho[y-1][x]==VAZIO)    //Comparo com o mapa, mas tiro um de cada para passar da coordenada 1,1 para 0,0. A subtração adicional é para ir para a proxima posição solicitadoa
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y-1, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador); //Dentro dessa função nao precisou do & antes de mapa e jogar pois nesse escopo eles já são ponteiros
            jogador->y--;

        }
        else if(mapa->tamanho[y-1][x]==CHAVE)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y-1, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->y--;
            jogador->chaves++;
            status(jogador, inimigos);

        }
        break;
    case 115://s
    case 83://S
    case 80: //PARA BAIXO
        jogador->frente = 'b';
        if(mapa->tamanho[y+1][x]==inimigos->caractere)
        {
            mortejogador(jogador,mapa,inimigos);
        }
        else if(mapa->tamanho[y+1][x]==VAZIO)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y+1, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->y++;

        }
        else if(mapa->tamanho[y+1][x]==CHAVE)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y+1, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->y++;
            jogador->chaves++;
            status(jogador, inimigos);
        }
        break;
    case 97://a
    case 65://A
    case 75: //PARA ESQUERDA
        jogador->frente = 'e';
        if(mapa->tamanho[y][x-1]==inimigos->caractere)
        {
            mortejogador(jogador,mapa,inimigos);
        }
        else if(mapa->tamanho[y][x-1]==VAZIO)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x-1,jogador->y, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->x--;

        }
        else if(mapa->tamanho[y][x-1]==CHAVE)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x-1,jogador->y, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->x--;
            jogador->chaves++;
            status(jogador, inimigos);
        }
        break;
    case 100://d
    case 68://D
    case 77: //PARA DIREITA
        jogador->frente = 'd';
        if(mapa->tamanho[y][x+1]==inimigos->caractere)
        {
            mortejogador(jogador,mapa,inimigos);
        }
        else if(mapa->tamanho[y][x+1]==VAZIO)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x+1,jogador->y, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->x++;

        }
        else if(mapa->tamanho[y][x+1]==CHAVE)
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x+1,jogador->y, PLAYER);
            atualizamapa(jogador->caractere, mapa, jogador);
            jogador->x++;
            jogador->chaves++;
            status(jogador, inimigos);
        }
        break;
    //Inserção de bomba
    case 'b'://BOMBA
    case 'B':
   // case '5':
        if(jogador->bombas>0)
        {

            textcolor(LIGHTMAGENTA);
            switch(jogador->frente)
            {
            //VERIFICA PARA QUE DIRECAO O JOGADOR ESTPA VIRADO
            case 'c':
                if(mapa->tamanho[y-1][x]==VAZIO)
                {
                    putchxy(jogador->x, jogador->y-1, BOMBA);
                    jogador->bombas--;
                    for(i=0; i<NUMBOMBAS; i++)
                    {
                        if(!bombas[i].acionada)
                        {
                            bombas[i].tx = jogador->x;
                            bombas[i].ty = jogador->y-1;
                            bombas[i].mx = jogador->x-1;
                            bombas[i].my = jogador->y-3;
                            bombas[i].start = clock();
                            bombas[i].acionada = 1;
                            break;
                        }
                    }
                    atualizamapa('@', mapa, jogador);
                }
                break;

            case 'b':
                if(mapa->tamanho[y+1][x]==VAZIO)
                {
                    putchxy(jogador->x, jogador->y+1, BOMBA);
                    jogador->bombas--;
                    for(i=0; i<NUMBOMBAS; i++)
                    {
                        if(!bombas[i].acionada)
                        {
                            bombas[i].tx = jogador->x;
                            bombas[i].ty = jogador->y+1;
                            bombas[i].mx = jogador->x-1;
                            bombas[i].my = jogador->y-1;
                            bombas[i].start = clock();
                            bombas[i].acionada = 1;
                            break;
                        }
                    }
                    atualizamapa('@', mapa, jogador);
                }
                break;

            case 'e':
                if(mapa->tamanho[y][x-1]==VAZIO)
                {
                    putchxy(jogador->x-1, jogador->y, BOMBA);
                    jogador->bombas--;
                    for(i=0; i<NUMBOMBAS; i++)
                    {
                        if(!bombas[i].acionada)
                        {
                            bombas[i].tx = jogador->x-1;
                            bombas[i].ty = jogador->y;
                            bombas[i].mx = jogador->x-2;
                            bombas[i].my = jogador->y-2;
                            bombas[i].start = clock();
                            bombas[i].acionada = 1;
                            break;
                        }
                    }
                    atualizamapa('@', mapa, jogador);
                }
                break;

            case 'd':
                if(mapa->tamanho[y][x+1]==VAZIO)
                {
                    putchxy(jogador->x+1, jogador->y, BOMBA);
                    jogador->bombas--;
                    for(i=0; i<NUMBOMBAS; i++)
                    {
                        if(!bombas[i].acionada)
                        {
                            bombas[i].tx = jogador->x+1;
                            bombas[i].ty = jogador->y;
                            bombas[i].mx = jogador->x;
                            bombas[i].my = jogador->y-2;
                            bombas[i].start = clock();
                            bombas[i].acionada = 1;
                            break;
                        }
                    }
                    atualizamapa('@', mapa, jogador);
                }
                break;
            }
            status(jogador, inimigos);
        }
        break;

    case '4':
        jogador->frente = 'e';
        break;

    case '5':
        case '2':
        jogador->frente = 'b';
        break;

    case '6':
        jogador->frente = 'd';
        break;

    case '8':
        jogador->frente = 'c';
        break;

    }
    textcolor(WHITE);
}
void atualizamapa(char item, tipo_mapa *mapa, tipo_jogador *jogador)
{
    //Atualiza a matriz contendo o mapa
    int x, y;
    x = jogador->x-1;
    y = jogador->y-2;
    if(item != '@')
    {
        mapa->tamanho[y][x]=VAZIO;

    }
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
void inicializa_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa, int first_run)
{
    //Inicializa os inimigos, encontrando todos eles no mapa de acordo com MAXEN e selecionando uma direcao
    int i, num_inimigos = 0;
    tipo_jogador padrao= {0,0,1,0,0,0,0,0,'c','E',0};

    for(i=0; i<MAXEN; i++)
    {
        if(first_run)
        {
            inimigos[i] = padrao;

        }

        if(inimigos[i].vidas > 0)
        {
            num_inimigos++;

        }
    }
    encontra(inimigos, mapa, num_inimigos);//Encontra as posições de cada inimigo no mapa

    for(i=0; i<num_inimigos; i++) //Define uma direção inicial para cada inimigo seguir
    {
        inimigos[i].direcao=rand()%4;
    }
}
void movimento_inimigos(tipo_jogador inimigos[], tipo_mapa *mapa, tipo_jogador *jogador, clock_t *inicio, tipo_bomba bombas[])
{
    int i, j, x, y, aux2, novadir, num_inimigos=0,difx,dify,difbx[MAXEN]= {0},difby[MAXEN]= {0};

    if(conta_tempo_inimigos(inicio)==1)
    {
        textcolor(LIGHTRED);
        for(i=0; i<MAXEN; i++)
        {
            if(inimigos[i].vidas == 1) //Verifica se o inimigo está vivo
            {
                x = inimigos[i].x-1; //Correção para tela conforme funções anteriores
                y = inimigos[i].y-2;
                num_inimigos++;
                if(jogador->nivel == 2)
                {
                    for(j=0; j<NUMBOMBAS; j++) // Roda para verificar todas as bombas
                    {
                        if(bombas[j].acionada==1)  // Calcula a distancia só das bombas acionadas no momento
                        {
                            difbx[i]=bombas[j].tx-inimigos[i].x;
                            difby[i]=bombas[j].ty-inimigos[i].y;
                        }
                        if (bombas[j].acionada==1&&((difbx[i]>-RADAR&&difbx[i]<RADAR)&&(difby[i]>-RADAR&&difby[i]<RADAR))) //Verifica se tem alguma bomba acionada e no raio que o inimigo pode ver
                        {
                            procurabomba(&bombas[j],&inimigos[i],mapa);
                            inimigos[i].auxmudanca=0; //Usei isso como uma flag para impedir que ele persiga o jogador no próximo else, pq talvez a bomba 2 esteja acionado mas a três não, entao preciso ter certeza que ele nao vai mudar
                        }
                        else if (inimigos[i].auxmudanca!=0)   //Caso nao tenha nenhuma bomba no seu raio de percepção verifica se o jogador está
                        {
                            procurajogador(jogador,&inimigos[i],mapa);
                        }
                    }
                }

                difx=jogador->x-inimigos[i].x;
                dify=jogador->y-inimigos[i].y;
                switch (inimigos[i].direcao)
                {
                //Movimentação padrão
                case 0: //PARA CIMA
                    inimigos[i].frente = 'c';
                    if(mapa->tamanho[y-1][x]==jogador->caractere&&dify!=0&&(difx<=RADAR||difx>=-RADAR))//VERIFICA COLISAO COM JOGADOR
                    {

                        novadirecaoinimigos(&inimigos[i]);
                        mortejogador(jogador,mapa,inimigos);

                    }
                    if(mapa->tamanho[y-1][x]==VAZIO)    //Comparo com o mapa, mas tiro um de cada para passar da coordenada 1,1 para 0,0. A subtração adicional é para ir para a proxima posição solicitadoa
                    {
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x,inimigos[i].y-1, ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]); //Dentro dessa função nao precisou do & antes de mapa e jogar pois nesse escopo eles já são ponteiros
                        inimigos[i].y--;
                        inimigos[i].auxmudanca++;

                    }
                    else
                    {
                        novadirecaoinimigos(&inimigos[i]);
                    }
                    break;
                case 1: //PARA BAIXO
                    inimigos[i].frente = 'b';
                    if(mapa->tamanho[y+1][x]==jogador->caractere&&dify!=0&&(difx<=RADAR||difx>=-RADAR))
                    {

                        novadirecaoinimigos(&inimigos[i]);
                        mortejogador(jogador,mapa,inimigos);
                    }
                    if(mapa->tamanho[y+1][x]==VAZIO)
                    {
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x,inimigos[i].y+1, ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]);
                        inimigos[i].y++;
                        inimigos[i].auxmudanca++;

                    }
                    else
                    {

                        novadirecaoinimigos(&inimigos[i]);
                    }
                    break;
                case 2: //PARA ESQUERDA
                    inimigos[i].frente = 'e';
                    if(mapa->tamanho[y][x-1]==jogador->caractere&&difx!=0&&(dify<=RADAR||dify>=-RADAR))
                    {

                        novadirecaoinimigos(&inimigos[i]);
                        mortejogador(jogador,mapa,inimigos);
                    }
                    if(mapa->tamanho[y][x-1]==VAZIO)
                    {
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x-1,inimigos[i].y, ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]);
                        inimigos[i].x--;
                        inimigos[i].auxmudanca++;

                    }
                    else
                    {
                        novadirecaoinimigos(&inimigos[i]);
                    }
                    break;
                case 3: //PARA DIREITA
                    inimigos[i].frente = 'd';
                    if(mapa->tamanho[y][x+1]==jogador->caractere&&difx!=0&&(dify<=RADAR||dify>=-RADAR))
                    {

                        novadirecaoinimigos(&inimigos[i]);
                        mortejogador(jogador,mapa,inimigos);
                    }
                    if(mapa->tamanho[y][x+1]== VAZIO)
                    {
                        putchxy(inimigos[i].x,inimigos[i].y,' ');
                        putchxy(inimigos[i].x+1,inimigos[i].y, ENEMY);
                        atualizamapa(inimigos[i].caractere, mapa, &inimigos[i]);
                        inimigos[i].x++;
                        inimigos[i].auxmudanca++;

                    }
                    else
                    {
                        novadirecaoinimigos(&inimigos[i]);
                    }
                    break;

                }
            }
        }
    }
    textcolor(WHITE);
}
int conta_tempo_inimigos(clock_t *start)
{
    //Funcao de contagem de tempo, recebe apenas a primeira vez que foi chamada a função, que deve ser inicializado fora desta funcao, dai em diante é zerado dentro da func
    clock_t atual;
    float set = (float)10000/VELOCIDADE;
    double dif;
    int esgotou = 0;

    atual=clock();

    dif=1000*((double)(atual-*start)/CLOCKS_PER_SEC);
    if(dif >= set)
    {
        esgotou = 1;
        *start=clock();
    }
    return esgotou;
}
int conta_tempo_bomba(tipo_bomba *bomba)
{
    clock_t atual;
    int esgotou = 0;
    atual=clock();

    bomba->t_decorrido = 1000*((double)(atual-bomba->start)/CLOCKS_PER_SEC);
    if(bomba->t_decorrido >= (double)T_BOMBA)
    {
        esgotou = 1;
    }
    return esgotou;
}
void explode_bomba(tipo_mapa *mapa, tipo_jogador *jogador, tipo_bomba bombas[], tipo_jogador inimigos[])
{
    int i, j, x, y, morte=0;
    for(i=0; i<NUMBOMBAS; i++)
    {
        if(bombas[i].acionada && conta_tempo_bomba(&bombas[i]))
        {
            for(y=bombas[i].my-RANGE; y<=bombas[i].my+RANGE; y++)
            {
                for(x=bombas[i].mx-RANGE; x<=bombas[i].mx+RANGE; x++)
                {
                    switch (mapa->tamanho[y][x])
                    {
                    case 'D':
                    case 'B':
                        mapa->tamanho[y][x] = VAZIO;
                        putchxy(x+1, y+2, VAZIO);
                        jogador->pontos+=10;
                        break;

                    case '@':
                        if(x == bombas[i].mx && y == bombas[i].my){
                            mapa->tamanho[y][x] = VAZIO;
                            putchxy(x+1, y+2, VAZIO);
                        }
                        break;

                    case 'E':
                        for(j=0; j<MAXEN; j++)
                        {
                            if(inimigos[j].x-1 == x && inimigos[j].y-2 == y)
                            {
                                inimigos[j].vidas--;
                                jogador->pontos+=20;
                            }
                        }
                        mapa->tamanho[y][x] = VAZIO;
                        putchxy(x+1, y+2, VAZIO);
                        break;

                    case 'K':
                        textcolor(LIGHTGREEN);
                        mapa->tamanho[y][x] = CHAVE;
                        putchxy(x+1, y+2, CHAVE_TELA);
                        textcolor(WHITE);
                        jogador->pontos+=10;
                        break;

                    case 'J':
                        morte = 1;
                        break;

                    }
                }
            }
            if(morte)
            {
                mortejogador(jogador, mapa, inimigos);
                morte = 0;
            }
            jogador->bombas++;
            bombas[i].acionada = 0;
            status(jogador, inimigos);
        }
    }
}
void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}
void novapos(tipo_jogador* jogador, tipo_mapa* mapa)
{//Função para encontrar uma nova posição para o jogador sempre que ele morrer
    int x, y, i, j, emboscada=0;

    putchxy(jogador->x, jogador->y, VAZIO);
    mapa->tamanho[jogador->y-2][jogador->x-1] = VAZIO;

    do
    {
        x=rand()%mapa->largura; //%60
        y=rand()%mapa->altura; //%25
        for(i=x-1; i<=x+1; i++)
        {
            for(j=y-1; j<=y+1; j++)
            {
                if(mapa->tamanho[j][i] == 'E')
                {
                    emboscada = 1;
                }
            }
        }

    }
    while(mapa->tamanho[y][x] != VAZIO || emboscada);

    mapa->tamanho[y][x] = jogador->caractere;

    jogador->y = y+2;
    jogador->x = x+1;

    textcolor(LIGHTCYAN);
    putchxy(jogador->x, jogador->y, PLAYER);
    textcolor(WHITE);

    aguarda_teclado(); //Pausa o jogo até o jogador realizar mais um movimento
}
void mortejogador(tipo_jogador *jogador, tipo_mapa *mapa, tipo_jogador inimigos[])
{
    jogador->vidas--;
    jogador->pontos-=100;
    if (jogador->vidas>0)
    {
        novapos(jogador,mapa);
    }
    status(jogador, inimigos);

}
void novadirecaoinimigos(tipo_jogador *inimigos)
{
    int aux=0,novadir;
    do
    {
        novadir=rand()%4;
        if (inimigos->direcao!=novadir)
        {
            inimigos->direcao=rand()%4;
            aux=1;
        }
    }
    while(!aux);
    aux=0;
    inimigos->auxmudanca=0;
}
void procurajogador(tipo_jogador *jogador,tipo_jogador *inimigos, tipo_mapa *mapa)
{

    int difx,dify,x,y,diresc=0;
    x = inimigos->x-1;
    y = inimigos->y-2;
    difx=jogador->x-inimigos->x; //VERIFICA A DISTANCIA DO JOGADOR
    dify=jogador->y-inimigos->y;
    if (dify!=0&&(mapa->tamanho[y+1][x]==VAZIO||mapa->tamanho[y-1][x]==VAZIO)) //VERIFICA EM Y SE A DISTANCIA DO JOGADOR TA DENTRO DO RADAR E SE O INIMIGO PODE SE MOVER NESSA DIRECAO/
    {
        if (dify>0&&dify<RADAR&&diresc<2&&mapa->tamanho[y+1][x]==VAZIO&&(difx>0&&difx<RADAR||difx<0&&difx>-RADAR))//Verifica a diferença em Y,se a próxima posição está disponivel para se mexer e se o jogador está no raio de visão do inimigo
        {
            inimigos->direcao=1;
            diresc++;
        }
        else if(dify<0&&dify>-RADAR&&diresc<2&&mapa->tamanho[y-1][x]==VAZIO&&(difx>0&&difx<RADAR||difx<0&&difx>-RADAR))
        {
            inimigos->direcao=0;
            diresc++;
        }
    }
    if (difx!=0&&(mapa->tamanho[y][x-1]== VAZIO||mapa->tamanho[y][x+1]== VAZIO)) //X
    {
        if (difx>0&&difx<RADAR&&diresc<2&&mapa->tamanho[y][x-1]== VAZIO&&(dify>0&&dify<RADAR||dify<0&&dify>-RADAR))
        {
            inimigos->direcao=3;
            diresc++;
        }
        else if(difx<0&&difx>-RADAR&&diresc<2&&mapa->tamanho[y][x+1]== VAZIO&&(dify>0&&dify<RADAR||dify<0&&dify>-RADAR))
        {
            inimigos->direcao=2;
            diresc++;
        }
    }
    if (diresc==1) //PARA IMPEDIR QUE ELE MUDE DE DIRECAO NO MEIO DA PERSEGUICAO
        inimigos->auxmudanca=0;
    if (dify==0)
    {
        if (difx>0)
            inimigos->direcao=3;
        else inimigos->direcao=2;
    }
    if (difx==0)
    {
        if (dify>0)
            inimigos->direcao=1;
        else inimigos->direcao=0;
    }

}

void procurabomba(tipo_bomba* bomba,tipo_jogador* inimigos,tipo_mapa *mapa)
{
//Bomba segue o mesmo padrão de procurar jogador, só que com a escolha final de posição invertida
    int difx,dify,x,y,diresc=0;
    x = inimigos->x-1;
    y = inimigos->y-2;
    difx=bomba->tx-inimigos->x;
    dify=bomba->ty-inimigos->y;

    if (dify!=0&&(mapa->tamanho[y+1][x]==VAZIO||mapa->tamanho[y-1][x]==VAZIO))//Y
    {
        if (dify>0&&dify<RADAR&&diresc<2&&mapa->tamanho[y+1][x]==VAZIO&&(difx>0&&difx<RADAR||difx<0&&difx>-RADAR))//Esse ultimo parenteses é para ter certeza de que ta dentro do raio de atuaçao do inimigo, e que nao ta no mesmo x/y só que do outro lado do mapa
        {
            inimigos->direcao=0;
        }
        else if(dify<0&&dify>-RADAR&&diresc<2&&mapa->tamanho[y-1][x]==VAZIO&&(difx>0&&difx<RADAR||difx<0&&difx>-RADAR))
        {
            inimigos->direcao=1;
        }
    }
    if (difx!=0&&(mapa->tamanho[y][x-1]== VAZIO||mapa->tamanho[y][x+1]== VAZIO)) //X
    {
        if (difx>0&&difx<RADAR&&diresc<2&&mapa->tamanho[y][x-1]== VAZIO&&(dify>0&&dify<RADAR||dify<0&&dify>-RADAR))
        {
            inimigos->direcao=2;
        }
        else if(difx<0&&difx>-RADAR&&diresc<2&&mapa->tamanho[y][x+1]== VAZIO&&(dify>0&&dify<RADAR||dify<0&&dify>-RADAR))
        {
            inimigos->direcao=3;
        }
    }

    if (dify==0)
    {
        if (difx>0)
            inimigos->direcao=2;
        else inimigos->direcao=3;
    }
    if (difx==0)
    {
        if (dify>0)
            inimigos->direcao=0;
        else inimigos->direcao=1;
    }
    /* ESSE CODIGO COMENTADO SE REFERE A IMPEDIR QUE ELE FIQUE TRAVADO EM UM CANTO AO TENTAR FUGIR DA BOMBA, AINDA NAO SEI SE VALE A PENA
      O PROBLEMA QUE ESTOU TENDO É COM A PRIORIDADE DE TROCAR DE DIRECAO, ELE ATÉ MUDA UMA VEZ, MAS NAO SE MANTEM NESSE CASO
      VERIFICAR FORMA DE TRAVAR ISSO
*/
      if((inimigos->direcao==0&&mapa->tamanho[y-1][x]!=VAZIO&&mapa->tamanho[y][x-1]==VAZIO))
          inimigos->direcao=2;
      else if(mapa->tamanho[y][x-1]!=VAZIO&&inimigos->direcao==0&&mapa->tamanho[y][x+1]==VAZIO)
          inimigos->direcao=3;
      else if((inimigos->direcao==1&&mapa->tamanho[y+1][x]!=VAZIO&&mapa->tamanho[y][x-1]==VAZIO))
          inimigos->direcao=2;
      else if(mapa->tamanho[y][x-1]!=VAZIO&&inimigos->direcao==1&&mapa->tamanho[y][x+1]==VAZIO)
          inimigos->direcao=3;
      else if(inimigos->direcao==2&&mapa->tamanho[y][x-1]!=VAZIO&&mapa->tamanho[y-1][x]==VAZIO)
          inimigos->direcao=0;
      else if(mapa->tamanho[y-1][x]!=VAZIO&&inimigos->direcao==2&&mapa->tamanho[y+1][x]==VAZIO)
          inimigos->direcao=1;
      else if((inimigos->direcao==3)&&mapa->tamanho[y][x+1]!=VAZIO&&mapa->tamanho[y-1][x]==VAZIO)
          inimigos->direcao=0;
      else if(mapa->tamanho[y-1][x]!=VAZIO&&inimigos->direcao==3&&mapa->tamanho[y+1][x]==VAZIO)
          inimigos->direcao=1;
      }
