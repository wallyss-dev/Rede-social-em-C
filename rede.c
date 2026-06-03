#include <stdio.h>
#include "rede.h"


int buscarUsuarioPorId(struct usuario rede[], int total_usuarios, int id_procurado) {
    for (int i = 0; i < total_usuarios; i++) {
        if (rede[i].id == id_procurado) {
            return i; 
        }
    }
    return -1; 
}


void cadastrarUsuarioNaRede(struct usuario rede[], int *total_usuarios) {
    if (*total_usuarios >= MAX_USUARIOS) {
        printf("\n[ERRO]: A rede social atingiu o limite maximo de usuarios.\n");
        return;
    }
    printf("\n--- ABA DE CADASTRO ---\n");
    
    struct usuario novo_usuario;
    lerDadosUsuario(&novo_usuario);
    
    if (buscarUsuarioPorId(rede, *total_usuarios, novo_usuario.id) != -1) {
        printf("\n[ERRO]: Ja existe um usuario cadastrado com o ID %d.\n", novo_usuario.id);
        return; 
    }
    
    rede[*total_usuarios] = novo_usuario;
    printf("\n[SUCESSO]: Conta cadastrada com exito!\n");
    (*total_usuarios)++; 
}


void listarUsuariosDaRede(struct usuario rede[], int total_usuarios) {
    printf("\n--- PERFIS CADASTRADOS NA PLATAFORMA ---\n");
    if (total_usuarios == 0) {
        printf("Nenhum usuario cadastrado ate o momento.\n");
        return;
    }
    for (int i = 0; i < total_usuarios; i++) {
        printf("ID: %d | Nome: %s | Idade: %d | Total de Amigos: %d\n", 
               rede[i].id, rede[i].nome, rede[i].idade, rede[i].qtd_amigos);         
    }
    printf("----------------------------------------\n");
}


void cadastrarAmizade(struct usuario rede[], int total_usuarios) {
    int id_remetente, id_destinatario;
    printf("\n--- MANDAR PEDIDO DE AMIZADE ---\n");
    printf("Digite o seu ID: ");
    scanf("%d", &id_remetente);
    printf("Digite o ID do usuario que quer adicionar: ");
    scanf("%d", &id_destinatario);
    
    if (id_remetente == id_destinatario) {
        printf("\n[ERRO]: Nao pode mandar um pedido para si mesmo!\n");
        return;
    }
    
    int pos_remetente = buscarUsuarioPorId(rede, total_usuarios, id_remetente);
    int pos_destinatario = buscarUsuarioPorId(rede, total_usuarios, id_destinatario);
    
    if (pos_remetente == -1 || pos_destinatario == -1) {
        printf("\n[ERRO]: Um dos IDs nao foi encontrado na rede.\n");
        return;
    }
    
    if (rede[pos_remetente].qtd_amigos >= MAX_AMIGOS || rede[pos_destinatario].qtd_amigos >= MAX_AMIGOS) {
        printf("\n[ERRO]: Limite de amigos atingido por um dos usuarios.\n");
        return;
    }
    
    for (int i = 0; i < rede[pos_remetente].qtd_amigos; i++) {
        if (rede[pos_remetente].amigos[i] == &rede[pos_destinatario]) {
            printf("\n[AVISO]: Voces ja sao amigos!\n");
            return;
        }
    }
    
    rede[pos_remetente].amigos[rede[pos_remetente].qtd_amigos] = &rede[pos_destinatario];
    rede[pos_remetente].qtd_amigos++;
    
    rede[pos_destinatario].amigos[rede[pos_destinatario].qtd_amigos] = &rede[pos_remetente];
    rede[pos_destinatario].qtd_amigos++;
    
    printf("\n[SUCESSO]: Agora %s e %s sao amigos!\n", rede[pos_remetente].nome, rede[pos_destinatario].nome);
}


void listarAmigosDoUsuario(struct usuario rede[], int total_usuarios) {
    int id_procurado;
    printf("\n--- LISTAR AMIGOS DE UM USUARIO ---\n");
    printf("Digite o ID do usuario para ver a lista: ");
    scanf("%d", &id_procurado);
    
    int pos = buscarUsuarioPorId(rede, total_usuarios, id_procurado);
    if (pos == -1) {
        printf("\n[ERRO]: Usuario nao encontrado.\n");
        return;
    }
    
    printf("\nAmigos de %s (ID: %d):\n", rede[pos].nome, rede[pos].id);
    if (rede[pos].qtd_amigos == 0) {
        printf("Este usuario ainda nao adicionou nenhum amigo.\n");
        return;
    }
    
    for (int i = 0; i < rede[pos].qtd_amigos; i++) {
        struct usuario *amigo = rede[pos].amigos[i];
        printf("- ID: %d | Nome: %s\n", amigo->id, amigo->nome);
    }
}


void removerAmigoDaLista(struct usuario *u, int id_remover) {
    for (int i = 0; i < u->qtd_amigos; i++) {
        if (u->amigos[i]->id == id_remover) {
            
            for (int j = i; j < u->qtd_amigos - 1; j++) {
                u->amigos[j] = u->amigos[j + 1];
            }
            u->qtd_amigos--;
            return;
        }
    }
}


void removerAmizade(struct usuario rede[], int total_usuarios) {
    int id_remetente, id_destinatario;
    printf("\n--- DESFAZER AMIZADE ---\n");
    printf("Digite o seu ID: ");
    scanf("%d", &id_remetente);
    printf("Digite o ID do amigo a ser removido: ");
    scanf("%d", &id_destinatario);

    int pos_remetente = buscarUsuarioPorId(rede, total_usuarios, id_remetente);
    int pos_destinatario = buscarUsuarioPorId(rede, total_usuarios, id_destinatario);

    if (pos_remetente == -1 || pos_destinatario == -1) {
        printf("\n[ERRO]: Um ou ambos os IDs nao foram encontrados.\n");
        return;
    }

    
    removerAmigoDaLista(&rede[pos_remetente], id_destinatario);
    removerAmigoDaLista(&rede[pos_destinatario], id_remetente);

    printf("\n[SUCESSO]: O vinculo de amizade foi desfeito!\n");
}


void removerUsuario(struct usuario rede[], int *total_usuarios) {
    int id_remover;
    printf("\n--- EXCLUIR CONTA ---\n");
    printf("Digite o ID do usuario que deseja excluir: ");
    scanf("%d", &id_remover);

    int pos = buscarUsuarioPorId(rede, *total_usuarios, id_remover);
    if (pos == -1) {
        printf("\n[ERRO]: Usuario nao encontrado.\n");
        return;
    }

     
    struct usuario *u_excluido = &rede[pos];
    for (int i = 0; i < u_excluido->qtd_amigos; i++) {
        struct usuario *amigo = u_excluido->amigos[i];
        removerAmigoDaLista(amigo, id_remover);
    }

    
    int pos_ultimo = *total_usuarios - 1;
    if (pos != pos_ultimo) {
        rede[pos] = rede[pos_ultimo];

        
        struct usuario *u_movido = &rede[pos];
        for (int i = 0; i < u_movido->qtd_amigos; i++) {
            struct usuario *amigo = u_movido->amigos[i];
            
            for (int j = 0; j < amigo->qtd_amigos; j++) {
                if (amigo->amigos[j]->id == u_movido->id) {
                    amigo->amigos[j] = &rede[pos]; // Corrige o ponteiro
                    break;
                }
            }
        }
    }
    
    (*total_usuarios)--;
    printf("\n[SUCESSO]: Usuario e todas as suas amizades foram removidos!\n");
}