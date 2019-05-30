#include<stdio.h>
#include<stdlib.h>
#include<conio2.h>
#include<string.h>
#include<windows.h>
#define _NOCURSOR
typedef struct pos_st
{
    int x,y;
} pos;
typedef struct mapa
{
    char tamanho[100][100];
    int largura,altura;
} mapa;

void imprime(mapa f);
void inicializa(char f[],mapa* atual);
pos encontra (char tipo,mapa atual);
void atualiza(mapa atual,pos jogador);
void movimenta(pos* jogador, int* i,int direcao,char c, mapa atual);
int main()
{
    int direcao;
    pos jogador;
    char c,f[100];
    mapa atual;
    printf("Entre o nome do arquivo que deseja abrir(sem o .txt no final):");
    gets(f); //fazer sem gets depois
    //strcpy(atual,"Atual.txt");
    strcat(f,".txt");
    printf("Entre o caractere que deseja encontrar: ");
    scanf("%c",&c);
    inicializa(f,&atual);
    clrscr();
    imprime(atual);
    jogador=encontra(c,atual);
    //printf("\nO jogador se encontra em %d x e %d y\n",jogador.x,jogador.y);
    int i=0;
    //gotoxy(jogador.x+1,jogador.y+1);
    while(!i){
            direcao=getch();
    movimenta(&jogador,&i,direcao,c,atual);
    }
    getch();
    return 0;
}

void inicializa(char f[],mapa* atual)
{
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

void imprime(mapa f)
{
    /*
    FILE *atual;
    char texto[100];
    atual=fopen("Atual.txt","r+");
    while (fgets(texto,100,atual)!=NULL)
        printf("%s",texto);
    fclose(atual);
    */
    int i=0;
    for(i=0; i<f.altura; i++)
        printf("%s",f.tamanho[i]);

}

pos encontra(char tipo, mapa atual)
{
    int i=0,j=0;
    pos posicao;
    do
    {
        do
        {
            if (atual.tamanho[i][j]==tipo)
            {
                posicao.x=j+1;//+1 em ambos para poder atualizar na tela
                posicao.y=i+1;
                break;
            }
            j++;
        }
        while(j<atual.largura);
        j=0;
        i++;
    }
    while(i<atual.altura);
    return posicao;
}

void movimenta(pos* jogador, int* i,int direcao,char c,mapa atual){
    //fflush(stdin); //gotoxy(0,0);
    if (direcao==72){//&&jogador->y-1==' '){//Para cima
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
    }

}
