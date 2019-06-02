#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
//#define _NOCURSOR

typedef struct pos_st{
    int x, y;
}tipo_pos;

typedef struct mapa{
    char tamanho[100][100];
    int largura, altura;
}tipo_mapa;


void imprime(tipo_mapa f);
void inicializa(char f[], tipo_mapa *atual);
tipo_pos encontra (char tipo, tipo_mapa atual);
void atualiza(tipo_mapa atual, tipo_pos jogador);
void movimenta(tipo_pos *jogador, int direcao, char c, tipo_mapa atual);
char teclado();
int menu(int inicio, tipo_mapa *atual, tipo_pos *jogador);


int main(){
    int direcao, endgame = 0;
    tipo_pos jogador;
    char f[100] = "mapabeta.txt", c = '&';
    tipo_mapa atual;
    textcolor(WHITE);
    /*printf("Entre o nome do arquivo que deseja abrir(sem o .txt no final):");
    gets(f); //fazer sem gets depois
    //strcpy(atual,"Atual.txt");
    strcat(f,".txt");
    printf("Entre o caractere que deseja encontrar: ");
    scanf("%c",&c);*/

    inicializa(f, &atual);
    jogador = encontra(c, atual);
    //clrscr();
    menu(1, &atual, &jogador);
    imprime(atual);
    //printf("\nO jogador se encontra em %d x e %d y\n", jogador.x, jogador.y);
    //gotoxy(jogador.x+1,jogador.y+1);

    while(!endgame){
        direcao = getch();
        if(direcao != 27){
            movimenta(&jogador, direcao, c, atual);

        }else{
            endgame = menu(0, &atual, &jogador);

        }
    }
    return 0;
}

int menu(int inicio, tipo_mapa *atual, tipo_pos *jogador){//RETORNA 1 SE JOGO DEVE ACABAR
    char c;
    int endgame = 0;
    if(inicio){
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
        while(!kbhit()){}
        clrscr();

    }else{
        start:
        gotoxy(1,26);
        textcolor(LIGHTGREEN);
        printf(" (N)New Game  (L)Load Game  (S)Save  (Q)Quit Game  (R)Return");
        textcolor(WHITE);

        while(!kbhit()){}
        c = teclado();
        switch(c){
            case 'n':
                clrscr();
                inicializa("mapabeta.txt", atual);
                imprime(*atual);
                *jogador = encontra('&', *atual);
                gotoxy(1,26);
                //printf("\r\t\t\t\t\tNOVO JOGO\t\t\t\t\t");
                break;

            case 'l':
                //printf("\r\t\t\t\t\tCARREGAR JOGO\t\t\t\t\t");
                //CHAMAR LEITURA DE MAPA JA SALVO
                clrscr();
                inicializa("mapajogo.txt", atual);
                imprime(*atual);
                *jogador = encontra('&', *atual);
                gotoxy(1,26);
                break;

            case 's':
                printf("\r\t\t\t\t\tSALVAR JOGO\t\t\t\t\t");
                //ESCREVER MAPA EM ARQUIVO .txt E PONTUACAO EM ARQUIVO .bin
                break;

            case 'r':
                printf("\r\t\t\t\t\tVOLTAR AO JOGO\t\t\t\t\t");
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
    }
    return endgame;
}

char teclado(){
    char c;
    if(kbhit()){
        c = getch();
    }
    //printf("%c", c);
    return c;
}

void inicializa(char f[], tipo_mapa *atual){
    int i=0;
    FILE *mapa;//*atual;
    char texto[100];
    mapa = fopen(f, "r");
    //atual=fopen("Atual.txt","w+");
    while(!feof(mapa)){
        fgets(texto, 100, mapa);
        strcpy(atual->tamanho[i], texto);
        i++;
    }
    //fputs(texto,atual);
    fclose(mapa);
    //fclose(atual);
    atual->largura = strlen(atual->tamanho[0]) - 1;
    atual->altura = i;
    //printf("Largura: %d\tAltura: %d", atual->largura, atual->altura);
}

void imprime(tipo_mapa f){
    int i;
    /*
    FILE *atual;
    char texto[100];
    atual=fopen("Atual.txt","r+");
    while (fgets(texto,100,atual)!=NULL)
        printf("%s",texto);
    fclose(atual);
    */
    for(i=0; i<f.altura; i++){
        printf("%s",f.tamanho[i]);

    }

}

tipo_pos encontra(char personagem, tipo_mapa atual){
    int i, j;
    tipo_pos posicao;
    for(i=0; i<atual.altura; i++){
        for(j=0; j<atual.largura; j++){
            if(atual.tamanho[i][j] == personagem){
                posicao.x = j+1;//+1 em ambos para poder atualizar na tela
                posicao.y = i+1;
                //printf("\t%d %d\n", posicao.x, posicao.y);
                break;
            }
            //j++;
        }//while(j<atual.largura);
        //j=0;
        //i++;
    }//while(i<atual.altura);

    return posicao;
}

void movimenta(tipo_pos *jogador, int direcao, char c, tipo_mapa atual){
    //fflush(stdin); //gotoxy(0,0);
    switch (direcao){
        case 119:
        case 72: //PARA CIMA
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y-1,'&');
            jogador->y--;
            break;
        case 115:
        case 80: //PARA BAIXO
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x,jogador->y+1,'&');
            jogador->y++;
            break;
        case 97:
        case 75: //PARA ESQUERDA
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x-1,jogador->y,'&');
            jogador->x--;
            break;
        case 100:
        case 77: //PARA DIREITA
            putchxy(jogador->x,jogador->y,' ');
            putchxy(jogador->x+1,jogador->y,'&');
            jogador->x++;
            break;
    }
}
