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
void atualizamapa(char c,tipo_mapa* mapa,tipo_pos* jogador,char sentido);


int main()
{
    int direcao, endgame = 0;
    tipo_pos jogador;
    char nome_arquivo[30] = "mapabeta.txt", c = '&';
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
    return 0;
}

int menu(int inicio, tipo_mapa *mapa, tipo_pos *jogador) //RETORNA 1 SE JOGO DEVE ACABAR
{
    char c;
    int endgame = 0;
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
        while(!kbhit()) {}
        clrscr();

    }
    else
    {
start:
        gotoxy(1,26);
        textcolor(LIGHTGREEN);
        printf("(N)New Game  (L)Load Game  (S)Save  (Q)Quit Game   (R)Return");
        textcolor(WHITE);

        while(!kbhit()) {}
        c = teclado();
        switch(c)
        {
        case 'n':
            clrscr();
            inicializa("mapabeta.txt", mapa);
            imprime(*mapa);
            *jogador = encontra('&', *mapa);
            //printf("\r\t\t\t\t\tNOVO JOGO\t\t\t\t\t");
            break;

        case 'l':
            //printf("\r\t\t\t\t\tCARREGAR JOGO\t\t\t\t\t");
            //CHAMAR LEITURA DE MAPA JA SALVO
            clrscr();
            inicializa("mapajogo.txt", mapa);
            imprime(*mapa);
            *jogador = encontra('&', *mapa);
            break;

        case 's':
            printf("\r\t\t\t\t\tSALVAR JOGO\t\t\t\t\t");
            //ESCREVER MAPA EM ARQUIVO .txt E PONTUACAO EM ARQUIVO .bin
            break;

        case 'r':
            //printf("\r                       RETURNING GAME\t\t\t\t\t\t\t");
            apaga_linha();
            break;

        case 'q':
            textcolor(RED);
            printf("\r                        ENDING GAME\t\t\t\t\t\t\t");
            textcolor(WHITE);
            endgame = 1;
            break;

        default:
            goto start;
        }
        if(!endgame)
        {
            gotoxy(jogador->x, jogador->y);
        }
    }

    return endgame;
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
    int i;
    for(i=0; i<mapa.altura; i++)
    {
        printf("%s", mapa.tamanho[i]);

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
            putchxy(jogador->x,jogador->y-1,c);
            atualizamapa(c,mapa,jogador,'C'); //Dentro dessa função nao precisou do & antes de mapa e jogar pois nesse escopo eles já são ponteiros
            jogador->y--;

        }
        break;
    case 115://S
    case 80: //PARA BAIXO
        if(mapa->tamanho[jogador->y][jogador->x-1]==' ')
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y+1,c);
            atualizamapa(c,mapa,jogador,'B');
            jogador->y++;

        }
        break;
    case 97: //A
    case 75: //PARA ESQUERDA
        if(mapa->tamanho[jogador->y-1][jogador->x-2]==' ')
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x-1,jogador->y,c);
            atualizamapa(c,mapa,jogador,'E');
            jogador->x--;

        }
        break;
    case 100://D
    case 77: //PARA DIREITA
        if(mapa->tamanho[jogador->y-1][jogador->x]==' ')
        {
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x+1,jogador->y,c);
            atualizamapa(c,mapa,jogador,'D');
            jogador->x++;
        }
        break;
    }
}

void atualizamapa(char c,tipo_mapa* mapa,tipo_pos* jogador,char sentido)
{
    mapa->tamanho[jogador->y-1][jogador->x-1]=' ';
    switch(sentido)
    {
    case 'c':
        mapa->tamanho[jogador->y-2][jogador->x-1]=c;
    case 'b':
        mapa->tamanho[jogador->y][jogador->x-1]=c;
    case 'e':
        mapa->tamanho[jogador->y-1][jogador->x-2]=c;
    case 'd':
        mapa->tamanho[jogador->y-1][jogador->x]=c;
    }
}
