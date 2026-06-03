#include <stdio.h>
#include <string.h>
#include "usuario.h"

void lerDadosUsuario(struct usuario *u) {
    printf("Digite o id do usuario: ");
    scanf("%d", &u->id);
    
    printf("Digite o nome do usuario: ");
    getchar();
    
    fgets(u->nome, 100, stdin);
    u->nome[strcspn(u->nome, "\n")] = '\0';
    
    printf("Digite a idade do usuario: ");
    scanf("%d", &u->idade);
    
    u->qtd_amigos = 0;
}