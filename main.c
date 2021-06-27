#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int selectFunctions(int select);
void cadastrarEstadia();
void lerEstadia();
int verificaMes(int mes);
void EncerrarEstadia();
void cadastrarQuarto();
int pesquisarDadosQuartos(int numHospedes);
int pesquisarDiariaQuarto(int codigo);
void cadastrarFuncionario();
void cadastrarCliente();
int validacaoString(char string[100]);
void listarCadastros();
void pesquisarCadastros();
int pesquisarClienteEstadia(char nomeCliente[]);
int gerarCodigo(int tarefa);
void pesquisarEstadia();
void pontosFidelidade();
void cabecalho(int tarefa);

//int checarCodigoQuarto(char codigo[100]);
//int calcularQntdeDiarias(char entrada[6], char saida[6]);
//int validacaoStringCliente(char string[100]);
//int validacaoTelefoneCliente(int num);



enum opcoes {SAIR= 0,CADASTRO = 1, CLIENTE = 11, FUNCIONARIO = 12, QUARTO = 13, ESTADIA = 14, PESQUISAR = 2, PESQUISAR_CADASTROS = 21, PESQUISAR_ESTADIA = 22, LISTAR = 3 , LISTAR_CADASTROS = 31, EXIBIR_ESTADIAS = 32, PONTOS_FIDELIDADE = 4, CHECKOUT = 5};

int selectFunctions(int select)
{
    switch(select)
    {
    case CADASTRO:
        cabecalho(1);
        puts("[11] - Cliente\n[12] - Funcionario\n[13] - Quarto\n[14] - Estadia\n[0] - Voltar");
        scanf("%i",&select);
        switch(select){
            case CLIENTE: cadastrarCliente(); break;
            case FUNCIONARIO: cadastrarFuncionario(); break;
            case QUARTO: cadastrarQuarto(); break;
            case ESTADIA: cadastrarEstadia(); break;
            default: cabecalho(3);break;
        }
        break;
    case PESQUISAR:
        cabecalho(2);
        puts("[21] - Clientes/Funcionarios\n[22] - Estadia\n[0] - Voltar");
        scanf("%i",&select);
        switch(select){
            case PESQUISAR_CADASTROS: pesquisarCadastros(); break;
            case PESQUISAR_ESTADIA: pesquisarEstadia(); break;
            default: cabecalho(3);break;
        }
        break;
    case LISTAR:
        cabecalho(4);
        puts("[31] - Clientes/Funcionarios\n[32] - Estadia\n[0] - Voltar");
        scanf("%i",&select);
        switch(select){
            case LISTAR_CADASTROS: listarCadastros(); break;
            case EXIBIR_ESTADIAS:/*  exibirEstadias(); */ break;
            default: cabecalho(3);break;
        }
        break;
    case PONTOS_FIDELIDADE:
        pontosFidelidade();
        break;
    case CHECKOUT:
        EncerrarEstadia();
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
            //printf("\nO do cliente e: %d", estadia->codCliente);
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
                printf("\nO numero do quarto encontrado e: %d", estadia->codQuarto);

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

                printf("\n Dias de entrada e saida gravados com sucesso\n");

                estadias = fopen("estadias.txt", "ab");
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
    Estadia estadia;
    FILE *arquivoEstadia;
    char nomeCliente[100];
    int codClienteEstadia, codQuartoEstadia, codigoEstadia, diarias, valorDiaria;
    float totalPagar;
    getchar();
    arquivoEstadia = fopen("estadias.txt", "rb");
    if (arquivoEstadia == NULL)
    {
        puts("Nao foi possivel abrir o arquivo! :(");
    }
    else
    {
        printf("\n Informe o nome do Cliente conforme cadastro de clientes:\n");
        fgets(nomeCliente, sizeof(nomeCliente), stdin);
        codClienteEstadia = 0;
        codClienteEstadia = pesquisarClienteEstadia(nomeCliente);
        if(codClienteEstadia == 0 )
        {
            printf("\n Impossivel localizar cliente");
        }
        else
        {

            do
            {
                if (codClienteEstadia == estadia.codCliente)
                {
                    codQuartoEstadia = estadia.codQuarto;
                    codigoEstadia = estadia.codigo;
                    diarias = estadia.quantidadeDiarias;
                    break;
                }

                fread(&estadia, sizeof(Estadia),1,arquivoEstadia);
            }
            while (!feof(arquivoEstadia));

            valorDiaria = pesquisarDiariaQuarto(codQuartoEstadia);

            totalPagar = valorDiaria*diarias;
            printf("\n O valor total a pagar por %d diarias e de: R$ %.2f reais. \n\n O valor de cada diaria e de: R$ %d\n\n", diarias, totalPagar, valorDiaria);
            //to do: mudar o status do quarto
            //to do: retirar essa estadia da lista?
        }

        fclose(arquivoEstadia);
        puts("Pressione qualquer tecla para continuar!");
        getchar();


    }

}

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
            printf("\nO codigo gerado para o quarto foi: %d", quarto.codigo);
            puts("\nDigite a quantidade de hospedes (Padrao: numero inteiro):");
            scanf("%d", &quarto.quantidadeHospedes);
            getchar();
            if(quarto.quantidadeHospedes <= 0){
                puts("\n Valor invalido");
                puts("\n Pressione qualquer tecla para continuar!");
                getchar();
            } else {
                puts("\nDigite o valor da diaria (Padrao: numero inteiro)");
                scanf("%d", &quarto.valorDiaria);
                getchar();
                if(quarto.valorDiaria <= 0){
                    puts("\n Valor invalido");
                    puts("\n Pressione qualquer tecla para continuar!");
                    getchar();
                } else {
                    strcpy(quarto.status, "desocupado");
                    fwrite(&quarto, sizeof(Quarto),1,arqQuarto);
                    fclose(arqQuarto);
                    puts("\nQuarto Cadastrado com sucesso! Voce ja pode utiliza-lo em uma hospedagem!");
                    puts("\nCadastrar mais quartos?\n[S][N]");
                }
            }

        }
    }
    while(getchar() == 's');
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

int pesquisarDiariaQuarto(int codigo)
{
    Quarto quarto;
    FILE *arquivoQuarto;
    arquivoQuarto = fopen("quartos.txt", "rb");
    if(arquivoQuarto == NULL)
    {
        puts("Nao foi possivel abrir o arquivo! :(");
        return 0;
    }
    else
    {
        do
        {
            if(codigo == quarto.codigo)
            {
                return quarto.valorDiaria;
            }
            fread(&quarto, sizeof(Quarto),1,arquivoQuarto);
        }
        while (!feof(arquivoQuarto));
        fclose(arquivoQuarto);
    }
}

typedef struct Funcionario
{
    int codigo;
    char nome[100];
    int telefone;
    char cargo[100];
    int salario;
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
            cabecalho(1);
            puts("Nao foi possivel abrir o arquivo! :(");
        }
        else
        {
            int cargo = 0;
            cabecalho(1);
            getchar();
            funcionario.codigo = gerarCodigo(2);
            printf("\nO codigo gerado para o funcionario foi: %d", funcionario.codigo);
            puts("\nDigite o nome:");
            fgets(funcionario.nome, sizeof(funcionario.nome),stdin);
                puts("\nTelefone de contato:");
                scanf("%i", &funcionario.telefone);
            getchar();
            do{
                cabecalho(1);
                puts("\nInforme o cargo:");
                puts("[1] - Recepcionista\n[2] - Auxiliar de limpeza\n[3] - Garcom\n[4] - Gerente");
                scanf("%i",&cargo);
                switch(cargo){
                    case 1: strcpy(funcionario.cargo,"Recepcionista     "); break;
                    case 2: strcpy(funcionario.cargo,"Auxiliar de Limpeza   "); break;
                    case 3: strcpy(funcionario.cargo,"Garcom    "); break;
                    case 4: strcpy(funcionario.cargo,"Gerente   "); break;
                    default:puts("Cargo nao reconhecido!"); sleep(2);
                }
            }while(cargo > 4 || cargo < 1);
            getchar();
            puts("\nInsira o salario:");
            scanf("%i", &funcionario.salario);
            getchar();
            fseek(arqFuncionario,SEEK_END,1);
            fwrite(&funcionario, sizeof(Funcionario),1,arqFuncionario);
            fclose(arqFuncionario);
            puts("\nCadastrar mais funcionarios?\n[S] [N]");
        }
    }
    while(getchar() == 's');
}

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
            do{
                puts("\nDigite o nome:");
                fgets(cliente.nome, sizeof(cliente.nome),stdin);
            }while(validacaoString(cliente.nome) < 1);
            puts("Telefone de contato:");
            scanf("%i", &cliente.telefone);
            getchar();
            puts("*-!-Endereco-!-*");
            do{
                puts("Digite a rua:");
                fgets(cliente.endereco.rua,sizeof(cliente.endereco.rua), stdin);
            }while(validacaoString(cliente.endereco.rua) < 1);
            puts("Numero:");
            scanf("%i", &cliente.endereco.numero);
            getchar();
            do{
                puts("\nInsira a cidade e o estado\nex:Belo Horizonte MG:");
                fgets(cliente.endereco.cidadeEstado,sizeof(cliente.endereco.cidadeEstado), stdin);
            }while(validacaoString(cliente.endereco.cidadeEstado) < 1);
            fseek(arqCliente,SEEK_END,1);
            fwrite(&cliente, sizeof(Cliente),1,arqCliente);
            fclose(arqCliente);
            puts("Cadastrar mais clientes?\n[S] [N]");
        }
    }
    while(getchar() == 's');
}

int validacaoString(char string[100]){
    int i = 0, error = 0; char letra;
    while(string[i] != '\0'){
        letra = string[i];
        if((letra >= 'a' && letra <= 'z') || (letra >= 'A' && letra <= 'Z') || (letra == 32)){
        }else{
            error++;
        }
        i++;
    }
    if(error > 1){
        puts("Caractere Invalido!");
        return 0;
    }else
    return 1;
}

void listarCadastros()
{
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
            puts("Nao foi possivel abrir o arquivo! :(");
            puts("Pressione qualquer tecla para continuar!");
            getchar();
        }
        else
        {
            while(fread(&cliente, sizeof(Cliente), 1,arqCliente) == 1)
            {
                printf("Codigo: %i  ", cliente.codigo);
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
            puts("Nao foi possivel abrir o arquivo! :(");
            puts("Pressione qualquer tecla para continuar!");
            getchar();
        }
        else
        {
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
                cabecalho(2);
                puts("Digite o codigo que deseja pesquisar:");
                scanf("%i", &codigo);
                while(fread(&funcionario, sizeof(Funcionario),1,arqFuncionario) == 1)
                {
                    if(codigo == funcionario.codigo)
                    {
                        printf("Codigo: #%i  ", funcionario.codigo);
                        printf("Nome: %s\n", funcionario.nome);
                        printf("Contato: %i\n", funcionario.telefone);
                        printf("Cargo: %s", funcionario.cargo);
                        printf("Salario: R$ %i,00\n", funcionario.salario);
                        puts("--------------------------------------");
                    }
                }
                fclose(arqFuncionario);
            }
            else if(escolha == 2)
            {
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
                        printf("Salario: R$ %i,00\n", funcionario.salario);
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
                printf("Codigo Estadia: %d\n", estadia.codigo);
                printf("Codigo Quarto: %d\n", estadia.codQuarto);
                printf("Data de entrada: %s\n", estadia.dataEntrada);
                printf("Data de saída: %s\n", estadia.dataSaida);
                printf("Quantidade de diarias: %d\n", estadia.quantidadeDiarias);
                puts("--------------------------------------");
               // break; -> achei que era esse brea que nao tava deixando ler os outros
            }

            fread(&estadia, sizeof(Estadia),1,arqEstadia);
        }
        while (!feof(arqEstadia));
        fclose(arqEstadia);
    }

    puts("Pressione qualquer tecla para continuar!");
    getchar();
}

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
                //break;
            }

            fread(&estadia, sizeof(Estadia),1,arqEstadia);
        }
        while (!feof(arqEstadia));
        fclose(arqEstadia);
    }
    puts("Pressione qualquer tecla para continuar!");
    getchar();
}

void cabecalho(int tarefa){
    system("cls");
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
        puts("*****************************************\n");
        break;
    case 4:
        puts("*****************************************");
        puts("*                                       *");
        puts("*                 Listas                *");
        puts("*                                       *");
        puts("*****************************************");
        break;
    }
}

int main(void)
{
    int select = -1;
    setlocale(LC_ALL,"");
    while(select != 0)
    {
        cabecalho(3);
        puts("[1] - Cadastros\n[2] - Pesquisar\n[3] - Todos os cadastros\n[4] - Pontos fidelidade\n[5] - Checkout\n[0] - Sair");
        scanf("%i", &select);
        selectFunctions(select);
    }
    return 0;
}
