#include <stdio.h>
#include <string.h> /* Necessário para strcspn */
#include "rede.h"

 /*
 Varre a rede sequencialmente procurando por um ID específico.
 Retorna o índice no vetor se achar, ou -1 caso não exista.
 */
 
int buscarUsuarioPorId(struct usuario rede[], int total_usuarios, int id_procurado) {
    for (int i = 0; i < total_usuarios; i++) {
        if (rede[i].id == id_procurado) {
            return i; /* Retorna a posição (índice) do elemento */
        }
    }
    return -1; 
}

 /*
 Grava as informações da memória RAM nos arquivos físicos de texto (.txt).
 Garante a integridade referencial salvando apenas pares de IDs das amizades.
 */
void salvarRede(struct usuario rede[], int total_usuarios) {
    FILE *f_usuarios = fopen("usuarios.txt", "w");
    FILE *f_amizades = fopen("amizades.txt", "w");

    if (f_usuarios == NULL || f_amizades == NULL) {
        printf("\n[ERRO]: Nao foi possivel abrir os arquivos para salvar os dados.\n");
        if (f_usuarios) fclose(f_usuarios);
        if (f_amizades) fclose(f_amizades);
        return;
    }

    /* Percorre o vetor de usuários gravando os dados em formato CSV personalizado */
    for (int i = 0; i < total_usuarios; i++) {
        fprintf(f_usuarios, "%d;%d;%s\n", rede[i].id, rede[i].idade, rede[i].nome);

        /* Percorre a lista de amigos e salva as relações */
        for (int j = 0; j < rede[i].qtd_amigos; j++) {
            /* (rede[i].id < rede[i].amigos[j]->id) impede a redundância de salvar pares invertidos */
            if (rede[i].id < rede[i].amigos[j]->id) {
                fprintf(f_amizades, "%d;%d\n", rede[i].id, rede[i].amigos[j]->id);
            }
        }
    }

    fclose(f_usuarios);
    fclose(f_amizades);
    printf("\n[SISTEMA]: Banco de dados atualizado com sucesso! (Dados salvos)\n");
}

 /*
 Reconstrói a rede social a partir dos arquivos físicos durante a inicialização.
 Restaura os dados cadastrais e reconstrói as conexões de ponteiros em tempo de execução.
 */
void carregarRede(struct usuario rede[], int *total_usuarios) {
    FILE *f_usuarios = fopen("usuarios.txt", "r");
    
    if (f_usuarios == NULL) {
        *total_usuarios = 0; /* Arquivo ausente indica primeira execução do software */
        return;
    }

    *total_usuarios = 0;
    
    /* Etapa 1: Carrega os dados básicos de cada perfil */
    while (!feof(f_usuarios)) {
        struct usuario u;
        u.qtd_amigos = 0; 

        if (fscanf(f_usuarios, "%d;%d;", &u.id, &u.idade) == 2) {
            fgets(u.nome, 100, f_usuarios);
            u.nome[strcspn(u.nome, "\n")] = '\0'; /* Sanitização do caractere residual de nova linha */

            rede[*total_usuarios] = u;
            (*total_usuarios)++;
        }
    }
    fclose(f_usuarios);

    /* Etapa 2: Reconstrói os vínculos de ponteiros baseando-se nas chaves de IDs mapeadas */
    FILE *f_amizades = fopen("amizades.txt", "r");
    if (f_amizades != NULL) {
        int id1, id2;
        while (fscanf(f_amizades, "%d;%d\n", &id1, &id2) == 2) {
            int pos1 = buscarUsuarioPorId(rede, *total_usuarios, id1);
            int pos2 = buscarUsuarioPorId(rede, *total_usuarios, id2);

            /* Se os dois IDs forem válidos na memória, reestabelece a bidirecionalidade */
            if (pos1 != -1 && pos2 != -1) {
                rede[pos1].amigos[rede[pos1].qtd_amigos] = &rede[pos2];
                rede[pos1].qtd_amigos++;

                rede[pos2].amigos[rede[pos2].qtd_amigos] = &rede[pos1];
                rede[pos2].qtd_amigos++;
            }
        }
        fclose(f_amizades);
    }
    printf("\n[SISTEMA]: Banco de dados carregado! %d perfis restaurados com sucesso.\n", *total_usuarios);
}

 /*
 Insere um novo perfil no vetor principal da rede.
 Contém validação para evitar estouro de limite físico e IDs duplicados.
 */
void cadastrarUsuarioNaRede(struct usuario rede[], int *total_usuarios) {
    if (*total_usuarios >= MAX_USUARIOS) {
        printf("\n[ERRO]: A rede social atingiu o limite maximo de usuarios.\n");
        return;
    }
    printf("\n--- ABA DE CADASTRO ---\n");
    
    struct usuario novo_usuario;
    lerDadosUsuario(&novo_usuario);
    
    /* Verifica se o ID inserido já pertence a alguém cadastrado */
    if (buscarUsuarioPorId(rede, *total_usuarios, novo_usuario.id) != -1) {
        printf("\n[ERRO]: Ja existe um usuario cadastrado com o ID %d.\n", novo_usuario.id);
        return; 
    }
    
    rede[*total_usuarios] = novo_usuario;
    printf("\n[SUCESSO]: Conta cadastrada com exito!\n");
    (*total_usuarios)++; 
}

/* Exibe em tela a listagem completa de todas as contas armazenadas */
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

/* Mapeia e cria uma conexão bidirecional de ponteiros entre duas structs da rede */
void cadastrarAmizade(struct usuario rede[], int total_usuarios) {
    int id_remetente, id_destinatario;
    printf("\n--- MANDAR PEDIDO DE AMIZADE ---\n");
    printf("Digite o seu ID: ");
    scanf("%d", &id_remetente);
    printf("Digite o ID do usuario que quer adicionar: ");
    scanf("%d", &id_destinatario);
    
    /* Validação 1: Impede autorrelacionamento */
    if (id_remetente == id_destinatario) {
        printf("\n[ERRO]: Nao pode mandar um pedido para si mesmo!\n");
        return;
    }
    
    int pos_remetente = buscarUsuarioPorId(rede, total_usuarios, id_remetente);
    int pos_destinatario = buscarUsuarioPorId(rede, total_usuarios, id_destinatario);
    
    /* Validação 2: Verifica existência das duas contas */
    if (pos_remetente == -1 || pos_destinatario == -1) {
        printf("\n[ERRO]: Um dos IDs nao foi encontrado na rede.\n");
        return;
    }
    
    /* Validação 3: Impede estouro da capacidade interna de armazenamento de amizades */
    if (rede[pos_remetente].qtd_amigos >= MAX_AMIGOS || rede[pos_destinatario].qtd_amigos >= MAX_AMIGOS) {
        printf("\n[ERRO]: Limite de amigos atingido por um dos usuarios.\n");
        return;
    }
    
    /* Validação 4: Impede conexões redundantes/duplicadas */
    for (int i = 0; i < rede[pos_remetente].qtd_amigos; i++) {
        if (rede[pos_remetente].amigos[i] == &rede[pos_destinatario]) {
            printf("\n[AVISO]: Voces ja sao amigos!\n");
            return;
        }
    }
    
    /* Efetivação do vínculo: O remetente aponta para o destinatário e vice-versa */
    rede[pos_remetente].amigos[rede[pos_remetente].qtd_amigos] = &rede[pos_destinatario];
    rede[pos_remetente].qtd_amigos++;
    
    rede[pos_destinatario].amigos[rede[pos_destinatario].qtd_amigos] = &rede[pos_remetente];
    rede[pos_destinatario].qtd_amigos++;
    
    printf("\n[SUCESSO]: Agora %s e %s ao amigos!\n", rede[pos_remetente].nome, rede[pos_destinatario].nome);
}

/* Filtra um perfil e varre seu vetor de ponteiros exibindo o nome de suas conexões */
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

 /*
 Função utilitária interna. Localiza um ID dentro da lista de amizades de um perfil específico,
 remove o elemento rearranjando o vetor interno para não deixar lacunas estruturais.
 */
void removerAmigoDaLista(struct usuario *u, int id_remover) {
    for (int i = 0; i < u->qtd_amigos; i++) {
        if (u->amigos[i]->id == id_remover) {
            /* Desloca todos os elementos subsequentes uma posição para trás */
            for (int j = i; j < u->qtd_amigos - 1; j++) {
                u->amigos[j] = u->amigos[j + 1];
            }
            u->qtd_amigos--;
            return;
        }
    }
}

/* Desfaz de maneira mútua a ligação de ponteiros estabelecida entre duas contas */
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

    /* Remove o vínculo em ambos os sentidos da relação */
    removerAmigoDaLista(&rede[pos_remetente], id_destinatario);
    removerAmigoDaLista(&rede[pos_destinatario], id_remetente);

    printf("\n[SUCESSO]: O vinculo de amizade foi desfeito!\n");
}

 /*
 Exclui um perfil da rede, trata o efeito cascata nas listas de amigos de terceiros
 e otimiza o vetor substituindo a lacuna pelo último elemento cadastrado.
 */
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

    /* Passo 1: Varre as contas dos amigos dele e remove as menções a ele (Efeito Cascata) */
    struct usuario *u_excluido = &rede[pos];
    for (int i = 0; i < u_excluido->qtd_amigos; i++) {
        struct usuario *amigo = u_excluido->amigos[i];
        removerAmigoDaLista(amigo, id_remover);
    }

    /* Passo 2: Otimização do Vetor Estático (Move o último elemento para a vaga do excluído) */
    int pos_ultimo = *total_usuarios - 1;
    if (pos != pos_ultimo) {
        rede[pos] = rede[pos_ultimo];

        /* Passo 3: Correção Crítica de Referência de Memória.
         * Como mudamos o último usuário de endereço na RAM, precisamos avisar os amigos dele 
         * para apontarem para a nova localização.
         */
        struct usuario *u_movido = &rede[pos];
        for (int i = 0; i < u_movido->qtd_amigos; i++) {
            struct usuario *amigo = u_movido->amigos[i];
            
            for (int j = 0; j < amigo->qtd_amigos; j++) {
                if (amigo->amigos[j]->id == u_movido->id) {
                    amigo->amigos[j] = &rede[pos]; /* Atualiza o ponteiro para o novo endereço */
                    break;
                }
            }
        }
    }
    
    (*total_usuarios)--;
    printf("\n[SUCESSO]: Usuario e todas as suas amizades foram removidos!\n");
}