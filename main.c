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
int pesquisarClienteEstadia(char nomeCliente[]);
int pesquisarDadosQuartos(int numHospedes);
int verificaMes(int mes);
void lerEstadia();
void pesquisarEstadia();
void pontosFidelidade();
void pesquisarCadastros();
void listarCadastros();
void cabecalho(int tarefa);
int gerarCodigo(int tarefa);
int calcularQntdeDiarias(char entrada[6], char saida[6]);
void alterarStatusQuarto(int codigoQuarto);

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
        puts("\nExibir as estadias NADA AQUI");
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
        puts("\nFazer checkout NADA AQUI");
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

//- - - - - - - BP - - - - - - -
//functions do Cadastro de Estadia
typedef struct Estadia
{
    int codigo;
    int codCliente;
    int codQuarto;
    int quantidadeDiarias;
    char dataEntrada[100];
    char dataSaida[100];
} Estadia;

void cadastrarEstadia()
{
    cabecalho(1);
    int execucao = 1;

    do
    {
        int i, calc, calc2, totalMes, diaEnt, mesEnt, diaSai, mesSai, qntDiaria, codCliente, codEstadia, codQuarto, numHospedes;
        char nomeCliente[100], dataEntrada[6], dataSaida[6], str[4];
        FILE *estadias;
        struct Estadia *estadia = malloc(sizeof(struct Estadia));

        getchar();
        codEstadia = gerarCodigo(4);
        estadia->codigo = codEstadia;
        printf("\nO codigo gerado para a estadia foi: %d", estadia->codigo);

        printf("\n Informe o nome do Cliente conforme cadastro de clientes:\n");
        fgets(nomeCliente, sizeof(nomeCliente), stdin);
        codCliente = pesquisarClienteEstadia(nomeCliente);
        if(codCliente == 0)
        {
            printf("\nImpossivel localizar cliente");
            execucao = 0;
        }
        else
        {
            estadia->codCliente = codCliente;
            printf("\nO do cliente e: %d", estadia->codCliente);
            printf("\n Informe o numero total de hospedes:\n");
            scanf("%i", &numHospedes);
            getchar();
            codQuarto = pesquisarDadosQuartos(numHospedes);

            if(codQuarto == 0)
            {
                printf("\nImpossivel localizar quarto disponivel ou com capacidade adequada!");
                execucao = 0;
            }
            else
            {
                estadia->codQuarto = codQuarto;
                printf("\nO do quarto encontrado foi o : %d", estadia->codQuarto);

                printf("\nSo aceitando reservas para o ano corrente.\n");

                printf("\n Informe o dia e o mes da data de chegada (nao coloque 0 na frente do algarismo, utilize um espaco entre cada dado):\n");
                scanf("%d %d", &diaEnt, &mesEnt);
                getchar();
                printf("\n Informe o dia e o mes da data de saida (nao coloque 0 na frente do algarismo, utilize um espaco entre cada dado):\n");
                scanf("%d %d", &diaSai, &mesSai);
                getchar();

                qntDiaria = 0;
                totalMes = verificaMes(mesEnt);
                calc = totalMes - diaEnt;

                if(mesSai-mesEnt > 1)
                {
                    for(i = mesSai-1; i > mesEnt; i--)
                    {
                        totalMes = verificaMes(i);
                        calc += totalMes;
                    }
                    qntDiaria = diaSai+calc;
                }
                else if (mesSai == mesEnt)
                {
                    calc2 = totalMes - diaSai;
                    qntDiaria = calc-calc2;
                }
                else
                {
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
                if (estadias != NULL)
                {
                    fwrite(estadia, sizeof(struct Estadia), 1, estadias);
                    fclose(estadias);
                    puts("\nEstadia criada com sucesso!");
                    lerEstadia();
                    //alterarStatusQuarto(estadia->codQuarto);
                    puts("\nCadastrar mais quartos?\n[S][N]");
                }
                else
                {
                    puts("\nErro ao gravar estadia");
                }

                //todo: mudar status do quarto para ocupado

            }

        }

    }

    while(getchar() == 's' && execucao == 1);

};

void lerEstadia()
{
    struct Estadia *estadia = malloc(sizeof(struct Estadia));
    FILE * file= fopen("estadias.txt", "rb");
    if (file != NULL)
    {
        fread(estadia, sizeof(struct Estadia), 1, file);
        fclose(file);
    }
    printf("\nCodigo:%d\nCliente:%d\nQuarto:%d\nDias:%d\nEntrada:%s\nSaida:%s\n",estadia->codigo, estadia->codCliente,estadia->codQuarto,estadia->quantidadeDiarias, estadia->dataEntrada, estadia->dataSaida);
}

int verificaMes(int mes)
{
    if(mes == 2)
    {
        return 28;
    }
    else if(mes == 4 || mes == 6 || mes == 9 || mes == 11)
    {
        return 30;
    }
    else
    {
        return 31;
    }
}

void EncerrarEstadia()
{
//printf("\n Informe o nome do Cliente conforme cadastro de clientes:\n");
//fgets(nomeCliente, sizeof(nomeCliente), stdin);
}

//functions do Cadastro de Quartos
typedef struct Quarto
{
    int codigo;
    int quantidadeHospedes;
    int valorDiaria;
    char status[100];
} Quarto;

void cadastrarQuarto()
{
    do
    {
        Quarto quarto;
        FILE *arqQuarto;
        arqQuarto = fopen("quartos.txt", "ab");
        if(arqQuarto == NULL)
        {
            puts("\nNao foi possivel abrir o arquivo! :(");
        }
        else
        {
            getchar();
            quarto.codigo = gerarCodigo(3);
            printf("\nO codigo gerado para o funcionario foi: %d", quarto.codigo);
            puts("\nDigite a quantidade de hospedes:");
            scanf("%i", &quarto.quantidadeHospedes);
            puts("\nDigite o valor da diaria (Valores disponiveis sao: 100, 200 ou 300)");
            scanf("%i", &quarto.valorDiaria);
            getchar();
            strcpy(quarto.status, "desocupado");
            fwrite(&quarto, sizeof(Quarto),1,arqQuarto);
            fclose(arqQuarto);
            puts("\nQuarto Cadastrado com sucesso! Voce ja pode utiliza-lo em uma hospedagem!");
            puts("\nCadastrar mais quartos?\n[S][N]");

        }
    }
    while(getchar() == 's');
}

void alterarStatusQuarto(int codigoQuarto){
    Quarto quarto;
    FILE *arqQuarto;
    int posicao = codigoQuarto-1;
    arqQuarto = fopen("quartos.txt", "rb");
    fseek(arqQuarto, posicao*sizeof(Quarto), SEEK_SET);
    fread(&quarto, sizeof(Quarto), 1, arqQuarto);
    printf("\n\nO quarto e: %d\n Tem a capacidade de: %d", quarto.codigo, quarto.quantidadeHospedes);
}

int pesquisarDadosQuartos(int numHospedes)
{
    Quarto quarto;
    FILE *arqQuarto;
    char statusQuarto[100];
    int codigo = 0;
    strcpy(statusQuarto, "desocupado");
    arqQuarto = fopen("quartos.txt", "rb");
    if(arqQuarto == NULL)
    {
        puts("Nao foi possivel abrir o arquivo! :(");
    }
    else
    {
        fread(&quarto, sizeof(Quarto),1,arqQuarto);
        do
        {
            if(numHospedes == quarto.quantidadeHospedes && strcmp(statusQuarto, quarto.status) == 0)
            {
                codigo = quarto.codigo;
                break;
            }
            fread(&quarto, sizeof(Quarto),1,arqQuarto);
        }
        while (!feof(arqQuarto));
        fclose(arqQuarto);
        return codigo;
    }
}

//functions do Cadastro de Funcionarios
typedef struct Funcionario
{
    int codigo;
    char nome[100];
    int telefone;
    char cargo[100];
    float salario;
} Funcionario;

void cadastrarFuncionario()
{
    do
    {
        Funcionario funcionario;
        FILE *arqFuncionario;
        arqFuncionario = fopen("cadFuncionario.txt", "ab");
        if(arqFuncionario == NULL)
        {
            puts("Nao foi possivel abrir o arquivo! :(");
        }
        else
        {
            cabecalho(1);
            getchar();
            funcionario.codigo = gerarCodigo(2);
            printf("\nO codigo gerado para o funcionario foi: %d", funcionario.codigo);
            puts("\nDigite o nome:");
            fgets(funcionario.nome, sizeof(funcionario.nome),stdin);
            puts("\nTelefone de contato:");
            scanf("%i", &funcionario.telefone);
            getchar();
            puts("\nInforme o cargo:");
            fgets(funcionario.cargo, sizeof(funcionario.cargo),stdin);
            puts("\nInsira o salario:");
            scanf("%f", &funcionario.salario);
            getchar();
            fseek(arqFuncionario,SEEK_END,1);
            fwrite(&funcionario, sizeof(Funcionario),1,arqFuncionario);
            fclose(arqFuncionario);
            puts("\nCadastrar mais funcionarios?\n[S] [N]");

        }
    }
    while(getchar() == 's');
}

//functions do Cadastro de Cliente
typedef struct Endereco
{
    char rua[50],cidadeEstado[50];
    int numero;
} Endereco;

typedef struct Cliente
{
    char nome[100];
    int codigo;
    int telefone;
    Endereco endereco;
} Cliente;

void cadastrarCliente()
{
    do
    {
        Cliente cliente;
        FILE *arqCliente;
        arqCliente = fopen("cadCliente.txt", "ab");
        if(arqCliente == NULL)
        {
            puts("Nao foi possivel abrir o arquivo! :(");
        }
        else
        {
            getchar();
            cabecalho(1);
            cliente.codigo = gerarCodigo(1);
            printf("\nO codigo gerado para o cliente foi: %d", cliente.codigo);
            puts("\nDigite o nome:");
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
    while(getchar() == 's');
}

// Lista cadastro de funcionario ou cliente
void listarCadastros()
{
    system("cls");
    cabecalho(2);
    getchar();
    int opcao = 1;
    puts("[1] - Clientes\n[2] - Funcionarios");
    scanf("%i", &opcao);
    getchar();
    if(opcao == 1)
    {
        Cliente cliente;
        FILE *arqCliente;
        arqCliente = fopen("cadCliente.txt", "rb");
        if(arqCliente == NULL)
        {
            //system("cls");
            puts("Nao foi possivel abrir o arquivo! :(");
            puts("Pressione qualquer tecla para continuar!");
            getchar();
        }
        else
        {
            //system("cls");
            while(fread(&cliente, sizeof(Cliente), 1,arqCliente) == 1)
            {
                printf("Codigo: %i", cliente.codigo);
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
    }
    else if(opcao == 2)
    {
        Funcionario funcionario;
        FILE *arqFuncionario;
        arqFuncionario = fopen("cadFuncionario.txt", "rb");
        if(arqFuncionario == NULL)
        {
            //system("cls");
            puts("Nao foi possivel abrir o arquivo! :(");
            puts("Pressione qualquer tecla para continuar!");
            getchar();
        }
        else
        {
            //system("cls");
            while(fread(&funcionario, sizeof(Funcionario), 1,arqFuncionario) == 1)
            {
                printf("Codigo: %i", funcionario.codigo);
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

//pesquisa de cliente e funcionario
void pesquisarCadastros()
{
    int tarefa;
    char pesquisa[100];
    int escolha;
    int codigo;
    cabecalho(2);
    puts("[1] - Clientes\n[2] - Funcionarios");
    scanf("%i", &tarefa);
    getchar();
    if(tarefa == 1)
    {
        system("cls");
        cabecalho(2);
        Cliente cliente;
        FILE *arqCliente;
        arqCliente = fopen("cadCliente.txt", "rb");
        if(arqCliente == NULL)
        {
            puts("Nao foi possivel abrir o arquivo! :(");
        }
        else
        {
            puts("O que deseja pesquisar?\n[1] - Codigo\n[2] - Nome");
            scanf("%i", &escolha);
            getchar();
            if(escolha == 1)
            {
                puts("Digite o codigo que deseja pesquisar:");
                scanf("%i", &codigo);
                while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1)
                {
                    if(codigo == cliente.codigo)
                    {
                        system("cls");
                        cabecalho(2);
                        printf("Codigo: #%i  ", cliente.codigo);
                        printf("Nome: %s\n", cliente.nome);
                        printf("Contato: %i\n", cliente.telefone);
                        printf("Rua: %s", cliente.endereco.rua);
                        printf("Numero: %i\n", cliente.endereco.numero);
                        printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
                        puts("--------------------------------------");
                    }
                }
                fclose(arqCliente);
            }
            else if(escolha == 2)
            {
                puts("Digite o nome que deseja pesquisar:");
                fgets(pesquisa, sizeof(pesquisa), stdin);
                while(fread(&cliente, sizeof(Cliente),1,arqCliente) ==1)
                {
                    if(strcmp(pesquisa, cliente.nome) == 0)
                    {
                        system("cls");
                        cabecalho(2);
                        printf("Codigo: #%i  ", cliente.codigo);
                        printf("Nome: %s\n", cliente.nome);
                        printf("Contato: %i\n", cliente.telefone);
                        printf("Rua: %s", cliente.endereco.rua);
                        printf("Numero: %i\n", cliente.endereco.numero);
                        printf("Cidade / Estado: %s", cliente.endereco.cidadeEstado);
                        puts("--------------------------------------");
                    }
                }
                fclose(arqCliente);
            }
        }
    }
    else if(tarefa == 2)
    {
        system("cls");
        cabecalho(2);
        Funcionario funcionario;
        FILE *arqFuncionario;
        arqFuncionario = fopen("cadFuncionario.txt", "rb");
        if(arqFuncionario == NULL)
        {
            puts("Nao foi possivel abrir o arquivo! :(");
        }
        else
        {
            puts("O que deseja pesquisar?\n[1] - Codigo\n[2] - Nome");
            scanf("%i", &escolha);
            getchar();
            if(escolha == 1)
            {
                system("cls");
                cabecalho(2);
                puts("Digite o codigo que deseja pesquisar:");
                fgets(pesquisa, sizeof(pesquisa), stdin);
                while(fread(&funcionario, sizeof(Funcionario),1,arqFuncionario) == 1)
                {
                    if(strcmp(pesquisa, funcionario.codigo) == 0)
                    {
                        printf("Codigo: #%i  ", funcionario.codigo);
                        printf("Nome: %s\n", funcionario.nome);
                        printf("Contato: %i\n", funcionario.telefone);
                        printf("Cargo: %s", funcionario.cargo);
                        printf("Salario: R$ %.2f\n", funcionario.salario);
                        puts("--------------------------------------");
                    }
                }
                fclose(arqFuncionario);
            }
            else if(escolha == 2)
            {
                system("cls");
                cabecalho(2);
                puts("Digite o nome que deseja pesquisar:");
                fgets(pesquisa, sizeof(pesquisa), stdin);
                while(fread(&funcionario, sizeof(Funcionario),1,arqFuncionario) == 1)
                {
                    if(strcmp(pesquisa, funcionario.nome) == 0)
                    {
                        printf("Codigo: #%i  ", funcionario.codigo);
                        printf("Nome: %s\n", funcionario.nome);
                        printf("Contato: %i\n", funcionario.telefone);
                        printf("Cargo: %s", funcionario.cargo);
                        printf("Salario: R$ %.2f\n", funcionario.salario);
                        puts("--------------------------------------");
                    }
                }
                fclose(arqFuncionario);
            }
        }
    }
    puts("Pressione qualquer tecla para continuar!");
    getchar();
}

//pesquisa de codigo do cliente pelo nome do cliente
int pesquisarClienteEstadia(char nomeCliente[])
{
    Cliente cliente;
    FILE *arqCliente;
    int codigo = 0;
    arqCliente = fopen("cadCliente.txt", "rb");
    if(arqCliente == NULL)
    {
        puts("Nao foi possivel abrir o arquivo do cliente! :(");
    }
    else
    {
        //while(fread(&cliente, sizeof(Cliente),1,arqCliente) == 1)
        // {
        //  if(strcmp(nomeCliente, cliente.nome) == 0)
        //  {
        //     codigo = cliente.codigo;
        // }
        // }
        do
        {
            if(strcmp(nomeCliente, cliente.nome) == 0)
            {
                codigo = cliente.codigo;
            }
            fread(&cliente, sizeof(Cliente),1,arqCliente);
        }
        while (!feof(arqCliente));
        fclose(arqCliente);
        return codigo;
    }
}

//gerador de codigo automatico
int gerarCodigo(int tarefa)
{
    int cod = 1;
    if(tarefa == 1)
    {
        Cliente cliente;
        FILE* arqCliente;
        arqCliente = fopen("cadCliente.txt","rb");
        if(arqCliente == NULL)
        {
            return cod;
        }
        else
        {
            do
            {
                cod++;
                fread(&cliente, sizeof(Cliente),1,arqCliente);
            }
            while (!feof(arqCliente));
            fclose(arqCliente);
        }
    }
    else if(tarefa == 2)
    {
        Funcionario funcionario;
        FILE *arqFuncionario;
        arqFuncionario = fopen("cadFuncionario.txt","rb");
        if(arqFuncionario == NULL)
        {
            return cod;
        }
        else
        {
            do
            {
                cod++;
                fread(&funcionario, sizeof(Funcionario),1,arqFuncionario);
            }
            while (!feof(arqFuncionario));
            fclose(arqFuncionario);
        }
    }
    else if(tarefa == 3)
    {
        Quarto quarto;
        FILE *arqQuarto;
        arqQuarto = fopen("quartos.txt","rb");
        if(arqQuarto == NULL)
        {
            return cod;
        }
        else
        {
            do
            {
                cod++;
                fread(&quarto, sizeof(Quarto),1,arqQuarto);
            }
            while (!feof(arqQuarto));
            fclose(arqQuarto);
        }
    }
    else if(tarefa == 4)
    {
        Estadia estadia;
        FILE *arqEstadia;
        arqEstadia = fopen("estadias.txt","rb");
        if(arqEstadia == NULL)
        {
            return cod;
        }
        else
        {
            do
            {
                cod++;
                fread(&estadia, sizeof(Estadia),1,arqEstadia);
            }
            while (!feof(arqEstadia));
            fclose(arqEstadia);
        }
    }
    return cod;
}

//pesquisa das estadias existentes para um cliente
void pesquisarEstadia()
{
    cabecalho(2);
    Estadia estadia;
    FILE *arqEstadia;
    char nomeCliente[100];
    int codClienteEstadia;
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
        codClienteEstadia = pesquisarClienteEstadia(nomeCliente);
        do
        {
           if (codClienteEstadia == estadia.codCliente)
           {
                printf("Codigo Cliente: %d\n", estadia.codCliente);
                printf("Codigo Estadia: %d", estadia.codigo);
                printf("Codigo Quarto: %d\n", estadia.codQuarto);
                printf("Data de entrada: %s\n", estadia.dataEntrada);
                printf("Data de saída: %s\n", estadia.dataSaida);
                printf("Quantidade de diarias: %d\n", estadia.quantidadeDiarias);
                puts("--------------------------------------");
                break;
            }

            fread(&estadia, sizeof(Estadia),1,arqEstadia);
        }
        while (!feof(arqEstadia));
        fclose(arqEstadia);
    }

    puts("Pressione qualquer tecla para continuar!");
    getchar();
}

//calculo dos pontos de fidelidade de um cliente
void pontosFidelidade()
{
    Estadia estadia;
    FILE *arqEstadia;
    char nomeCliente[100];
    int codClienteEstadia;
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
        codClienteEstadia = pesquisarClienteEstadia(nomeCliente);
        do
        {
           if (codClienteEstadia == estadia.codCliente)
            {
                printf("Codigo Cliente: %d\n", estadia.codCliente);
                printf("Codigo Estadia: %d\n", estadia.codigo);
                printf("Quantidade de diarias: %d\n", estadia.quantidadeDiarias);
                printf("Pontos fidelidade: %d\n", estadia.quantidadeDiarias * 10);
                puts("--------------------------------------");
                break;
            }

            fread(&estadia, sizeof(Estadia),1,arqEstadia);
        }
        while (!feof(arqEstadia));
        fclose(arqEstadia);
    }
    puts("Pressione qualquer tecla para continuar!");
    getchar();
}

void cabecalho(int tarefa)
{
    switch(tarefa)
    {
    case 1:
        puts("*****************************************");
        puts("*                                       *");
        puts("*           Tela de Cadastro            *");
        puts("*                                       *");
        puts("*****************************************");
        break;
    case 2:
        puts("*****************************************");
        puts("*                                       *");
        puts("*              Pesquisas                *");
        puts("*                                       *");
        puts("*****************************************");
        break;
    case 3:
        puts("*****************************************");
        puts("*                                       *");
        puts("*              Hotel Descanso           *");
        puts("*                Garantido              *");
        puts("*                                       *");
        puts("*****************************************");
    }
}

//main
int main(void)
{
    //system("cls");
    int select = -1;
    setlocale(LC_ALL,"");
    while(select != 0)
    {
        system("cls");
        cabecalho(3);
        puts("\n\nSelecione as opcoes a seguir: \n1 - Cadastrar cliente; \n2 - Cadastrar funcionario; \n3 - Cadastrar quarto; \n4 - Cadastrar estadia; \n5 - Fazer pesquisa; \n6 - Listar Cadastros\n7 - Exibir estadias; \n8 - Fazer pesquisa de estadia \n9 - Calcular pontos fidelidade; \n10 - Fazer checkout; \n0 - Sair;\n");
        scanf("%i", &select);
        selectFunctions(select);
    }
    return 0;
}



