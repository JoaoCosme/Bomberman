#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>
#include <windows.h>
#define _NOCURSOR

typedef struct pos_st{
    int x,y;
}tipo_pos;

typedef struct mapa{
    char tamanho[100][100];
    int largura,altura;
}tipo_mapa;


void imprime(tipo_mapa f);
void inicializa(char f[], tipo_mapa *atual);
tipo_pos encontra (char tipo, tipo_mapa atual);
void atualiza(tipo_mapa atual, tipo_pos jogador);
void movimenta(tipo_pos *jogador, int direcao, char c, tipo_mapa atual);
char teclado();
int menu();


int main(){
    int direcao, endgame = 0;
    tipo_pos jogador;
    char f[100] = "mapa.txt", c = '&';
    tipo_mapa atual;

    /*printf("Entre o nome do arquivo que deseja abrir(sem o .txt no final):");
    gets(f); //fazer sem gets depois
    //strcpy(atual,"Atual.txt");
    strcat(f,".txt");
    printf("Entre o caractere que deseja encontrar: ");
    scanf("%c",&c);*/

    inicializa(f, &atual);
    clrscr();
    imprime(atual);
    jogador = encontra(c, atual);
    //printf("\nO jogador se encontra em %d x e %d y\n", jogador.x, jogador.y);
    //gotoxy(jogador.x+1,jogador.y+1);

    while(!endgame){
        direcao = getch();
        if(direcao != 27){
            movimenta(&jogador, direcao, c, atual);

        }else{
            endgame = menu();
            //gotoxy(1, 14);
            //printf("%d", endgame);

        }
    }
    //getch();
    //clrscr();
    return 0;
}


int menu(){//RETORNA 1 SE JOGO DEVE ACABAR
    char c;
    //CHAMAR FUNCAO DE PAUSA DO JOGO AQUI
    gotoxy(1,13);
    printf("(n)Novo Jogo  ");
    printf("(c)Carregar Jogo  ");
    printf("(s)Salvar Jogo  ");
    printf("(q)Sair do Jogo  ");
    printf("(v)Voltar para o Jogo");

    while(!kbhit()){}
    c = teclado();
    switch(c){
        case 'n':
            printf("\r\t\t\t\t\tNOVO JOGO\t\t\t\t\t");
            //CHAMAR LEITURA DO ARQUIVO MAPA
            break;

        case 'c':
            printf("\r\t\t\t\t\tCARREGAR JOGO\t\t\t\t\t");
            //CHAMAR LEITURA DE MAPA JA SALVO
            break;

        case 's':
            printf("\r\t\t\t\t\tSALVAR JOGO\t\t\t\t\t");
            //ESCREVER MAPA EM ARQUIVO .txt E PONTUACAO EM ARQUIVO .bin
            break;

        case 'q':
            printf("\r\t\t\t\t\tSAIR DO JOGO\t\t\t\t\t");
            return 1;

        case 'v':
            printf("\r\t\t\t\t\tVOLTAR AO JOGO\t\t\t\t\t");
            break;

        default:
            menu();
    }
    return 0;
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
    mapa=fopen(f,"r");
    //atual=fopen("Atual.txt","w+");
    while (fgets(texto,100,mapa)!=NULL)
    {
        strcpy(atual->tamanho[i],texto);
        atual->largura=strlen(atual->tamanho[i]);
        i++;
    }
    //fputs(texto,atual);
    fclose(mapa);
    //fclose(atual);
    atual->altura=i;
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
    for(i=0; i<f.altura; i++)
        printf("%s",f.tamanho[i]);

}

tipo_pos encontra(char tipo, tipo_mapa atual){
    int i=0,j=0;
    tipo_pos posicao;
    do{
        do{
            if(atual.tamanho[i][j]==tipo){
                posicao.x=j+1;//+1 em ambos para poder atualizar na tela
                posicao.y=i+1;
                break;
            }
            j++;
        }while(j<atual.largura);
        j=0;
        i++;
    }while(i<atual.altura);

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
    /*if (direcao==72){//&&jogador->y-1==' '){//Para cima
        putchxy(jogador->x,jogador->y,' ');
        putchxy(jogador->x,jogador->y-1,'&');
        jogador->y--;
    }
    if (direcao==80){ //Para baixo
        putchxy(jogador->x,jogador->y,' ');
        putchxy(jogador->x,jogador->y+1,'&');
        jogador->y++;
    }
    if (direcao==75){ //Pro lado esquerdo
        putchxy(jogador->x,jogador->y,' ');
        putchxy(jogador->x-1,jogador->y,'&');
        jogador->x--;
    }
    if (direcao==77){ //Pro outro lado esquerdo
        putchxy(jogador->x,jogador->y,' ');
        putchxy(jogador->x+1,jogador->y,'&');
        jogador->x++;
    }*/
}
