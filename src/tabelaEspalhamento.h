#ifndef __tabelaEspalhamento__
#define __tabelaEspalhamento__

#include "lista.h"

/* Ponteiros para a estrutura que representa um item da tabela e a tabela em si, respectivamente */
typedef void *Item;
typedef void *HashTable;

/* Função responsável por inicializar uma tabela, definindo seu tamanho e criando as listas de colisões */
HashTable iniciaTabela(int tamanho);

/* Função que gera o índice em que uma chave deve estar */
int getChave(char *chave, int tamanhoHash);

/* Função que adiciona um item à tabela */
void adicionaItem(HashTable t, char *chave, Item valor);

/* Função que deleta um único item da tabela */
void deletaItem(HashTable t, char *chave);

/* Função em que dada uma chave, retorna seu respectivo valor */
Item getValor(HashTable t, char *chave);

/* Função que deleta a tabela completamente */
void deletaTabela(HashTable t);

/* Função para debug, imprime a tabela no console */
void imprimeTabela(HashTable t);

#endif