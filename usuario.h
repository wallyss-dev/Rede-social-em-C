#ifndef USUARIO_H
#define USUARIO_H

#define MAX_AMIGOS 10

 /*
 Estrutura que define as propriedades de um usuário na rede social.
 Contém dados primitivos e um vetor de ponteiros para referenciar as amizades.
 */
 
struct usuario {
    int id;                             /* Identificador numérico único do usuário */
    char nome[100];                     /* Armazena o nome completo do usuário */
    int idade;                          /* Idade do usuário */
    struct usuario *amigos[MAX_AMIGOS]; /* Vetor de ponteiros para outras structs do tipo usuario */
    int qtd_amigos;                     /* Contador de amizades ativas deste perfil */
};

/* Protótipo da função de captura de dados de entrada */
void lerDadosUsuario(struct usuario *u);

#endif