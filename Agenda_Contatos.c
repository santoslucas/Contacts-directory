#include <stdio.h>
#include <stdlib.h>

struct data
{
    int dia, mes, ano;
};

struct contato
{
    char nome[50], celular[22];
    struct data data1;
};

/*
    O void salvar_agenda e' chamado 3 vezes: antes de finalizar o programa, no void inserir_contato e na main apos um contato ser excluido.

    Primeiramente, o antigo arquivo e' sobrescrevido ("w+") para leitura/escrita.
    Dessa forma, como agora o arquivo esta zerado, e' inserido por meio de "fprintf" cada dado dos contatos no arquivo.
    Note que os dados sao formatados para, posteriormente, serem lidos do arquivo da forma certa.
    Por meio do "for", todos os contatos sao impressos no arquivo.

    Depois, temos a mesma logica para salvar o arquivo binario, no entanto, por meio de 1 fwrite.
*/
void salvar_agenda(struct contato *p, int posicao, FILE *agenda, int tipo_arquivo)
{
    agenda = fopen("texto.txt", "w+");
    for(int i=0; i<posicao; i++)
    {
        fprintf(agenda, "%s\n",p[i].nome);
        fprintf(agenda, "%s\n", p[i].celular);
        fprintf(agenda, "%d/%d/%d\n", p[i].data1.dia, p[i].data1.mes, p[i].data1.ano);
    }
    fclose(agenda);

    agenda = fopen("binario.bin", "w+b");
    fwrite(p, sizeof(struct contato), posicao, agenda);

    //se o programa inicalmente carregou o aquivo de texto, o binario e' fechado e o de texto e' reaberto
    //o arquivo binario nao precisa de tal procedimento, pois ja foi aberto no "for" anterior
    if(tipo_arquivo == 1)
    {   
        fclose(agenda);
        agenda = fopen("texto.txt", "r+"); //abre um arquivo ja existente para leitura/escrita
    }
}

// A funcao le os dados do contato e grava na posicao apontada pelo ponteiro "p".
// Alem disso, ao final e' chamado o procedimento "salvar_agenda" para salvar os contatos "p[]" no arquivo da agenda.
void inserir_contato(struct contato *p, int posicao, FILE *agenda, int tipo_arquivo)
{

    printf("\nInsira o nome do contato:\n\n");
    getchar();
    gets(p[posicao].nome);

    printf("\nInsira o numero do contato (sem espacamento, apenas caracteres):\n");
    gets(p[posicao].celular);

    printf("\nInsira a data de nascimento do contato (com espacamento entre d m a):\n");
    scanf("%d %d %d", &p[posicao].data1.dia, &p[posicao].data1.mes, &p[posicao].data1.ano);

    salvar_agenda(p, posicao, agenda, tipo_arquivo); //salvar no arquivo.
    printf("\n*** O contato foi salvo. ***\n");
}

/*  Como o void recebe o valor da "posicao", que se refere ao numero de contatos inseridos durante a execucao do codigo,
    por meio do "for" ele passa por todos os dados de todos contatos e imprime eles.
*/
void exibir_contatos(struct contato *p, int posicao)
{
    printf("\n\n   CONTATOS:");
    for(int i=0; i<posicao; i++)
    {
        printf("\n%i. %s\n", (i+1), p[i].nome); //(i+1), pois o primeiro contato ocupa a posicao 0. Assim a impressao fica melhor.
        printf("   %s\n", p[i].celular);
        printf("   %i/%i/%i\n", p[i].data1.dia, p[i].data1.mes, p[i].data1.ano);
    }
}

/*
    Primeiramente e' lido o nome que o usuario deseja procurar.
    Depois por meio de 4 "for" e' buscado essa substring digitada pelo usuario (nome_procurado) em cada contato (strings).

    No primeiro "for", verifica-se o tamanho da substring "nome_procurado".
    No segundo "for", "h" serve para passarmos por cada nome de contato salvo (p[h].nome).
    No terceiro "for", "i" serve para passar por cada letra do nome do contato (p[h].nome[i]). 
    Se a letra do nome do contato (p[h].nome[i]) for igual a primeira letra da substring "nome_procurado", significa que e' um possivel comeco da substring na string.
    Dessa forma, entramos no quarto "for".
    No quarto "for", verfica-se quantas letras a partir da primeira letra da substring sao iguais as letras da string (p[h].nome) de forma consecutiva.
    O valor de letras consecutivas iguais na string e na substring e' armazenado no "cont".
    Se o "cont" for iguais ao "tamanho" da substring, significa que a substring(nome_procurado) esta dentro da string (p[h].nome).
    Então, e' impresso os dados do contato.
*/
void procurar_contato(struct contato *p, int posicao)
{
    char nome_procurado[50];
    printf("\n\nDeseja buscar qual contato?\n");
    getchar();
    gets(nome_procurado);

    printf("\n\n   CONTATOS ENCONTRADOS:");

    int h, i, j, k, cont=0, tamanho=0;
    for(k=0; nome_procurado[k]!='\0'; k++)
    {
        tamanho++; //tamanho da substring
    }
    
    for(h=0; h<posicao; h++)
    {
        for(i=0; p[h].nome[i]!='\0'; i++)
        {
            if(p[h].nome[i] == nome_procurado[0]) //[i] e' um possivel comeco da substring na string
            {
                //analisa a igualdade das strings a partir do primeiro caracter igual
                for(j=0, k=i; nome_procurado[j]==p[h].nome[k] && nome_procurado[j]!='\0'; j++, k++)
                {
                    cont++; //quantidade de letras consecutivas iguais
                }

                if(cont == tamanho) //se o "cont" for igual ao "tamanho", então a substring faz parte da string
                {
                    printf("\n%i. %s\n", h, p[h].nome);
                    printf("   %s\n", p[h].celular);
                    printf("   %i/%i/%i\n", p[h].data1.dia, p[h].data1.mes, p[h].data1.ano);
                    cont=0; //reinicia para a busca no proximo contato
                    break; //ja encontrou uma substring na string, portanto pode ser interrompido
                }

                else cont=0; //reinicia para nova aparicao de um caracter igual entre as strings
            }
        }
    }
}

/*
    O procedimento chama "void procurar_contato" para listar os nomes pesquisados.
    Apos isso, pergunta qual numero esta antecendendo o contato que deseja apagar, que e' equivalente a "posicao" do contato (p[posicao].nome).
    Esse numero e' associado a variavel "numero_excluir".
    No "for", como i=numero_excluir, o contato que sera excluido (p[i]) e' sobrescrevido pelo proximo contato da lista e assim sucessivamente.
    Portanto, como o antigo ultimo contato agora e' o penultimo contato, o novo ultimo contato e' agora resto de memoria.
    Apos isso, a memoria e' realocada na funcao main e esse resto de memoria e' excluido.
    
*/
void excluir_contato(struct contato *p, int posicao)
{
    procurar_contato(p, posicao); //listar o resultado da busca de nomes para excluir

    int numero_excluir;
    printf("\nDigite o numero que antecede o nome do contato que deseja EXCLUIR:\n");
    scanf("%i", &numero_excluir);
    for(int i=numero_excluir; i<posicao; i++)
    {
        p[i] = p[i+1]; //sobresceve o contato que desenha excluir e os demais avancam uma posicao (i).
    }
}

/*
    Na main, inicialmente temos a parte de carregar os contatos do arquivo de texto e do binario(nao funciona).
    E' perguntado se queremos carregar os contatos do arquivo binario ou de texto. A resposta e' salva em "tipo_arquivo".
    Temos 2 "if" para selecionar o carregamento desejado.

    Na parte de arquivo de texto, e' aberto o arquivo para leitura/escrita. Se ele nao existe um e' criado.
    Se ele ja existe, "tamanho" e' acrescido em 1 e a memoria destinada a "p" e' realocada para 1 contato (struct contato).
    Os "fscanf" carregam os dados dos contatos na formatacao certa.
    "Posicao" e' acrescida em 1, pois 1 contato foi salvo.
    O processo e' executado ate o arquivo acabar "while(!feof(agenda))".

    O mesmo processo e' repetido na parte do arquivo binario, mas essa parte precisa de ser compensada com outra reducao na alocacao.
    Apos isso temos o MENU, que esta comentado no proprio.
*/
int main()
{   
    // inicialmente aloca 0 posicoes de memoria. Em na parte de inserir contato e' realocada, assim como na parte de carregar os contatos do arquivo.
    // p e' um ponteiro da struct contato.
    int tamanho=0, posicao=0, tipo_arquivo; //tamanho=0 para alocar 0 espacos de memoria, posicao=0 pois nenhum contato foi inserido.
    struct contato *p = (struct contato*) malloc(tamanho*sizeof(struct contato));
    FILE *agenda;

    // pergunta ao usuario se deseja carregar a agenda do arquivo texto ou do arquivo binario.
    printf("Deseja carregar a agenda do arquivo texto ou do arquivo binario?\n-arquivo texto(1)\n-arquivo binario(2)\n");
    scanf("%i", &tipo_arquivo);
    
    
    if(tipo_arquivo == 1)
    {
        agenda = fopen("texto.txt", "r+");//le/escreve num arquivo existente.
        if(agenda == NULL) //se o arquivo ainda nao foi criado, fopen retorna NULL.
        {
            agenda = fopen("texto.txt", "w+"); //se o arquivo nao existir, um e' criado.
        }
        else //se ja foi criado, os contatos sao carregados.
        {
            while(!feof(agenda))
            {
                tamanho++; //aumenta o tamanho em 1 para a realocacao destinar memoria para 1 contato (struct contato).
                p = (struct contato*) realloc(p, (tamanho)*sizeof(struct contato));
                fscanf(agenda, "%[^\n]s",p[posicao].nome);
                fscanf(agenda, "%s\n", p[posicao].celular);
                fscanf(agenda, "%d/%d/%d\n", &p[posicao].data1.dia, &p[posicao].data1.mes, &p[posicao].data1.ano);
                posicao++; //posicao aumenta pois 1 contato foi salvo.
            }
        }
    }
    
    else if(tipo_arquivo == 2)
    {
        agenda = fopen("binario.bin", "r+b");
        if(agenda == NULL)
        {
            agenda = fopen("binario.bin", "w+b");
        }
        else
        {
            while(!feof(agenda))
            {
                tamanho++;
                p = (struct contato*) realloc(p, (tamanho)*sizeof(struct contato));
                fread(&p[posicao], sizeof(struct contato), 1, agenda);
                posicao++;
            }
            posicao--;
            tamanho--;
            p = (struct contato*) realloc(p, (tamanho)*sizeof(struct contato));
        }
    }

    //*** MENU ***
    // enquanto continuar=1 o programa se repete.
    int continuar=1;
    while(continuar)
    {

        int operacao;
        printf("\nQual operacao deseja executar:\n\n-Inserir contato(1)\n-Exibir todos contatos(2)\n-Procurar contato(3)\n-Excluir contato(4)\n");
        scanf("%i", &operacao); //operacao que o programa deve executar

        /*
            E' adicionado 1 ao "tamanho" para que seja realocada memoria para salvar +1 contato.
            E' passado para funcao "inserir_contato" a "posicao", que se refere ao primeiro contato inserido na execucao do codigo.
            Alem disso e' passado o enderco que o ponteiro "p" da struct contato aponta, o aquivo "agenda" e o "tipo_arquivo".
            Apos a execucao da funcao, e' somado 1 a "posicao", pois foi inserido 1 contato.
        */
        if(operacao == 1)
        {
            tamanho++;
            p = (struct contato*) realloc(p, (tamanho)*sizeof(struct contato));
            inserir_contato(p, posicao, agenda, tipo_arquivo);
            posicao++;
        }

        // Para os 3 "else if":
        // E' passado para o procedimento o endereco para onde "p" aponta, o valor de "posicao" que se refere a quantos contatos foram lidos.
        else if(operacao == 2)
        {
            exibir_contatos(p, posicao); //chama o void
        }

        else if(operacao == 3)
        {
            procurar_contato(p, posicao);//chama o void
        }

        else if(operacao == 4)
        {
            excluir_contato(p, posicao); //chama o void
            tamanho--; //pois agora temos -1 contato
            posicao--; //pois agora temos -1 contato
            p = realloc(p, (tamanho)*sizeof(struct contato)); //a memoria e' realocada para (tamanho--), pois 1 contato foi apagado. Assim o lixo de memoria e' apagado.
            printf("\n*** O contato foi excluido. ***\n");
            salvar_agenda(p, posicao, agenda, tipo_arquivo); //os 2 tipos de arquivo sao sobrescrevidos com a nova lista de contatos.
        }
        
        printf("\nDeseja finalizar o programa?\n\nSim(0)\nNao(1)\n\n");
        scanf("%i", &continuar); //se o usuario digitar 0 o while e' interrompdo e o programa acaba.
    }

    salvar_agenda(p, posicao, agenda, tipo_arquivo); //antes de encerrar o programa, os 2 tipos de arquivo sao sobrescrevidos com a nova lista de contatos.
    printf("\n*** PROGRAMA ENCERRADO ***\n");
    free(p); //libera a memoria alocada.
    fclose(agenda); //fecha e salva o arquivo.

    return 0;
}