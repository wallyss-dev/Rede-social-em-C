#include <stdio.h>
#include <stdlib.h>
#include "usuario.h"
#include "rede.h"

int main() {
    // Declaração da rede de usuários baseada na REDE.H
    struct usuario rede[MAX_USUARIOS];
    int total_usuarios = 0;
    
    int opcao;
    int entradaValida;

    // Inicialização: Carrega os dados salvos nos arquivos de texto para a memória
    carregarRede(rede, &total_usuarios);

    // Laço principal do menu da rede social
    do {
        // 1. EXIBIÇÃO DAS OPÇÕES DO SEU SISTEMA
        printf("\n====================================\n");
        printf("        MENU DA REDE SOCIAL         \n");
        printf("====================================\n");
        printf("1. Cadastrar Usuario\n");
        printf("2. Listar Todos os Usuarios\n");
        printf("3. Adicionar Amigo (Mandar Pedido)\n");
        printf("4. Listar Amigos de um Usuario\n");
        printf("5. Desfazer Amizade\n");
        printf("6. Excluir Conta\n");
        printf("7. Salvar e Sair\n");
        printf("------------------------------------\n");
        
        // 2. O ESCUDO: Validação estrita contra letras, símbolos ou números inválidos
        do {
            printf("Escolha uma acao (1-7): ");
            
            // scanf tenta ler um número inteiro. Se o usuário digitar uma letra, ele retorna 0
            entradaValida = scanf("%d", &opcao);

            if (entradaValida != 1 || opcao < 1 || opcao > 7) {
                printf("\n[ERRO]: Entrada invalida! Digite apenas numeros entre 1 e 7.\n\n");
                
                // Limpa completamente a "sujeira" deixada no teclado para não travar em loop
                while (getchar() != '\n');
                
                // Força o laço a continuar pedindo a digitação
                entradaValida = 0; 
            }
        } while (entradaValida == 0);

        // 3. DIRECIONAMENTO SEGURO PARA AS FUNÇÕES DO SEU REDE.C
        switch (opcao) {
            case 1:
                cadastrarUsuarioNaRede(rede, &total_usuarios);
                break;
                
            case 2:
                listarUsuariosDaRede(rede, total_usuarios);
                break;
                
            case 3:
                cadastrarAmizade(rede, total_usuarios);
                break;
                
            case 4:
                listarAmigosDoUsuario(rede, total_usuarios);
                break;
                
            case 5:
                removerAmizade(rede, total_usuarios);
                break;
                
            case 6:
                removerUsuario(rede, &total_usuarios);
                break;
                
            case 7:
                printf("\n[SISTEMA]: Salvando dados e encerrando com seguranca...\n");
                // Grava tudo o que está na memória RAM nos arquivos físicos antes de fechar
                salvarRede(rede, total_usuarios);
                printf("Ate logo!\n");
                break;
        }

    } while (opcao != 7); // O programa só fecha quando a opção digitada for o 7

    return 0;
}
