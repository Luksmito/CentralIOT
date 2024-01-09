#ifndef MEMORIA_H
#define MEMORIA_H
#define N_DISPOSITIVOS 5

#include <Arduino.h>
#include <EEPROM.h>
#include "comunicador.h"

enum {
  MAXIMO_DISPOSITIVOS_CADASTRADOS,
  DISPOSITIVO_CADASTRADO,
  DISPOSITIVO_DELETADO,
  ERRO_AO_DELETAR
};


uint8_t calculaEnderecoNome(uint8_t enderecoInicial);

uint8_t calculaEnderecoFuncoes(uint8_t enderecoInicial);

uint8_t calculaEnderecoMac(uint8_t enderecoInicial);

void gravaNome(uint8_t enderecoNome, char nome[10]);

void gravaMac(uint8_t enderecoMac, uint8_t mac[6]);

void gravaFuncoes(uint8_t enderecoFuncoes, char funcoes[3][10]);

void leNome(uint8_t enderecoNome, char nome[10]);

void leFuncoes(uint8_t enderecoFuncoes, char funcoes[3][10]);

void leMac(uint8_t enderecoMac, uint8_t mac[6]);

bool macsIguais(uint8_t mac1[6], uint8_t mac2[6]);

class Memoria {
public:
  static uint8_t enderecosDispositivos[N_DISPOSITIVOS][2];

  static void inicializa();

  static void fecha();
 

  static bool enderecoMacExiste(uint8_t mac[6]);

  static uint8_t deletarDispositivo(uint8_t mac[6]);

  static uint8_t gravaDispositivo(Dispositivo dispositivo);

  static uint8_t retornaDispositivos(Dispositivo dispositivo[5]);
};

      



#endif