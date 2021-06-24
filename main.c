#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

void cadastrarEstadia();
void cadastrarQuarto();
void cadastrarFuncionario();
void cadastrarCliente();
void pesquisarCliente();
void listarClientes();
int checarCodigo(char codigo[100]);
int checarCodigoQuarto(char codigo[100]);
void cabecalhoClientes(int opcao);
void pesquisarClienteEstadia(char nomeCliente[], char codCliente[]);
void pesquisarDadosQuartos(char numHospedes[], char codQuarto[]);
int verificaMes(int mes);
void lerEstadia();

//function do Menu principal
enum opcoes {SAIR, CLIENTE, FUNCIONARIO, QUARTO, ESTADIA, PESQUISAR_CLIENTE, LISTAR_CLIENTES, PESQUISAR_FUNCIONARIO, EXIBIR_ESTADIAS, PONTOS_FIDELIDADE, CHECKOUT};
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
    case PESQUISAR_CLIENTE:
        puts("\nFazer pesquisa de cliente");
        pesquisarCliente();
        break;
        case LISTAR_CLIENTES:
        puts("\nListar clientes");
        listarClientes();
        break;
    case PESQUISAR_FUNCIONARIO:
        puts("\nFazer pesquisa de funcionario");
        break;
    case EXIBIR_ESTADIAS:
        puts("\nExibir as estadias");
        break;
    case PONTOS_FIDELIDADE:
        puts("\nCalcular pontos fidelidade");
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
    } else if (mesSai == mesEnt){ //nao ta funfando esse
        calc2 = (mesSai-diaSai);
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
    // oi

    //todo: mudar status do quarto para ocupado
    //concertar calculo de diarias

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
void cadastrarFuncionario(){


    char nome[50];
    char cargo[20];
    char telefone[20];
    int codigo = rand();
    float salario;
    int resp;

    do
    {
        do
        {
            printf("\nDigite seu nome: ");
            setbuf(stdin, NULL);
            scanf("%50s", nome);
        }
        while( strlen(nome) < 6);

        do
        {
            printf("Digite seu cargo: ");
            setbuf(stdin, NULL);
            scanf("%20s", cargo);
        }
        while( strlen(cargo) < 6);

        do
        {
            printf("Digite seu salario: ");
            setbuf(stdin, NULL);
            scanf("%f", &salario);
        }
        while( salario <= 0);

        do
        {
            printf("Digite seu telefone: ");
            setbuf(stdin, NULL);
            scanf("%20s", telefone);
        }
        while( strlen(telefone) < 6);

        printf("\nCodigo: %d", codigo);
        printf("\nNome: %s", nome);
        printf("\nCargo: %s", cargo);
        printf("\nSalario: %f", salario);
        printf("\nTelefone: %s", telefone);

        printf("\nPara cadastrar novamente digite 1 para sair digite 2: ");
        scanf("%d",&resp);

    }
    while (resp==1);

    return 0;
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
  int scodigo;
  do{
      Cliente cliente;
      FILE *arqCliente;
      int scodigo;
      arqCliente = fopen("cadCliente.txt", "ab");
      if(arqCliente == NULL){
        puts("Nao foi possivel abrir o arquivo! :(");
      }else{
       getchar();
      system("cls");
      cabecalhoClientes(1);
      puts("Insira o codigo do cliente:");
      fgets(cliente.codigo, sizeof(cliente.codigo),stdin);
      scodigo = checarCodigo(cliente.codigo);
      if(scodigo == 1){
        puts("Codigo NAO disponivel!");
      }else{
        //strcat(cliente.codigo, "#");
        puts("Digite o nome:");
        fgets(cliente.nome, sizeof(cliente.nome),stdin);
        strcat(cliente.nome, "#");
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

void listarClientes(){
  system("cls");
  cabecalhoClientes(2);
  Cliente cliente;
  FILE *arqCliente;
  arqCliente = fopen("cadCliente.txt", "rb");
    if(arqCliente == NULL){
      puts("Nao foi possivel abrir o arquivo! :(");
      puts("Pressione qualquer tecla para continuar!");
      getchar();
    }else{
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
      getch();
    fclose(arqCliente);
}

void pesquisarCliente(){
  system("cls");
  cabecalhoClientes(3);
  Cliente cliente;
  FILE *arqCliente;
  char pesquisa[100];
  int escolha;
  getchar();
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
        strcat(pesquisa, "#");
        while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1){
            if(strcmp(pesquisa, cliente.codigo) == 0){
              system("cls");
              cabecalhoClientes(3);
              printf("Codigo: %s", cliente.codigo);
              printf("Nome: %s\n", cliente.nome);
              printf("Contato: %i\n", cliente.telefone);
              printf("Rua: %s", cliente.endereco.rua);
              printf("Numero: %i\n", cliente.endereco.numero);
              printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
              puts("--------------------------------------");
            }
        }
      }else if(escolha != 1){
        puts("Digite o nome que deseja pesquisar:");
        fgets(pesquisa, sizeof(pesquisa), stdin);
        strcat(pesquisa, "#");
        while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1){
            if(strcmp(pesquisa, cliente.nome) == 0){
              system("cls");
              cabecalhoClientes(3);
              printf("Codigo: %s", cliente.codigo);
              printf("Nome: %s\n", cliente.nome);
              printf("Contato: %i\n", cliente.telefone);
              printf("Rua: %s", cliente.endereco.rua);
              printf("Numero: %i\n", cliente.endereco.numero);
              printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
              puts("--------------------------------------");
            }
         }
      }
      fclose(arqCliente);
      puts("Pressione qualquer tecla para continuar!");
      getch();
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

int checarCodigo(char codigo[100]){
    Cliente cliente;
    FILE *arqCliente;
    arqCliente = fopen("cadCliente.txt", "rb");
    while(fread(&cliente, sizeof(Cliente),1,arqCliente) == 1){
        system("cls");
        if(strcmp(codigo, cliente.codigo) == 0){
          fclose(arqCliente);
          return 1;
        }else{
          fclose(arqCliente);
          return 0;
    }
  }
}

void cabecalhoClientes(int opcao){
  switch(opcao){
    case 1:{
      puts("----------------------------------------");
      puts("-                                      -");
      puts("-     AREA DE CADASTRO DE CLIENTES     -");
      puts("-                                      -");
      puts("----------------------------------------\n");
      break;
    }
    case 2:{
      puts("----------------------------------------");
      puts("-                                      -");
      puts("-           LISTA DE CLIENTES          -");
      puts("-                                      -");
      puts("----------------------------------------\n");
      break;
    }
        case 3:{
      puts("----------------------------------------");
      puts("-                                      -");
      puts("-          PESQUISAR CLIENTES          -");
      puts("-                                      -");
      puts("----------------------------------------\n");
      break;
    }
  }
}

//main
int main(void)
{
    system("cls");
    int select = -1;
    setlocale(LC_ALL,"");
    while(select != 0)
    {
        puts("\n\nSelecione as opcoes a seguir: \n1 - Cadastrar cliente; \n2 - Cadastrar funcionario; \n3 - Cadastrar quarto; \n4 - Cadastrar estadia; \n5 - Fazer pesquisa de cliente; \n6 - Listar Clientes\n7 - Fazer pesquisa de funcionario; \n8 - Exibir estadias; \n9 - Calcular pontos fidelidade; \n10 - Fazer checkout; \n0 - Sair;\n");
        scanf("%i", &select);
        selectFunctions(select);
    }
    return 0;
}



