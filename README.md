# 🚦 Sistema de Semáforo Inteligente para Pedestres

Projeto desenvolvido com **Arduino** para simular um sistema de semáforo inteligente com controle de travessia de pedestres.

O sistema utiliza um **sensor ultrassônico HC-SR04** para detectar a aproximação de um pedestre, LEDs para representar os sinais de carros e pedestres, um **display OLED** para exibir o estado do sistema e a contagem regressiva, e um **buzzer** para sinalização sonora.

Quando um pedestre é detectado a uma distância de até **3 cm**, o semáforo dos carros passa para amarelo durante **10 segundos** e, em seguida, fica vermelho durante **30 segundos**, permitindo a travessia.

Após o término da travessia, o semáforo dos carros volta a ficar verde e o sistema aguarda **30 segundos** antes de aceitar uma nova solicitação.

---

## 🎯 Objetivo

Desenvolver um sistema de semáforo inteligente capaz de:

* Detectar a presença de um pedestre;
* Controlar automaticamente o sinal dos carros;
* Controlar o sinal destinado aos pedestres;
* Exibir informações no display OLED;
* Emitir sinais sonoros durante a travessia;
* Controlar os tempos de cada etapa do semáforo;
* Evitar novas solicitações durante o intervalo de segurança.

---

## ⚙️ Funcionamento

O sistema inicia com:

* 🟢 **Carros:** sinal verde;
* 🔴 **Pedestres:** sinal vermelho.

Enquanto nenhum pedestre é detectado, o sinal dos carros permanece verde.

Quando o sensor ultrassônico detecta um objeto a uma distância de **3 cm ou menos**, uma solicitação de travessia é iniciada.

### Sequência da travessia

1. 🚶 Pedestre é detectado;
2. 🟡 Sinal dos carros fica amarelo por **10 segundos**;
3. 🔴 Sinal dos carros fica vermelho;
4. 🟢 Sinal dos pedestres fica verde por **30 segundos**;
5. 🔊 O buzzer realiza sinalização sonora durante a travessia;
6. 🟢 Sinal dos carros volta a ficar verde;
7. ⏱️ O sistema aguarda **30 segundos**;
8. 🔎 Após o intervalo, o sensor volta a aceitar novas solicitações.

---

## 🧠 Fluxo do Sistema

```text
                ┌───────────────┐
                │     INÍCIO    │
                └───────┬───────┘
                        │
                        ▼
             ┌─────────────────────┐
             │ Inicializa sistema  │
             └──────────┬──────────┘
                        │
                        ▼
             ┌─────────────────────┐
             │ Carros: VERDE       │
             │ Pedestres: VERMELHO │
             └──────────┬──────────┘
                        │
                        ▼
              ┌──────────────────┐
              │ Pode fazer       │
              │ novo pedido?     │
              └───────┬──────────┘
                      │
              ┌───────┴───────┐
              │               │
             NÃO             SIM
              │               │
              ▼               ▼
       Mantém sinal       Verifica sensor
          verde                │
                              ▼
                     ┌──────────────────┐
                     │ Distância ≤ 3 cm?│
                     └───────┬──────────┘
                             │
                     ┌───────┴───────┐
                     │               │
                    NÃO             SIM
                     │               │
                     ▼               ▼
              Mantém verde      Sinal AMARELO
                                  por 10s
                                    │
                                    ▼
                              Sinal VERMELHO
                              Pedestre VERDE
                                  por 30s
                                    │
                                    ▼
                              Carros VERDE
                            Pedestre VERMELHO
                                    │
                                    ▼
                              Aguarda 30s
                                    │
                                    ▼
                              Verifica sensor
```

---

## 🔌 Componentes Utilizados

| Componente              | Quantidade | Função                                |
| ----------------------- | ---------: | ------------------------------------- |
| Arduino Uno             |          1 | Controlador do sistema                |
| Protoboard              |          1 | Montagem do circuito                  |
| LED vermelho            |          2 | Sinal vermelho dos carros e pedestres |
| LED amarelo             |          1 | Sinal de atenção dos carros           |
| LED verde               |          2 | Sinal verde dos carros e pedestres    |
| Resistor 220 Ω          |          5 | Proteção dos LEDs                     |
| HC-SR04                 |          1 | Detecção de pedestres                 |
| Display OLED 128x64 I2C |          1 | Exibição das informações              |
| Buzzer                  |          1 | Sinalização sonora                    |
| Fios jumper             |          — | Conexões do circuito                  |

---

## 📌 Mapeamento dos Pinos

### 🚗 Semáforo dos carros

| Componente   | Pino Arduino |
| ------------ | -----------: |
| LED vermelho |           D8 |
| LED amarelo  |           D9 |
| LED verde    |          D10 |

### 🚶 Semáforo dos pedestres

| Componente   | Pino Arduino |
| ------------ | -----------: |
| LED verde    |          D11 |
| LED vermelho |          D12 |

### 📡 Sensor HC-SR04

| HC-SR04 | Arduino |
| ------- | ------: |
| TRIG    |      D5 |
| ECHO    |      D6 |
| VCC     |      5V |
| GND     |     GND |

### 🔊 Buzzer

| Buzzer | Arduino |
| ------ | ------: |
| Sinal  |      D7 |
| GND    |     GND |

### 🖥️ Display OLED I2C

| OLED | Arduino Uno |
| ---- | ----------- |
| VCC  | 5V          |
| GND  | GND         |
| SDA  | SDA / A4    |
| SCL  | SCL / A5    |

---

## ⏱️ Temporização

| Estado                      |     Duração |
| --------------------------- | ----------: |
| 🟢 Carros verde             |  Indefinido |
| 🟡 Carros amarelo           | 10 segundos |
| 🔴 Carros vermelho          | 30 segundos |
| 🟢 Pedestres verde          | 30 segundos |
| ⏱️ Intervalo após travessia | 30 segundos |
| 📡 Distância de acionamento |    Até 3 cm |

---

## 🖥️ Display OLED

O display OLED apresenta informações sobre o estado atual do sistema.

Durante o funcionamento, podem ser exibidas mensagens como:

```text
SISTEMA
INICIADO
```

Durante o sinal verde dos carros:

```text
CARROS
SIGA
```

Durante o sinal amarelo:

```text
ATENCAO
PARE
```

Durante a travessia:

```text
PEDESTRE
SIGA
```

Além disso, durante os períodos de contagem regressiva, o display apresenta o tempo restante.

---

## 🔊 Sinalização Sonora

O buzzer é utilizado durante o período em que o sinal dos carros está vermelho e o sinal dos pedestres está verde.

O som alterna entre ligado e desligado para indicar que a travessia está liberada.

---

## 💻 Tecnologias Utilizadas

* **C/C++**
* **Arduino**
* **Arduino IDE**
* **HC-SR04**
* **OLED I2C**
* **Adafruit GFX**
* **Adafruit SSD1306**

---

## 📚 Bibliotecas

O projeto utiliza as seguintes bibliotecas:

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
```

### Adafruit GFX

Biblioteca utilizada para trabalhar com elementos gráficos e textos no display.

### Adafruit SSD1306

Biblioteca utilizada para controlar o display OLED baseado no controlador SSD1306.

---

## 📂 Estrutura do Projeto

```text
sistema-semaforo-inteligente/
│
├── src/
│   └── sinal_inteligente.ino
│
├── docs/
│   └── circuito.png
│
├── README.md
│
└── .gitignore
```

### `src/`

Contém o código-fonte do projeto Arduino.

### `docs/`

Contém imagens e documentação complementar do projeto, como o diagrama do circuito.

### `README.md`

Documentação principal do projeto.

---

## 🛠️ Como Executar

### 1. Clone o repositório

```bash
git clone URL_DO_REPOSITORIO
```

### 2. Abra o projeto

Abra a pasta do projeto no **Arduino IDE** ou no **VS Code**.

### 3. Instale as bibliotecas

Instale:

* Adafruit GFX Library
* Adafruit SSD1306

A biblioteca `Wire` já faz parte do ambiente Arduino.

### 4. Monte o circuito

Realize as conexões seguindo o mapeamento de pinos apresentado neste README.

### 5. Conecte o Arduino

Conecte o Arduino Uno ao computador através do cabo USB.

### 6. Selecione a placa

No Arduino IDE:

```text
Tools → Board → Arduino Uno
```

### 7. Selecione a porta

Escolha a porta COM correspondente ao Arduino.

### 8. Faça o upload

Compile e envie o código para o Arduino.

---

## 🧪 Simulação

O projeto pode ser desenvolvido e testado utilizando uma simulação do circuito antes da montagem física.

A imagem do circuito pode ser encontrada em:

```text
docs/circuito.png
```

---

## 📸 Circuito

> A imagem abaixo representa a montagem física/simulada do sistema.

![Circuito do Sistema](docs/circuito.png)

---

## 🚀 Possíveis Melhorias

Algumas funcionalidades podem ser adicionadas futuramente:

* Ajuste da distância de detecção;
* Botão físico para solicitação de travessia;
* Display com informações adicionais;
* Diferentes padrões sonoros;
* Sensor com maior alcance;
* Registro das travessias;
* Controle de múltiplos semáforos;
* Integração com outros sensores;
* Sistema de prioridade para situações específicas.

---

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como atividade prática envolvendo conceitos de:

* Programação embarcada;
* Lógica de programação;
* Estruturas condicionais;
* Controle de fluxo;
* Sensores;
* Atuadores;
* Sistemas eletrônicos;
* Comunicação I2C;
* Automação.

---

## 👨‍💻 Autor

**Flavio Pneu**

Estudante de **Ciência da Computação**.

---

## 📄 Licença

Este projeto foi desenvolvido para fins acadêmicos e de aprendizado.
