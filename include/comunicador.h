#ifndef _COMUNICADOR_H_
#define _COMUNICADOR_H_

#include <Arduino.h>
#include <espnow.h>
#include <cstring>

extern bool recebido;

typedef struct dispositivo {
  char nome[10];  // Array de caracteres para um nome de até 9 caracteres + 1 para o caractere nulo '\0'
  char funcoes[3][10] = { "", "", "" };  // Array bidimensional de caracteres para 3 strings de até 9 caracteres + 1 para o caractere nulo
  uint8_t macAddress[6];
} Dispositivo;

typedef struct mensagem {
  char funcao[10];
} Mensagem;

extern Mensagem mensagem;

extern Dispositivo d;
extern Dispositivo ds[5];

void limparDispositivos();

void ativarEspNow();

void desativarEspNow();

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);

void onDataSent(uint8_t *mac_addr, uint8_t sendStatus);

bool chamarFuncao(uint8_t mac[6], char funcao[10]);

#endif