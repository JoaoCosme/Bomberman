#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>


typedef struct pos_st
{
    int x,y;
} pos;

void imprime(char f[]);
pos encontra (char tipo,char f[]);
int main()
{
    pos jogador;
    char c,f[100];
    printf("Entre o nome do arquivo que deseja abrir(sem o .txt no final):");
    gets(f); //fazer sem gets depois
    strcat(f,".txt");
    printf("Entre o caractere que deseja encontrar: ");
    scanf("%c",&c);
    imprime(f);
    jogador=encontra(c,f);
    printf("\nO jogador se encontra em %d x e %d y\n",jogador.x,jogador.y);
    return 0;
}

void imprime(char f[])
{
    FILE *mapa;
    char texto[100];
    mapa=fopen(f,"r");
    while (fgets(texto,100,mapa)!=NULL)
    fclose(mapa);
}

pos encontra (char tipo,char f[]){
    pos caractere;
    caractere.y=0; //Por algum motivo se eu nao declaro o .y como 0 aqui da erro
    int i=0,xt,colunas=0,enc=0;
    FILE *mapa;
    char texto[100]={ };
    mapa=fopen(f,"r");
    while (fgets(texto,100,mapa)!=NULL) //Essa função trata linha do arquivo como uma string, entao tanto para imprimir para quanto buscar tem que ter essa mentalidade
    {
        xt=strlen(texto);
        printf("%s",texto);
        colunas++;
        if(!enc)
        {
            while(texto[i]!='\n'&&texto[i]!=tipo)
            {
                if (texto[i+1]==tipo)
                {
                    enc=1;
                    break;
                }
                caractere.x++;
                if (texto[i+1]=='\n')
                {
                    caractere.y++;
                    caractere.x=0;
                }
                i++;
            }
        }
        if (i==0&&!enc)   //O unico caso daqui ter um i=0 aqui seria se fosse uma linha vazia, se nao tiver no jogo da para tirar esse if
        {
            caractere.y++;
        }
        i=0;
    }
    printf("\n");
    printf("O arquivo tem: %d Comprimento e %d Altura\n",xt,colunas);
    fclose(mapa);
    return caractere;
}
