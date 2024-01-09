#include "interface.h"

enum {
    BOTAO_BAIXO,
    BOTAO_CIMA,
    BOTAO_SELECT,
    BOTAO_DEFAULT
};

int buttonUp = D3;    // Botão para cima
int buttonDown = D5;  // Botão para baixo
int buttonSelect = D4;// Botão de seleção

LiquidCrystal_I2C lcd(0x27, 16, 2);

void clearAndPrintHeader(String header) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(header);
}

void tom(int frequencia, int duracao){
  float periodo = 1000.0/frequencia; //Periodo em ms
  for (int i = 0; i < duracao/(periodo); i++){ //Executa a rotina de dentro o tanta de vezes que a frequencia desejada cabe dentro da duracao
    digitalWrite(D8, HIGH);
    delayMicroseconds(periodo*500); //Metade do periodo em ms
    digitalWrite(D8, LOW);
    delayMicroseconds(periodo*500);
  }
}

void inicializaBotoes() {
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);  
  pinMode(D8, OUTPUT);
}

uint8_t processaBotao() {
    if (digitalRead(buttonUp) == LOW) {
        tom(440, 200); 
            // Debounce
        return BOTAO_CIMA;
    }
    if (digitalRead(buttonDown) == LOW) {
        tom(220, 200); 
            // Debounce
        return BOTAO_BAIXO;
    }
    if (digitalRead(buttonSelect) == LOW) {
        tom(440, 200);  
        return BOTAO_SELECT;
    }
    return BOTAO_DEFAULT;
}

void inicializaLcd() {
  lcd.init();
  lcd.backlight();
}

void EstadoBase::renderizar() {}

void EstadoBase::processarEntrada() {}

MaquinaEstados* MaquinaEstados::instance = nullptr;


MaquinaEstados* MaquinaEstados::getInstance() {
    if (!instance) {
        instance = new MaquinaEstados();
    }
    return instance;
}

void MaquinaEstados::setEstado(EstadoBase *novoEstado) {
    if (estadoAtual != nullptr) {
        delete estadoAtual; // Liberando a memória do estado anterior
    }
    estadoAtual = novoEstado;
}
void MaquinaEstados::renderizar() {
    estadoAtual->renderizar();
}
void MaquinaEstados::processarEntrada() {
    estadoAtual->processarEntrada();
    
}
void MaquinaEstados::loop() {
    // Exemplo de loop principal da máquina de estados
    estadoAtual->renderizar();
    estadoAtual->processarEntrada();
    // Outras operações de gerenciamento de estados...
    
}
// Outros métodos de gestão de estado...


TelaMenu::TelaMenu(int v1, int v2) : EstadoBase(v1, v2), atualizar(true) {}

void TelaMenu::renderizar() {
     if(atualizar) {
          clearAndPrintHeader("Menu: ");
          lcd.setCursor(0,1);
          switch (opcaoAtual) {
            case 0:
              lcd.print(">Dispositivos");
              break;
            case 1:
              lcd.print(">Cadastrar Disp.");
              break;
            case 2:
              lcd.print(">Deletar Disp");
              break;
            default:
              break;
          }
          atualizar = false;
        }
}

void TelaMenu::processarEntrada() {
    uint8_t botao = processaBotao();
    switch (botao)
    {
    case BOTAO_CIMA:
        opcaoAtual = (opcaoAtual - 1 + opcoes) % opcoes;
        atualizar = true;
        break;
    case BOTAO_BAIXO:
        opcaoAtual = (opcaoAtual + 1) % opcoes;
        atualizar = true;
        break;
    case BOTAO_SELECT:{
        if (opcaoAtual == 0) {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new TelaDispositivos(0,0));
        } else if (opcaoAtual == 1) {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new CadastrarDispositivos(0,0));
        } else if(opcaoAtual == 2) {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new DeletarDispositivos(0,0));
        }   
        
        break;
    }
    case BOTAO_DEFAULT:
        atualizar=false;
        break;
    }
    
}



CadastrarDispositivos::CadastrarDispositivos(int v1, int v2) : EstadoBase(v1, v2), atualizar(true) {
    // Inicialização de membros, se necessário
}

void CadastrarDispositivos::renderizar() {
    if (atualizar) {
        ativarEspNow();
        inicio = millis();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Esperando");
        lcd.setCursor(0, 1);
        lcd.print("Dispositivo");
        atualizar = false;
    }
}

void CadastrarDispositivos::processarEntrada() {
    atual = millis();
    if (recebido) {
        desativarEspNow();
        if (Memoria::enderecoMacExiste(d.macAddress)) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Dispositivo");
            lcd.setCursor(0, 1);
            lcd.print("Ja cadastrado");
            delay(1000);
        } else {
            Memoria::gravaDispositivo(d);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Dispositivo");
            lcd.setCursor(0, 1);
            lcd.print("cadastrado!!");
            delay(1000);
        }
        
        MaquinaEstados *sistema = MaquinaEstados::getInstance();
        sistema->setEstado(new TelaMenu(0, 3));
    }
    if (atual - inicio >= 30000) {
        desativarEspNow();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tempo");
        lcd.setCursor(0, 1);
        lcd.print("Esgotado");
        delay(1000);
        MaquinaEstados *sistema = MaquinaEstados::getInstance();
        sistema->setEstado(new TelaMenu(0, 3));
    }
    // Se necessário, adicione lógica para outras ações ou entradas.
}   

DeletarDispositivos::DeletarDispositivos(int v1, int v2) : EstadoBase(v1, v2), atualizar(true) {}

void DeletarDispositivos::renderizar() {
     if(atualizar) {
          uint8_t qt_dispositivos = Memoria::retornaDispositivos(ds);
          opcoes=qt_dispositivos+1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Deletar:");
          lcd.setCursor(0,1);
          switch (opcaoAtual) {
            case 0:
                lcd.print(">Voltar");
              break;
            case 1:
              lcd.print(">");
              lcd.print(ds[0].nome);
              break;
            case 2:
              lcd.print(">");
              lcd.print(ds[1].nome);
              break;
            case 3:
              lcd.print(">");
              lcd.print(ds[2].nome);
              break;
            case 4:
              lcd.print(">");
              lcd.print(ds[3].nome);
              break;
            case 5:
                lcd.print(">");
                lcd.print(ds[4].nome);
                break;
            default:
              break;
            
          }
          atualizar = false;
        }
}

void DeletarDispositivos::processarEntrada() {
    uint8_t botao = processaBotao();
    switch (botao)
    {
    case BOTAO_CIMA:
        opcaoAtual = (opcaoAtual - 1 + opcoes) % opcoes;
        atualizar = true;
        break;
    case BOTAO_BAIXO:
        opcaoAtual = (opcaoAtual + 1) % opcoes;
        atualizar = true;
        break;
    case BOTAO_SELECT:{
        if (opcaoAtual == 0) {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new TelaMenu(0,3));
        } else {
            uint8_t res = Memoria::deletarDispositivo(ds[opcaoAtual-1].macAddress);
            if (res == DISPOSITIVO_DELETADO) {
                clearAndPrintHeader("Dispositivo");
                lcd.setCursor(0,1);
                lcd.print("Deletado");
                delay(1000);
                opcaoAtual = 0;
                atualizar=true;
            } else {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Erro ao");
                lcd.setCursor(0,1);
                lcd.print("Deletar");
                delay(1000);
                opcaoAtual = 0;
                atualizar=true;
            }
           
        }
        
        break;
    }
    case BOTAO_DEFAULT:
        atualizar=false;
        break;
    }
    
}

TelaDispositivos::TelaDispositivos(int v1, int v2) : EstadoBase(v1, v2), atualizar(true) {}

void TelaDispositivos::renderizar() {
     if(atualizar) {
        limparDispositivos();
        uint8_t qt_dispositivos = Memoria::retornaDispositivos(ds);
        opcoes=qt_dispositivos+1;
        clearAndPrintHeader("Dispositivos: ");
        lcd.setCursor(0,1);
        switch (opcaoAtual) {
        case 0:
            lcd.print(">Voltar");
            break;
        case 1:
            lcd.print(">");
            lcd.print(ds[0].nome);
            break;
        case 2:
            lcd.print(">");
            lcd.print(ds[1].nome);
            break;
        case 3:
            lcd.print(">");
            lcd.print(ds[2].nome);
            break;
        case 4:
            lcd.print(">");
            lcd.print(ds[3].nome);
            break;
        case 5:
            lcd.print(">");
            lcd.print(ds[4].nome);
            break;
        default:
            break;
        
        }
        atualizar = false;
    }
}

void TelaDispositivos::processarEntrada() {
    uint8_t botao = processaBotao();
    switch (botao)
    {
    case BOTAO_CIMA:
        opcaoAtual = (opcaoAtual - 1 + opcoes) % opcoes;
        atualizar = true;
        break;
    case BOTAO_BAIXO:
        opcaoAtual = (opcaoAtual + 1) % opcoes;
        atualizar = true;
        break;
    case BOTAO_SELECT:{
        if (opcaoAtual == 0) {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new TelaMenu(0,3));
        } else {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new TelaFuncoes(0,0,ds[opcaoAtual-1]));
        }
        
        break;
    }
    case BOTAO_DEFAULT:
        atualizar=false;
        break;
    }
    
}

TelaFuncoes::TelaFuncoes(int v1, int v2, Dispositivo d) : EstadoBase(v1, v2), atualizar(true), dispositivo(d) {}

void TelaFuncoes::renderizar() {
     if(atualizar) {
        opcoes = 0;
        for (int i = 0; i < 3; i++) {
        if (dispositivo.funcoes[i][0] != '\0') {
            opcoes+=1;
        }
        }
        opcoes+=1;
       clearAndPrintHeader("Funcoes: ");
        lcd.setCursor(0,1);
        switch (opcaoAtual) {
        case 0:
            lcd.print(">Voltar");
            break;
        case 1:
            lcd.print(">");
            lcd.print(dispositivo.funcoes[0]);
            break;
        case 2:
            lcd.print(">");
            lcd.print(dispositivo.funcoes[1]);
            break;
        case 3:
            lcd.print(">");
            lcd.print(dispositivo.funcoes[2]);
            break;
        default:
            break;
        
        }
        atualizar = false;
    }
}

void TelaFuncoes::processarEntrada() {
    uint8_t botao = processaBotao();
    switch (botao)
    {
    case BOTAO_CIMA:
        opcaoAtual = (opcaoAtual - 1 + opcoes) % opcoes;
        atualizar = true;
        break;
    case BOTAO_BAIXO:
        opcaoAtual = (opcaoAtual + 1) % opcoes;
        atualizar = true;
        break;
    case BOTAO_SELECT:{
        if (opcaoAtual == 0) {
            MaquinaEstados *sistema = MaquinaEstados::getInstance();
            sistema->setEstado(new TelaMenu(0,3));
        } else{
            Serial.print("Funcao chamada: ");
            Serial.println(dispositivo.funcoes[opcaoAtual-1]);
            chamarFuncao(dispositivo.macAddress,dispositivo.funcoes[opcaoAtual-1]);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Funcao");
            lcd.setCursor(0,1);
            lcd.print("chamada!");
            delay(1000);
            atualizar=true;
        }
        break;
    }
    case BOTAO_DEFAULT:
        atualizar=false;
        break;
    }
    
}