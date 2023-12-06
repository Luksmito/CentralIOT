#ifndef _INTERFACE_H_
#define _INTERFACE_H_

// Inclusão de bibliotecas necessárias
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <espnow.h>
#include "memoria.h"

// Declaração de variáveis globais externas
/*LiquidCrystal_I2C lcd;
int buttonUp;    // Botão para cima
int buttonDown;  // Botão para baixo
int buttonSelect;// Botão de seleção*/

// Protótipos de funções de inicialização
void inicializaLcd();
void inicializaBotoes();

// Classe base para os diferentes estados da interface
class EstadoBase {
public:
    // Variáveis de estado comuns a todas as telas
    uint8_t opcaoAtual;
    uint8_t opcoes;
    
    // Construtor da classe EstadoBase
    EstadoBase(uint8_t v1, uint8_t v2) : opcaoAtual(v1), opcoes(v2) {}

    // Métodos virtuais puros que devem ser implementados pelas subclasses
    virtual void renderizar() = 0;
    virtual void processarEntrada() = 0;
    // Outros métodos comuns...
    
    // Destrutor virtual para permitir a destruição correta dos objetos derivados
    virtual ~EstadoBase() {}
};

// Gerenciador de máquina de estados
class MaquinaEstados {
private:
    static MaquinaEstados* instance;
    EstadoBase *estadoAtual;

public:
    // Métodos para acessar a instância única e manipular estados
    static MaquinaEstados* getInstance();
    void setEstado(EstadoBase *novoEstado);
    void renderizar();
    void processarEntrada();
    void loop();
    // Outros métodos de gestão de estado...
};

// Classe representando a Tela de Menu, derivada da classe EstadoBase
class TelaMenu : public EstadoBase {
public:
    bool atualizar = true;

    // Construtor da classe TelaMenu
    TelaMenu(int v1, int v2);

    // Implementações dos métodos virtuais da classe base
    void renderizar() override;      
    void processarEntrada() override;
};

// Classes para diferentes telas ou estados específicos
class CadastrarDispositivos : public EstadoBase {
public:
    long long int inicio;
    long long int atual;
    bool atualizar = true;

    CadastrarDispositivos(int v1, int v2);

    void renderizar() override ;
    void processarEntrada() override ;   
};

class DeletarDispositivos : public EstadoBase {
public:
    bool atualizar = true;

    DeletarDispositivos(int v1, int v2);

    void renderizar() override ;
    void processarEntrada() override ;   
};

class TelaDispositivos : public EstadoBase {
public:
    bool atualizar = true;

    TelaDispositivos(int v1, int v2);

    void renderizar() override ;
    void processarEntrada() override ;   
};

class TelaFuncoes : public EstadoBase {
public:
    Dispositivo dispositivo;
    bool atualizar = true;

    TelaFuncoes(int v1, int v2, Dispositivo d);

    void renderizar() override ;
    void processarEntrada() override ;   
};

#endif
