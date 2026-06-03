#include <stdio.h>
#include "rede.h" 

int main() {
    struct usuario rede[MAX_USUARIOS];
    int total_usuarios = 0; 
    int escolha; 
    
    do {
        printf("\n====================================\n");
        printf("     == MINI FACEBOOK ==            \n");
        printf("====================================\n");
        printf("1. Criar conta\n");
        printf("2. Excluir conta\n");
        printf("3. Listar contas cadastradas\n");
        printf("4. Mandar pedido de amizade\n");
        printf("5. Desfazer amizade\n");
        printf("6. Listar amigos\n");
        printf("7. Sair\n");
        printf("====================================\n");
        printf("Escolha uma das opcoes: ");
        
        scanf("%d", &escolha);
        
        if (escolha == 1) {
            cadastrarUsuarioNaRede(rede, &total_usuarios);
            
        } else if (escolha == 2) {
            removerUsuario(rede, &total_usuarios);
            
        } else if (escolha == 3) {
            listarUsuariosDaRede(rede, total_usuarios);
            
        } else if (escolha == 4) {
            cadastrarAmizade(rede, total_usuarios); 
                
        } else if (escolha == 5) {
            removerAmizade(rede, total_usuarios);
            
        } else if (escolha == 6) {
            listarAmigosDoUsuario(rede, total_usuarios); 
            
        } else if (escolha == 7) {
            printf("\n[Encerrando o sistema......]\n");
            
        } else {
            printf("\n[AVISO]: Opcao invalida! Digite um numero de 1 a 7.\n");
        }

    } while (escolha != 7); 

    return 0;   
}