#include "comunicador.h"

bool recebido = false;
Dispositivo d;
Dispositivo ds[5];
Mensagem mensagem;

bool statusMensagem;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  if (sendStatus == 0) {
    statusMensagem = true;
  }
  else {
    statusMensagem = false;
  }
}

void limparDispositivos() {
  for (int i = 0; i < 5; i++) {
    ds[i].nome[0] = '\0';
    ds[i].funcoes[i][0] = '\0';
    ds[i].funcoes[i][1] = '\0';
    ds[i].funcoes[i][2] = '\0';
  }
}

void ativarEspNow() {
  if (esp_now_init() != 0) {
    Serial.println("Erro ao inicializar ESPNow");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  delay(500);
  Serial.println("ESPNow ativado!");
  recebido=false;
}

bool chamarFuncao(uint8_t mac[6], char funcao[10]) {
  ativarEspNow();
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  
  strcpy(mensagem.funcao, funcao); 
  esp_now_send(mac, (uint8_t *) &mensagem, sizeof(Mensagem));

  desativarEspNow();
  return statusMensagem;
}

void desativarEspNow() {
  if (esp_now_deinit() != 0) {
    Serial.println("Erro ao desativar ESPNow");
    return;
  }
  Serial.println("ESPNow desativado!");
  recebido=false;
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&d, incomingData, sizeof(Dispositivo));
  Serial.println();
  Serial.print("Bytes recebidos: ");
  Serial.println(len);
  Serial.print("Nome: ");
  Serial.println(d.nome);
  Serial.print("Funcao: ");
  Serial.println(d.funcoes[0]);
  Serial.print("mac: ");
 
  for (int i = 0; i < 6; i++) {
    Serial.print(d.macAddress[i]);
    Serial.print(":");
  }
  Serial.println();
  recebido = true;
  //Acende ou apaga o led de acordo com a String recebida
}