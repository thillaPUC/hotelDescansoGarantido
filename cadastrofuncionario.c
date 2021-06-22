#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>
 
int main(){
	char nome[50];
  char cargo[20];
  char telefone[20];
	int codigo = rand();
	float salario;
  int resp;
  FILE *arq;

 arq = fopen("funcionario.txt", "w");

  do
  {
    srand(time(NULL));
  
  for (codigo=0; codigo < 1; codigo++)
  {
    printf(" \nCódigo: %d ", rand() % 100);
  } 

	do{
		printf("\nDigite seu nome: ");
		setbuf(stdin, NULL);
		scanf("%50s", nome);
	}while( strlen(nome) < 4);
 
	do{
		printf("Digite seu cargo: ");
		setbuf(stdin, NULL);
		scanf("%20s", cargo);
	}while( strlen(cargo) < 4);
 
	do{
		printf("Digite seu salário: ");
		setbuf(stdin, NULL);
		scanf("%fs", &salario);
	}while( salario <= 0);

 do{
		printf("Digite seu telefone: ");
		setbuf(stdin, NULL);
		scanf("%20s", telefone);
	}while( strlen(telefone) < 9);


  fprintf(arq, "\n Código: %d ", rand() % 100);
	fprintf(arq,"\nNome: %s", nome);
	fprintf(arq,"\nCargo: %s", cargo);
  fprintf(arq,"\nSalario: %f", salario);
  fprintf(arq,"\nTelefone: %s", telefone);


    printf("\nPara cadastrar novamente digite 1 para sair digite 2: ");
    scanf("%d",&resp);
  
  }while (resp==1);

  fclose(arq);
  
  return 0;
		}