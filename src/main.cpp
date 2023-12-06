#include <EEPROM.h>
#include <cstring>
#include <ESP8266WiFi.h>

#include "interface.h"




void limparMemoria() {
  for (int i = 0; i < 250; i++) {
    EEPROM.write(i,0b00000000);
    //delay(4);
  }
}

void gravarDispositivoTeste() {
  strcpy(d.nome, "teste");
  strcpy(d.funcoes[0], "funcao1");
  strcpy(d.funcoes[1], "funcao2");
  for (uint8_t i = 0; i < 6; i++) {
    d.macAddress[i] = i;
  }
  Memoria::gravaDispositivo(d);
}

void lerDispositivos() {
  int res = Memoria::retornaDispositivos(ds);
  for (int i = 0; i < res; i++) {
    Serial.print("Nome: ");
    Serial.println(ds[i].nome);
    Serial.println("Funcoes: ");
    for (int j = 0; j < 3; j++) {
      if (ds[i].funcoes[j][0] != '\0') {
        Serial.println(ds[i].funcoes[j]);
      }
    }
    Serial.print("Mac address: ");
    for (int k = 0; k < 6; k++) {
      Serial.print(ds[i].macAddress[k]);
      Serial.print(":");
    }
    Serial.print("\n\n");
  }
}

void leFlags() {
  for (int i = 0; i < 5; i++) {
    Serial.print("Endereco ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(EEPROM.read(i));
  }
}

MaquinaEstados *maquinaEstados;

void setup() {
  // put your setup code here, to run once:
  maquinaEstados = MaquinaEstados::getInstance();
  maquinaEstados->setEstado(new TelaMenu(0,3));
  inicializaLcd();
  inicializaBotoes();
  Serial.begin(9600);
  Memoria::inicializa();
  WiFi.mode(WIFI_STA);
  //Inicializa o ESP-NOW
  
  Serial.println();
  Serial.print("Endereco MAC: ");
  Serial.println(WiFi.macAddress());
}



void loop() {
  // put your main code here, to run repeatedly:
  maquinaEstados->loop();
  if (Serial.available() > 0) { // Verifica se há dados disponíveis para leitura
    char receivedChar = Serial.read(); // Lê o próximo caractere disponível na porta serial

    if (receivedChar == '1') {
      Serial.println(receivedChar);
      gravarDispositivoTeste();
    } else if (receivedChar == '2') {
      Serial.println(receivedChar);
      lerDispositivos();
    } else if (receivedChar == '3') {
      Serial.println(receivedChar);
      limparMemoria();
    } else if (receivedChar == '4') {
      Serial.println(receivedChar);
      leFlags();
    } else if (receivedChar == '5') {
      Serial.println(receivedChar);
      uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
      uint8_t res = Memoria::deletarDispositivo(mac);
      Serial.println(res); 
    } else if (receivedChar == '6') {
      Serial.println(receivedChar);
      Serial.println();
      Serial.print("Endereco MAC: ");
      Serial.println(WiFi.macAddress());
    }
  }
  delay(1000/30);
}
