

typedef struct dispositivo {
  char nome[10];  // Array de caracteres para um nome de até 9 caracteres + 1 para o caractere nulo '\0'
  char funcoes[3][10] = { "", "", "" };  // Array bidimensional de caracteres para 3 strings de até 9 caracteres + 1 para o caractere nulo
  uint8_t macAddress[6];
} Dispositivo;
