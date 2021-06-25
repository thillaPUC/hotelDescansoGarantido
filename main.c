#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

void cadastrarEstadia();
void cadastrarQuarto();
void cadastrarFuncionario();
void cadastrarCliente();
int checarCodigoQuarto(char codigo[100]);
void pesquisarClienteEstadia(char nomeCliente[], char codCliente[]);
void pesquisarDadosQuartos(char numHospedes[], char codQuarto[]);
int verificaMes(int mes);
void lerEstadia();
void pesquisarEstadia();
void pontosFidelidade();
void pesquisarCadastros();
void listarCadastros();
int checarCodigo(int tarefa,char codigo[100]);

//function do Menu principal
enum opcoes {SAIR, CLIENTE, FUNCIONARIO, QUARTO, ESTADIA, PESQUISAR_CADASTROS, LISTAR_CADASTROS, EXIBIR_ESTADIAS, PESQUISAR_ESTADIA, PONTOS_FIDELIDADE, CHECKOUT};


int selectFunctions(int select)
{
    switch(select)
    {
    case CLIENTE:
        puts("\nCadastrar cliente");
        cadastrarCliente();
        break;
    case FUNCIONARIO:
        puts("\nCadastrar funcionario");
        cadastrarFuncionario();
        break;
    case QUARTO:
        puts("\nCadastrar quarto");
        cadastrarQuarto();
        break;
    case ESTADIA:
        puts("\nCadastrar estadia");
        cadastrarEstadia();
        break;
    case PESQUISAR_CADASTROS:
        puts("\nFazer pesquisa de cadastros");
        pesquisarCadastros();
        break;
        case LISTAR_CADASTROS:
        puts("\nListar cadastros");
        listarCadastros();
        break;
    case EXIBIR_ESTADIAS:
        puts("\nExibir as estadias");
        break;
    case PESQUISAR_ESTADIA:
        puts("\nFazer pesquisa de estadia");
        pesquisarEstadia();
        break;
    case PONTOS_FIDELIDADE:
        puts("\nCalcular pontos fidelidade");
        pontosFidelidade();
        break;
    case CHECKOUT:
        puts("\nFazer checkout");
        break;
    case SAIR:
        return 0;
        break;
    default:
        puts("\nDigite uma opcao valida");
        break;
    }
    return 0;
}

//function do Cadastro de Estadia
typedef struct Estadia{
    char codigo[100];
    char codCliente[100];
    char codQuarto[100];
    int quantidadeDiarias;
    char dataEntrada[100];
    char dataSaida[100];
} Estadia;

void cadastrarEstadia(){
    int i, calc, calc2, totalMes, diaEnt, mesEnt, diaSai, mesSai, qntDiaria;
    char codEstadia[100], nomeCliente[100], codCliente[100], numHospedes[100], codQuarto[5], dataEntrada[6], dataSaida[6], str[4];
    FILE *estadias;
    struct Estadia *estadia = malloc(sizeof(struct Estadia));

    getchar();
    printf("\n Informe um codigo para esta estadia:\n");
    fgets(codEstadia, sizeof(codEstadia), stdin);
    strcpy(estadia->codigo,codEstadia);

    printf("\n Informe o nome do Cliente conforme cadastro de clientes:\n");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    pesquisarClienteEstadia(nomeCliente, codCliente);
    strcpy(estadia->codCliente,codCliente);

    printf("\n Informe o numero total de hospedes:\n");
    fgets(numHospedes, sizeof(numHospedes), stdin);
    pesquisarDadosQuartos(numHospedes, codQuarto);
    strcpy(estadia->codQuarto,codQuarto);

    printf("\nSo aceitando reservas para o ano corrente.\n");
    printf("\n Informe o dia e, em seguida, o mes da data de chegada (Obs.: nao coloque 0 na frente do algorismo):\n");
    scanf("%d %d", &diaEnt, &mesEnt);

    printf("\n Informe o dia e, em seguida, o mes da data de saida (Obs.: nao coloque 0 na frente do algorismo):\n");
    scanf("%d %d", &diaSai, &mesSai);

    qntDiaria = 0;
    totalMes = verificaMes(mesEnt);
    calc = totalMes - diaEnt;

    if(mesSai-mesEnt > 1){
        for(i = mesSai-1; i > mesEnt; i--){
            totalMes = verificaMes(i);
            calc += totalMes;
        }
        qntDiaria = diaSai+calc;
    } else if (mesSai == mesEnt){
        calc2 = totalMes - diaSai;
        qntDiaria = calc-calc2;
    } else {
        qntDiaria = diaSai+calc;
    }

    estadia->quantidadeDiarias = qntDiaria;

    sprintf(str, "%d", diaEnt);
    strcpy(dataEntrada, str);
    strcat(dataEntrada, "-");
    sprintf(str, "%d", mesEnt);
    strcat(dataEntrada, str);

    sprintf(str, "%d", diaSai);
    strcpy(dataSaida, str);
    strcat(dataSaida, "-");
    sprintf(str, "%d", mesSai);
    strcat(dataSaida, str);

    strcpy(estadia->dataEntrada,dataEntrada);
    strcpy(estadia->dataSaida,dataSaida);

    estadias = fopen("estadias.txt", "wb");
    if (estadias != NULL) {
       fwrite(estadia, sizeof(struct Estadia), 1, estadias);
       fclose(estadias);
    }

    //todo: mudar status do quarto para ocupado

    lerEstadia();
};

void lerEstadia(){
    struct Estadia *estadia = malloc(sizeof(struct Estadia));
    FILE * file= fopen("estadias.txt", "rb");
    if (file != NULL) {
        fread(estadia, sizeof(struct Estadia), 1, file);
        fclose(file);
    }
    printf("\nCodigo:%s\nCliente:%s\nQuarto:%s\nDias:%d\nEntrada:%s\nSaida:%s\n",estadia->codigo, estadia->codCliente,estadia->codQuarto,estadia->quantidadeDiarias, estadia->dataEntrada, estadia->dataSaida);
}

int verificaMes(int mes){
    if(mes == 2){
        return 28;
    }
    else if(mes == 4 || mes == 6 || mes == 9 || mes == 11){
        return 30;
    }
    else{
        return 31;
    }
}

//functions do Cadastro de Quartos
typedef struct Quarto{
    char codigo[100];
    char quantidadeHospedes[100];
    int valorDiaria;
    char status[100];
} Quarto;

void cadastrarQuarto(){
  int scodigo;
  do{
      Quarto quarto;
      FILE *arqQuarto;
      int scodigo;
      arqQuarto = fopen("quartos.txt", "ab");
      if(arqQuarto == NULL){
        puts("\nNao foi possivel abrir o arquivo! :(");
      }else{
      getchar();
      puts("\nInsira o codigo do quarto ( Padrao: 2 ou 3 algarismos )");
      fgets(quarto.codigo, sizeof(quarto.codigo),stdin);
      scodigo = checarCodigoQuarto(quarto.codigo);
      if(scodigo == 1){
        puts("\nCodigo NAO disponivel!");
      }else{
        //strcat(quarto.codigo, "#");
        puts("\nDigite a quantidade de hospedes ( no maximo 4 ):");
        fgets(quarto.quantidadeHospedes, sizeof(quarto.quantidadeHospedes),stdin);
        //strcat(quarto.quantidadeHospedes, "#");
        puts("\nDigite o valor da diaria:");
        scanf("%i", &quarto.valorDiaria);
        getchar();
        strcpy(quarto.status, "desocupado");
        fseek(arqQuarto,SEEK_END,1);
        fwrite(&quarto, sizeof(Quarto),1,arqQuarto);
        fclose(arqQuarto);
        puts("\nCadastrar mais quartos?\n[S] [N]");
      }
    }
  }while(getchar() == 's');
}

void pesquisarDadosQuartos(char numHospedes[], char codQuarto[]){
  Quarto quarto;
  FILE *arqQuarto;
  char statusQuarto[100];
  strcpy(statusQuarto, "desocupado");
  arqQuarto = fopen("quartos.txt", "rb");
    if(arqQuarto == NULL){
      puts("Nao foi possivel abrir o arquivo! :(");
    }else{
        fread(&quarto, sizeof(Quarto),1,arqQuarto);
        do {
            if(strcmp(numHospedes, quarto.quantidadeHospedes) == 0 && strcmp(statusQuarto, quarto.status) == 0){
                strcpy(codQuarto,quarto.codigo);
                break;
            }
            fread(&quarto, sizeof(Quarto),1,arqQuarto);
        } while (!feof(arqQuarto));
        fclose(arqQuarto);
    }
}

int checarCodigoQuarto(char codigo[100]){
    Quarto quarto;
    FILE *arqQuarto;
    arqQuarto = fopen("quartos.txt", "rb");
    fread(&quarto, sizeof(Quarto),1,arqQuarto);
    do {
        if(strcmp(codigo, quarto.codigo) == 0){
          fclose(arqQuarto);
          return 1;
          break;
        }
        fread(&quarto, sizeof(Quarto),1,arqQuarto);
    } while (!feof(arqQuarto));
    return 0;
    fclose(arqQuarto);
}

//function do Cadastro de Funcionarios
typedef struct Funcionario{
    char codigo[100];
    char nome[100];
    int telefone;
    char cargo[100];
    float salario;
} Funcionario;
void cadastrarFuncionario(){
  do{
      Funcionario funcionario;
      FILE *arqFuncionario;
      arqFuncionario = fopen("cadFuncionario.txt", "ab");
      if(arqFuncionario == NULL){
        puts("Nao foi possivel abrir o arquivo! :(");
      }else{
       getchar();
      system("cls");
      puts("Insira o codigo do Funcionario:");
      fgets(funcionario.codigo, sizeof(funcionario.codigo),stdin);
      if(checarCodigo(2, funcionario.codigo) == 1){
        puts("Codigo NAO disponivel!");
      }else{
        puts("Digite o nome:");
        fgets(funcionario.nome, sizeof(funcionario.nome),stdin);
        puts("Telefone de contato:");
        scanf("%i", &funcionario.telefone);
        getchar();
        puts("Informe o cargo:");
        fgets(funcionario.cargo, sizeof(funcionario.cargo),stdin);
        puts("Insira o salario:");
        scanf("%f", &funcionario.salario);
        getchar();
        fseek(arqFuncionario,SEEK_END,1);
        fwrite(&funcionario, sizeof(Funcionario),1,arqFuncionario);
        fclose(arqFuncionario);
        puts("Cadastrar mais funcionarios?\n[S] [N]");
      }
    }
  }while(getchar() == 's');
}
//functions do Cadastro de Cliente
typedef struct Endereco{
    char rua[50],cidadeEstado[50];
    int numero;
} Endereco;
typedef struct Cliente{
    char nome[100];
    char codigo[100];
    int telefone;
    Endereco endereco;
} Cliente;
void cadastrarCliente(){
  do{
      Cliente cliente;
      FILE *arqCliente;
      arqCliente = fopen("cadCliente.txt", "ab");
      if(arqCliente == NULL){
        puts("Nao foi possivel abrir o arquivo! :(");
      }else{
       getchar();
      system("cls");
      puts("Insira o codigo do cliente:");
      fgets(cliente.codigo, sizeof(cliente.codigo),stdin);
      if(checarCodigo(1, cliente.codigo) == 1){
        puts("Codigo NAO disponivel!");
      }else{
        puts("Digite o nome:");
        fgets(cliente.nome, sizeof(cliente.nome),stdin);
        puts("Telefone de contato:");
        scanf("%i", &cliente.telefone);
        getchar();
        puts("\n*-!-Endereco-!-*");
        puts("Digite a rua:");
        fgets(cliente.endereco.rua,sizeof(cliente.endereco.rua), stdin);
        puts("Numero:");
        scanf("%i", &cliente.endereco.numero);
        getchar();
        puts("Insira a cidade e o estado\n\nex:Belo Horizonte MG:");
        fgets(cliente.endereco.cidadeEstado,sizeof(cliente.endereco.cidadeEstado), stdin);
        fseek(arqCliente,SEEK_END,1);
        fwrite(&cliente, sizeof(Cliente),1,arqCliente);
        fclose(arqCliente);
        puts("Cadastrar mais clientes?\n[S] [N]");
      }
    }
  }while(getchar() == 's');
}
// Functions de pesquisa
void listarCadastros(){
  system("cls");
  getchar();
  int opcao = 1;
  puts("[1] - Clientes\n[2] - Funcionarios");
  scanf("%i", &opcao);
  getchar();
  if(opcao == 1){
      Cliente cliente;
      FILE *arqCliente;
      arqCliente = fopen("cadCliente.txt", "rb");
        if(arqCliente == NULL){
          system("cls");
          puts("Nao foi possivel abrir o arquivo! :(");
          puts("Pressione qualquer tecla para continuar!");
          getchar();
        }else{
          system("cls");
          while(fread(&cliente, sizeof(Cliente), 1,arqCliente) == 1){
            printf("Codigo: %s", cliente.codigo);
            printf("Nome: %s\n", cliente.nome);
            printf("Contato: %i\n", cliente.telefone);
            printf("Rua: %s", cliente.endereco.rua);
            printf("Numero: %i\n", cliente.endereco.numero);
            printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
            puts("--------------------------------------");
          }
        }
          puts("Pressione qualquer tecla para continuar!");
          getchar();
  }else if(opcao == 2){
      Funcionario funcionario;
      FILE *arqFuncionario;
      arqFuncionario = fopen("cadFuncionario.txt", "rb");
        if(arqFuncionario == NULL){
          system("cls");
          puts("Nao foi possivel abrir o arquivo! :(");
          puts("Pressione qualquer tecla para continuar!");
          getchar();
        }else{
          system("cls");
          while(fread(&funcionario, sizeof(Funcionario), 1,arqFuncionario) == 1){
            printf("Codigo: %s", funcionario.codigo);
            printf("Nome: %s\n", funcionario.nome);
            printf("Contato: %i\n", funcionario.telefone);
            printf("Cargo: %s", funcionario.cargo);
            printf("Salario: R$ %.2f\n", funcionario.salario);
            puts("--------------------------------------");
          }
        }
          puts("Pressione qualquer tecla para continuar!");
          getchar();
          fclose(arqFuncionario);
  }
}
void pesquisarCadastros(){
  int tarefa;
  char pesquisa[100];
  int escolha;
  puts("[1] - Clientes\n[2] - Funcionarios");
  scanf("%i", &tarefa);
  getchar();
  if(tarefa == 1){
      system("cls");
      Cliente cliente;
      FILE *arqCliente;
      arqCliente = fopen("cadCliente.txt", "rb");
        if(arqCliente == NULL){
          puts("Nao foi possivel abrir o arquivo! :(");
        }else{
          puts("O que deseja pesquisar?\n[1] - Codigo\n[2] - Nome");
          scanf("%i", &escolha);
          getchar();
          if(escolha == 1){
            puts("Digite o codigo que deseja pesquisar:");
            fgets(pesquisa, sizeof(pesquisa), stdin);
            while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1){
                if(strcmp(pesquisa, cliente.codigo) == 0){
                  system("cls");
                  printf("Codigo: %s", cliente.codigo);
                  printf("Nome: %s\n", cliente.nome);
                  printf("Contato: %i\n", cliente.telefone);
                  printf("Rua: %s", cliente.endereco.rua);
                  printf("Numero: %i\n", cliente.endereco.numero);
                  printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
                  puts("--------------------------------------");
                }
            }fclose(arqCliente);
          }else if(escolha == 2){
            puts("Digite o nome que deseja pesquisar:");
            fgets(pesquisa, sizeof(pesquisa), stdin);
            while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1){
                if(strcmp(pesquisa, cliente.nome) == 0){
                  system("cls");
                  printf("Codigo: %s", cliente.codigo);
                  printf("Nome: %s\n", cliente.nome);
                  printf("Contato: %i\n", cliente.telefone);
                  printf("Rua: %s", cliente.endereco.rua);
                  printf("Numero: %i\n", cliente.endereco.numero);
                  printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
                  puts("--------------------------------------");
                }
            }fclose(arqCliente);
          }
        }
  }else if(tarefa == 2){
      system("cls");
      Funcionario funcionario;
      FILE *arqFuncionario;
      arqFuncionario = fopen("cadFuncionario.txt", "rb");
        if(arqFuncionario == NULL){
          puts("Nao foi possivel abrir o arquivo! :(");
        }else{
          puts("O que deseja pesquisar?\n[1] - Codigo\n[2] - Nome");
          scanf("%i", &escolha);
          getchar();
          if(escolha == 1){
            puts("Digite o codigo que deseja pesquisar:");
            fgets(pesquisa, sizeof(pesquisa), stdin);
            while(fread(&funcionario, sizeof(Funcionario),1,arqFuncionario) == 1){
                if(strcmp(pesquisa, funcionario.codigo) == 0){
                  printf("Codigo: %s", funcionario.codigo);
                  printf("Nome: %s\n", funcionario.nome);
                  printf("Contato: %i\n", funcionario.telefone);
                  printf("Cargo: %s", funcionario.cargo);
                  printf("Salario: R$ %.2f\n", funcionario.salario);
                  puts("--------------------------------------");
                }
            }fclose(arqFuncionario);
          }else if(escolha == 2){
            puts("Digite o nome que deseja pesquisar:");
            fgets(pesquisa, sizeof(pesquisa), stdin);
            while(fread(&funcionario, sizeof(Funcionario),1,arqFuncionario) == 1){
                if(strcmp(pesquisa, funcionario.nome) == 0){
                  printf("Codigo: %s", funcionario.codigo);
                  printf("Nome: %s\n", funcionario.nome);
                  printf("Contato: %i\n", funcionario.telefone);
                  printf("Cargo: %s", funcionario.cargo);
                  printf("Salario: R$ %.2f\n", funcionario.salario);
                  puts("--------------------------------------");
                }
            }fclose(arqFuncionario);
          }
      }
    }
    puts("Pressione qualquer tecla para continuar!");
    getchar();
}



int checarCodigo(int tarefa, char codigo[100]){
  if(tarefa == 1){
    Cliente cliente;
    FILE *arqCliente;
    arqCliente = fopen("cadCliente.txt", "rb");
    while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1){
        system("cls");
        if(strcmp(codigo, cliente.codigo) == 0){
          return 1;
        }
          return 0;
    }
  }else if(tarefa == 2){
    Funcionario funcionario;
    FILE *arqFuncionario;
    arqFuncionario = fopen("cadCliente.txt", "rb");
    while(fread(&funcionario, sizeof(Funcionario),1,arqFuncionario) ==1){
        if(strcmp(codigo, funcionario.codigo) == 0){
          fclose(arqFuncionario);
          return 1;
        }
          return 0;
    }
  }
}
void pesquisarClienteEstadia(char nomeCliente[], char codCliente[]){
  Cliente cliente;
  FILE *arqCliente;
  arqCliente = fopen("cadCliente.txt", "rb");
    if(arqCliente == NULL){
      puts("Nao foi possivel abrir o arquivo! :(");
    }else{
        strcat(nomeCliente, "#");
        while(fread(&cliente, sizeof(Cliente),1,arqCliente) == 1){
            if(strcmp(nomeCliente, cliente.nome) == 0){
              strcpy(codCliente,cliente.codigo);
            }
         }
      fclose(arqCliente);
    }
}

void pesquisarEstadia()
{
  system("cls");
  Estadia estadia;
  FILE *arqEstadia;
  char nomeCliente[100], codCliente[100];
  getchar();
  arqEstadia = fopen("estadias.txt", "rb");
  if (arqEstadia == NULL)
  {
    puts("Nao foi possivel abrir o arquivo! :(");
  }
  else
  {
    printf("\n Informe o nome do Cliente conforme cadastro de clientes:\n");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    pesquisarClienteEstadia(nomeCliente, codCliente);
    while (fread(&estadia, sizeof(Estadia), 1, arqEstadia) == 1)
    {
      if (strcmp(codCliente, estadia.codCliente) == 0)
      {
        system("cls");
        printf("Codigo Cliente: %s", estadia.codCliente);
        printf("Codigo Estadia: %s", estadia.codigo);
        printf("Codigo Quarto: %s\n", estadia.codQuarto);
        printf("Data de entrada: %i\n", estadia.dataEntrada);
        printf("Data de saída: %i\n", estadia.dataSaida);
        printf("Quantidade de diarias: %i\n", estadia.quantidadeDiarias);
        puts("--------------------------------------");
      }
    }
  }
  fclose(arqEstadia);
  puts("Pressione qualquer tecla para continuar!");
  getch();
}

void pontosFidelidade()
{
  system("cls");
  Estadia estadia;
  FILE *arqEstadia;
  char nomeCliente[100], codCliente[100];
  getchar();
  arqEstadia = fopen("estadias.txt", "rb");
  if (arqEstadia == NULL)
  {
    puts("Nao foi possivel abrir o arquivo! :(");
  }
  else
  {
    printf("\n Informe o nome do Cliente conforme cadastro de clientes:\n");
    fgets(nomeCliente, sizeof(nomeCliente), stdin);
    pesquisarClienteEstadia(nomeCliente, codCliente);
    while (fread(&estadia, sizeof(Estadia), 1, arqEstadia) == 1)
    {
      if (strcmp(codCliente, estadia.codCliente) == 0)
      {
        system("cls");
        printf("Codigo Cliente: %s", estadia.codCliente);
        printf("Codigo Estadia: %s", estadia.codigo);
        printf("Quantidade de diarias: %i\n", estadia.quantidadeDiarias);
        printf("Pontos de fidelidade: %i\n", estadia.quantidadeDiarias * 10);
        puts("--------------------------------------");
      }
    }
  }
  fclose(arqEstadia);
  puts("Pressione qualquer tecla para continuar!");
  getch();
}

//main
int main(void)
{
    system("cls");
    int select = -1;
    setlocale(LC_ALL,"");
    while(select != 0)
    {
        puts("\n\nSelecione as opcoes a seguir: \n1 - Cadastrar cliente; \n2 - Cadastrar funcionario; \n3 - Cadastrar quarto; \n4 - Cadastrar estadia; \n5 - Fazer pesquisa; \n6 - Listar Cadastros\n7 - Exibir estadias; \n8 - Fazer pesquisa de estadia \n9 - Calcular pontos fidelidade; \n10 - Fazer checkout; \n0 - Sair;\n");
        scanf("%i", &select);
        selectFunctions(select);
    }
    return 0;
}



