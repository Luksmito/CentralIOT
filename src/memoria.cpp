#include "memoria.h"




uint8_t calculaEnderecoNome(uint8_t enderecoInicial) {
  return enderecoInicial;
}
uint8_t calculaEnderecoFuncoes(uint8_t enderecoInicial) {
  return enderecoInicial + 10;
}
uint8_t calculaEnderecoMac(uint8_t enderecoInicial) {
  return enderecoInicial + 40;
}
void gravaNome(uint8_t enderecoNome, char nome[10]) {
  int i = 0;
  while(nome[i] != '\0') {
    EEPROM.write(enderecoNome+i, nome[i]);
    //delay(4);
    i++;
    
  }
  EEPROM.write(enderecoNome+i, 0b00000000);
  //delay(4);
  //EEPROM.commit();
}
void gravaMac(uint8_t enderecoMac, uint8_t mac[6]) {
  for (int i = 0; i < 6; i++) {
    EEPROM.write(enderecoMac+i, mac[i]);
    //delay(4);
  }
  //EEPROM.commit();
}
void gravaFuncoes(uint8_t enderecoFuncoes, char funcoes[3][10]) {
  for (int i = 0; i < 3; i++) {
      uint8_t contador = 0;
      while(funcoes[i][contador] != '\0') {
        EEPROM.write(enderecoFuncoes + i*10 + contador, funcoes[i][contador]);
        //delay(4);
        contador++;
      }
      EEPROM.write(enderecoFuncoes + i*10 + contador, 0);
      //delay(4);
  }
  
  //EEPROM.commit();
}

void leNome(uint8_t enderecoNome, char nome[10]) {
  Serial.print("Endereco nome: ");
  Serial.println(enderecoNome);
  char c = EEPROM.read(enderecoNome);
  //delay(4);
  int contador = 0;
  while ( c != '\0' ) {
    nome[contador] = c;
    contador++;
    c=EEPROM.read(enderecoNome+contador);
    //delay(4);
  }
  nome[contador] = '\0';
}

void leFuncoes(uint8_t enderecoFuncoes, char funcoes[3][10]) {
  Serial.print("Endereco das funcoes: ");
  Serial.println(enderecoFuncoes);
  for(int i = 0; i < 3; i++) {
    char c;
    int contador = 0;
    c=EEPROM.read(i*10+enderecoFuncoes);
    //delay(4);
    Serial.print("lido caractere: ");
    Serial.println(c);
    
    while(c != '\0') {
      funcoes[i][contador] = c;
      contador++;
      c=EEPROM.read(i*10+enderecoFuncoes+contador);
      //delay(4);
      Serial.print("lido caractere: ");
      Serial.println(c);
      
    }
    funcoes[i][contador]='\0';
  }

}


void leMac(uint8_t enderecoMac, uint8_t mac[6]) { 
  Serial.print("Endereco do mac: ");
  Serial.println(enderecoMac);
  for (int i = 0; i < 6; i++) {
    mac[i] = EEPROM.read(enderecoMac+i);
    //delay(4);
  }
}

bool macsIguais(uint8_t mac1[6], uint8_t mac2[6]) {
  for (int i = 0; i < 6; i++) {
    if (mac1[i] != mac2[i]) return false;
  }
  return true;
}



void Memoria::inicializa() {
    EEPROM.begin(250); // Especifique o tamanho total da EEPROM aqui
}

void Memoria::fecha() {
    EEPROM.end();
}

bool Memoria::enderecoMacExiste(uint8_t mac[6]) {
    int enderecoInicial = -1;
    uint8_t conteudo = 0;
    for (int i = 0; i < N_DISPOSITIVOS; i++) {
        conteudo = EEPROM.read(i);
        
        if (conteudo == 255) {
        enderecoInicial = enderecosDispositivos[i][0];
        uint8_t enderecoMac = calculaEnderecoMac(enderecoInicial);
        uint8_t mac1[6];
        leMac(enderecoMac, mac1);
        if (macsIguais(mac,mac1)) {
            return true;
        }
        }
    }
    return false;
    }

void printaMac(uint8_t mac[6]) {
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i]);
    Serial.print(':');
  }
  Serial.println();
}

uint8_t Memoria::deletarDispositivo(uint8_t mac[6]) {
    int enderecoInicial = -1;
    uint8_t conteudo = 0;
    for (int i = 0; i < N_DISPOSITIVOS; i++) {
        conteudo = EEPROM.read(i);
        
        if (conteudo == 255) {
          enderecoInicial = enderecosDispositivos[i][0];
          uint8_t enderecoMac = calculaEnderecoMac(enderecoInicial);
          uint8_t mac1[6];
          leMac(enderecoMac, mac1);
          Serial.println("Mac lido: ");
          printaMac(mac1);
          Serial.println("Mac recebido: ");
          printaMac(mac);
          if (macsIguais(mac,mac1)) {
              EEPROM.write(i, 0);
              EEPROM.commit();
              return DISPOSITIVO_DELETADO;
          }
        }
       
    }
    return ERRO_AO_DELETAR;
}

uint8_t Memoria::gravaDispositivo(Dispositivo dispositivo) {
    int enderecoInicial = -1;
    uint8_t conteudo = 0;
    uint8_t enderecoFlagDispositivo;
    for (int i = 0; i < N_DISPOSITIVOS; i++) {
        conteudo = EEPROM.read(i);
        //delay(4);
        if (conteudo == 0) {
        enderecoInicial = enderecosDispositivos[i][0];
        enderecoFlagDispositivo = i;
        break;
        }
    }
    Serial.print("Endereco Flag: ");
    Serial.println(enderecoFlagDispositivo);
    if (enderecoInicial == -1) {
        return MAXIMO_DISPOSITIVOS_CADASTRADOS;
    }

    Serial.print("Endereco Inicial: ");
    Serial.println(enderecoInicial);

    uint8_t enderecoNome = calculaEnderecoNome(enderecoInicial);
    uint8_t enderecoMac = calculaEnderecoMac(enderecoInicial);
    uint8_t enderecoFuncoes = calculaEnderecoFuncoes(enderecoInicial);

    Serial.print("Endereco nome: ");
    Serial.println(enderecoNome);
    Serial.print("Endereco mac: ");
    Serial.println(enderecoMac);
    Serial.print("Endereco funcoes: ");
    Serial.println(enderecoFuncoes);

    gravaNome(enderecoNome, dispositivo.nome);
    gravaMac(enderecoMac, dispositivo.macAddress);
    gravaFuncoes(enderecoFuncoes, dispositivo.funcoes);
    EEPROM.write(enderecoFlagDispositivo, 255);
    EEPROM.commit();
    uint8_t teste=EEPROM.read(enderecoFlagDispositivo);
    Serial.print("Endereco Flag recuperado: ");
    Serial.println(enderecoFlagDispositivo);
    return DISPOSITIVO_CADASTRADO;
}

uint8_t Memoria::retornaDispositivos(Dispositivo dispositivo[5]) {
    uint8_t contador = 0;
    for (int i = 0; i < 5; i++) {
        uint8_t flag = EEPROM.read(i);
        //delay(4);
        if (flag == 255) {
          leNome(calculaEnderecoNome(enderecosDispositivos[i][0]), dispositivo[contador].nome);
          leFuncoes(calculaEnderecoFuncoes(enderecosDispositivos[i][0]), dispositivo[contador].funcoes);
          leMac(calculaEnderecoMac(enderecosDispositivos[i][0]), dispositivo[contador].macAddress);
          contador++;
        }
    }
    return contador;
}


// Defina o array fora da classe com a inicialização dos endereços
uint8_t Memoria::enderecosDispositivos[N_DISPOSITIVOS][2] = {
    {20, 65},
    {66, 111},
    {112, 157},
    {158, 203},
    {204, 249}
};
