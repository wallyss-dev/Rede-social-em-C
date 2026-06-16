#include <stdio.h>
#include <string.h>
#include "usuario.h"

 /*
 Captura os dados via teclado para preenchimento de uma struct usuario.
 Realiza o tratamento do buffer para leitura correta de strings compostas.
 */

void lerDadosUsuario(struct usuario *u) {
    printf("Digite o id do usuario: ");
    scanf("%d", &u->id);
    
    printf("Digite o nome do usuario: ");
    getchar(); /* Limpa o caractere de nova linha '\n' residual do buffer do stdin */
    
    /* Lê a linha inteira incluindo os espaços vazios */
    fgets(u->nome, 100, stdin);
    /* Substitui a quebra de linha capturada pelo fgets por um terminador nulo */
    u->nome[strcspn(u->nome, "\n")] = '\0';
    
    printf("Digite a idade do usuario: ");
    scanf("%d", &u->idade);
    
    /* Inicializa o perfil com zero conexões de amizade */
    u->qtd_amigos = 0;
}