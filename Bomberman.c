#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
//#define _NOCURSOR

typedef struct
{
    int x, y;
} tipo_pos;

typedef struct
{
    char tamanho[25][62];
    int largura, altura;
} tipo_mapa;


void imprime(tipo_mapa mapa);
void inicializa(char nome_arquivo[], tipo_mapa *mapa);
tipo_pos encontra (char personagem, tipo_mapa mapa);
void movimenta(tipo_pos *jogador, int direcao, char c, tipo_mapa* mapa);
char teclado();
int menu(int inicio, tipo_mapa *mapa, tipo_pos *jogador);
void apaga_linha();
void atualizamapa(char c, tipo_mapa *mapa, tipo_pos *jogador, char sentido);
void aguarda_teclado();

int main()
{
    int direcao, endgame = 0;
    tipo_pos jogador;
    char nome_arquivo[30] = "mapa_fase1.txt", c = 'j';
    tipo_mapa mapa;
    textcolor(WHITE);
    /*printf("Entre o nome do arquivo que deseja abrir(sem o .txt no final):");
    gets(f); //fazer sem gets depois
    //strcpy(mapa,"mapa.txt");
    strcat(f,".txt");
    printf("Entre o caractere que deseja encontrar: ");
    scanf("%c",&c);*/

    inicializa(nome_arquivo, &mapa);
    jogador = encontra(c, mapa);
    //clrscr();
    menu(1, &mapa, &jogador);
    imprime(mapa);
    //printf("\nO jogador se encontra em %d x e %d y\n", jogador.x, jogador.y);
    //gotoxy(jogador.x+1,jogador.y+1);

    while(!endgame)
    {
        direcao = getch();
        if(direcao != 27)
        {
            movimenta(&jogador, direcao, c, &mapa);

        }
        else
        {
            endgame = menu(0, &mapa, &jogador);

        }
    }
    system("pause");
    return 0;
}

int menu(int inicio, tipo_mapa *mapa, tipo_pos *jogador) //RETORNA 1 SE JOGO DEVE ACABAR
{
    FILE *save;
    char c, save_num, nome_save[20] = "save_0.txt", lista_saves[9][20];
    int num_saves=0, i, endgame = 0;
    if(inicio)
    {
        textcolor(LIGHTRED);
        printf  ("############################################################");
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
        aguarda_teclado();
        clrscr();

    }
    else
    {
start:
        gotoxy(1,26);
        textcolor(LIGHTGREEN);
        printf("(N)New Game  (L)Load Game  (S)Save  (Q)Quit Game   (R)Return");
        textcolor(WHITE);

        aguarda_teclado();
        //c = teclado();
        switch(teclado()){
            case 'N':
            case 'n':
                clrscr();
                inicializa("mapa_fase1.txt", mapa);
                imprime(*mapa);
                *jogador = encontra('j', *mapa);
                break;

            case 'L':
            case 'l':
                //CHAMAR LEITURA DE MAPA JA SALVO
                i=0;
                do{ //TESTA QUANTOS SAVES JA EXISTEM
                    i++;
                    save_num = i + '0';
                    nome_save[5] = save_num;
                    save = fopen(nome_save, "r");
                    if(save != NULL){
                        strcpy(lista_saves[i-1], nome_save); //COPIA O NOME DO SAVE PARA UMA STRING
                        num_saves++;
                        fclose(save);
                    }
                }while(save != NULL);

                if(num_saves > 0){
                    textcolor(LIGHTCYAN);
                    for(i=0; i<num_saves; i++){ //IMPRIME A LISTA DE SAVES A DIRETIA DA TELA DO JOGO
                        gotoxy(62, 3*i+1);
                        printf("(%d)%s", i+1, lista_saves[i]);
                    }
                    textcolor(WHITE);

                    load:
                    aguarda_teclado();
                    save_num = teclado();

                    if(save_num<'1' || (save_num - '0')>num_saves){ //TESTA SE O USUARIO ESCOLHEU UM SAVE VALIDO, ENTRE 1 E NUM_SAVES
                        goto load;
                    }

                    nome_save[5] = save_num;
                    clrscr();

                    inicializa(nome_save, mapa);
                    imprime(*mapa);
                    *jogador = encontra('j', *mapa);

                }else{
                    gotoxy(1,26);
                    textcolor(LIGHTGREEN);
                    printf("\r                       NO SAVED GAMES\t\t\t\t");
                    textcolor(WHITE);
                }

                break;

            case 'S':
            case 's':
                //ESCREVER PONTUACAO EM ARQUIVO .bin
                i=0;
                do{ //TESTA QUANTOS SAVES JA EXISTEM
                    i++;
                    save_num = i + '0';
                    nome_save[5] = save_num;
                    save = fopen(nome_save, "r");
                    if(save != NULL){
                        fclose(save);
                    }
                }while(save != NULL);

                save = fopen(nome_save, "w");

                for(i=0; i<mapa->altura; i++){
                    fputs(mapa->tamanho[i], save);

                }
                fclose(save);
                textcolor(LIGHTGREEN);
                printf("\r                  GAME SAVED SUCCESSFULLY\t\t\t\t\t");
                textcolor(WHITE);
                break;

            case 'R':
            case 'r':
                apaga_linha();
                break;

            case 'Q':
            case 'q':
                textcolor(LIGHTRED);
                printf("\r                        ENDING GAME\t\t\t\t\t\t\n");
                textcolor(WHITE);
                endgame = 1;
                break;

            default:
                goto start;
        }
        if(!endgame){
            gotoxy(jogador->x, jogador->y);
        }
    }

    return endgame;
}

void aguarda_teclado(){
    while(!kbhit()){}
}

void apaga_linha()
{
    printf("\r\t\t\t\t\t\t\t\t");
}

char teclado()
{
    char c;
    if(kbhit())
    {
        c = getch();
    }
    //printf("%c", c);
    return c;
}

void inicializa(char nome_arquivo[], tipo_mapa *mapa)
{
    int i=0;
    FILE *arquivo_mapa;
    char texto[62];
    arquivo_mapa = fopen(nome_arquivo, "r");
    if(arquivo_mapa == NULL)
    {
        printf("ERRO AO ABRIR MAPA!\n");

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

void imprime(tipo_mapa mapa)
{
    int i, j;
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

tipo_pos encontra(char personagem, tipo_mapa mapa)
{
    int i=0, j=0;
    tipo_pos posicao;
    //for(i=0; i<mapa.altura; i++){
    //for(j=0; j<mapa.largura; j++){
    do
    {
        do
        {
            if(mapa.tamanho[i][j] == personagem)
            {
                posicao.x = j+1;//+1 em ambos para ficar conforme as coordenadas da tela, que começa em 1,1
                posicao.y = i+1;
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

    return posicao;
}

void movimenta(tipo_pos *jogador, int direcao, char c, tipo_mapa* mapa)
{
    //fflush(stdin); //gotoxy(0,0);
    int teste;
    switch (direcao)
    {
    case 119://W
    case 72: //PARA CIMA
        if(mapa->tamanho[jogador->y-2][jogador->x-1]==' ')   //Comparo com o mapa, mas tiro um de cada para passar da coordenada 1,1 para 0,0. A subtração adicional é para ir para a proxima posição solicitadoa
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y-1, c);
            atualizamapa(c, mapa, jogador, 'c'); //Dentro dessa função nao precisou do & antes de mapa e jogar pois nesse escopo eles já são ponteiros
            jogador->y--;

        }
        break;
    case 115://S
    case 80: //PARA BAIXO
        if(mapa->tamanho[jogador->y][jogador->x-1]==' ')
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y+1,c);
            atualizamapa(c, mapa, jogador, 'b');
            jogador->y++;

        }
        break;
    case 97: //A
    case 75: //PARA ESQUERDA
        if(mapa->tamanho[jogador->y-1][jogador->x-2]==' ')
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x-1,jogador->y,c);
            atualizamapa(c, mapa, jogador, 'e');
            jogador->x--;

        }
        break;
    case 100://D
    case 77: //PARA DIREITA
        if(mapa->tamanho[jogador->y-1][jogador->x]==' ')
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x+1,jogador->y,c);
            atualizamapa(c, mapa, jogador, 'd');
            jogador->x++;
        }
        break;
    }
}

void atualizamapa(char c, tipo_mapa *mapa, tipo_pos *jogador, char sentido)
{
    mapa->tamanho[jogador->y-1][jogador->x-1]=' ';
    switch(sentido)
    {
    case 'c':
        mapa->tamanho[jogador->y-2][jogador->x-1]=c;
        break;

    case 'b':
        mapa->tamanho[jogador->y][jogador->x-1]=c;
        break;

    case 'e':
        mapa->tamanho[jogador->y-1][jogador->x-2]=c;
        break;

    case 'd':
        mapa->tamanho[jogador->y-1][jogador->x]=c;
        break;
    }
}
