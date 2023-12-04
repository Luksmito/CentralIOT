#ifndef _INTERFACE_H_
#define _INTERFACE_H_
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <espnow.h>
#include "memoria.h"
extern LiquidCrystal_I2C lcd;


extern int buttonUp;    // Botão para cima
extern int buttonDown;  // Botão para baixo
extern int buttonSelect;// Botão de seleção

void inicializaLcd();

class EstadoBase {
  public:
    uint8_t opcaoAtual;
    uint8_t opcoes;

    EstadoBase(uint8_t v1, uint8_t v2) : opcaoAtual(v1), opcoes(v2) {}
    virtual void renderizar() = 0;
    virtual void processarEntrada() = 0;
    // Outros métodos comuns...
    virtual ~EstadoBase() {}
};


class MaquinaEstados {
private:
    static MaquinaEstados* instance;
    EstadoBase *estadoAtual;
public:
  static MaquinaEstados* getInstance();

  void setEstado(EstadoBase *novoEstado);

  void renderizar();

  void processarEntrada();

  void loop();
  // Outros métodos de gestão de estado...
};

// Classe que herda da interface base para representar a Tela de Menu
class TelaMenu : public EstadoBase {
public:
    bool atualizar;
    TelaMenu(int v1, int v2) : EstadoBase(v1, v2), atualizar(true) {}

    void renderizar() override;
    void processarEntrada() override;
};

class CadastrarDispositivos : public EstadoBase{
  public:
    long long int inicio;
    long long int atual;
    bool atualizar = true;
    CadastrarDispositivos(int v1, int v2);

    void renderizar() override ;

    void processarEntrada() override ;
      
};

class DeletarDispositivos : public EstadoBase{
  public:
    bool atualizar = true;
    DeletarDispositivos(int v1, int v2);

    void renderizar() override ;

    void processarEntrada() override ;
      
};

class TelaDispositivos : public EstadoBase{
  public:
    bool atualizar = true;
    TelaDispositivos(int v1, int v2);

    void renderizar() override ;

    void processarEntrada() override ;
      
};

class TelaFuncoes : public EstadoBase{
  public:
    Dispositivo dispositivo;
    bool atualizar = true;

    TelaFuncoes(int v1, int v2, Dispositivo d);

    void renderizar() override ;

    void processarEntrada() override ;
      
};

#endif