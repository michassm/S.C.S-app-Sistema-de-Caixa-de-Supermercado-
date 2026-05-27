#include <stdio.h>
/*
    S.C.S app (Sistema de Caixa de Supermercado)
    SUPER MERCADO WYDEN
    Participantes: 
        Michel da Silva Moreira         RA: 202601387642
        Rute Carboni Moreira            RA: 202601733461
        Amanda Araujo Sanches           RA: 202602499452
        Manuelly Tristão de Carvalho    RA: 202602113643
*/

struct Frutas
{
    char Produto[20];
    int codProduto;
    double precoProduto;
};


void iniciarSistema(){
    printf("\n Bem vindo ao Super Mercado Wyden");
    printf("\n Deseja iniciar o atendimento? (s/n)\n");
}

int main(){
    char initSistema;

    //inicia o sistema
    iniciarSistema();
    if (initSistema == 'n')
    {
        printf("Sistema não iniciado");
    }else{

    }
     
    return 0;
}
