#ifndef USUARIO_H
#define USUARIO_H

#define MAX_AMIGOS 10

struct usuario {
    int id;
    char nome[100];
    int idade;
    struct usuario *amigos[MAX_AMIGOS];
    int qtd_amigos;
};

void lerDadosUsuario(struct usuario *u);

#endif