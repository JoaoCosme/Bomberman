#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<windows.h>

typedef struct pos_st
{
    int x,y;
} pos;

void imprime(char f[100][100]);
void inicializa(char f[],char atual[100][100]);
pos encontra (char tipo,char f[]);
int main()
{
    pos jogador;
    char c,f[100],atual[100][100];
    printf("Entre o nome do arquivo que deseja abrir(sem o .txt no final):");
    gets(f); //fazer sem gets depois
    //strcpy(atual,"Atual.txt");
    strcat(f,".txt");
    printf("Entre o caractere que deseja encontrar: ");
    scanf("%c",&c);
    inicializa(f,atual);
    jogador=encontra(c,atual);
    printf("\nO jogador se encontra em %d x e %d y\n",jogador.x,jogador.y);

     while(1)
     {
         imprime(atual);
         //Sleep(100); Só serve para o windows tambem
         system("cls"); //Só serve para o windows, depois podemos trocar por outra função
     }

    return 0;
}

void inicializa(char f[],char atual[100][100])
{
    int i=0;
    FILE *mapa;//*atual;
    char texto[100];
    mapa=fopen(f,"r");
    //atual=fopen("Atual.txt","w+");
    while (fgets(texto,100,mapa)!=NULL)
        {
        strcpy(atual[i],texto);
        i++;
        }
        //fputs(texto,atual);
    fclose(mapa);
    //fclose(atual);
}

void imprime(char f[100][100])
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
    for(i=0;i<50;i++)
        printf("%s",f[i]);
}

pos encontra (char tipo,char f[])
{
    pos caractere;
    caractere.y=0; //Por algum motivo se eu nao declaro o .y como 0 aqui da erro
    int i=0,xt,colunas=0,enc=0;
    FILE *mapa;
    char texto[100]= { };
    mapa=fopen(f,"r");
    while (fgets(texto,100,mapa)!=NULL) //Essa função trata linha do arquivo como uma string, entao tanto para imprimir para quanto buscar tem que ter essa mentalidade
    {
        xt=strlen(texto);
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
