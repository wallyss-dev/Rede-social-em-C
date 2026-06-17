# Mini Facebook em C

Este repositório contém o projeto final desenvolvido para a disciplina de **Algoritmo, Estruturas de Dados e Programação** do curso de Inteligência Artificial e Ciência de Dados no **Piauí Instituto de Tecnologia (PIT)**, sob a orientação do **Professor Joselito Junior**.

O sistema consiste em um simulador de rede social baseado em console, operando em conformidade com o padrão **C99**. O projeto aplica conceitos fundamentais de estruturas de dados personalizadas (`struct`), modularização de escopo em arquivos-fonte e cabeçalhos, tratamento avançado de buffers de entrada/saída e mapeamento de relacionamentos bidirecionais por meio de vetores de ponteiros.

## 📁 Arquitetura do Repositório

O código-fonte é rigorosamente segmentado em módulos independentes para garantir alta coesão e baixo acoplamento:

* **`usuario.h`**: Contrato técnico contendo a definição da `struct usuario`, constantes de dimensionamento (`MAX_AMIGOS`) e assinatura do coletor de dados.
* **`usuario.c`**: Implementação das rotinas isoladas do perfil de usuário, gerenciando a entrada padrão de dados e tratamento de strings.
* **`rede.h`**: Definição da volumetria global da plataforma (`MAX_USUARIOS = 50`) e protótipos de controle do grafo social.
* **`rede.c`**: O núcleo lógico do sistema. Contém os algoritmos de busca linear, casamento de IDs, estabelecimento de amizades cruzadas, shifting de vetores e deleção lógica com atualização retroativa de ponteiros.
* **`main.c`**: Orquestrador central. Gerencia o fluxo de execução contínuo através de um menu interativo textual.

## 🛠️ Especificações de Compilação

Para compilar o ecossistema de arquivos de forma otimizada e com checagem estrita de integridade de código através do compilador GCC, execute o comando abaixo no terminal de sua preferência:

```bash
gcc -Wall -Wextra -O2 -std=c99 main.c rede.c usuario.c -o mini_facebook
