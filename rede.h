#ifndef REDE_H
#define REDE_H

#include "usuario.h"

#define MAX_USUARIOS 50 

/* Protótipos das funções de gerenciamento da rede social */
int buscarUsuarioPorId(struct usuario rede[], int total_usuarios, int id_procurado);
void cadastrarUsuarioNaRede(struct usuario rede[], int *total_usuarios);
void listarUsuariosDaRede(struct usuario rede[], int total_usuarios);
void cadastrarAmizade(struct usuario rede[], int total_usuarios);
void listarAmigosDoUsuario(struct usuario rede[], int total_usuarios);

/* Protótipos das funções de exclusão e manutenção de integridade */
void removerAmigoDaLista(struct usuario *u, int id_remover);
void removerAmizade(struct usuario rede[], int total_usuarios);
void removerUsuario(struct usuario rede[], int *total_usuarios);

/* Protótipos para persistência de dados */
void salvarRede(struct usuario rede[], int total_usuarios);
void carregarRede(struct usuario rede[], int *total_usuarios);

#endif