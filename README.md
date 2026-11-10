# 🚀 Smart Bin

> **Projeto Integrado:** O Smart Bin é uma lixeira inteligente capaz de identificar a própria lotação e gerar avisos para o usuário.

---

## 📌 Sumário
- [Sobre o Projeto](#-sobre-o-projeto)
- [Arquitetura do Sistema](#-arquitetura-do-sistema)
- [Hardware & Componentes](#-hardware--componentes)
- [Estrutura do Repositório](#-estrutura-do-repositório)
- [Desenho Esquemático (Wokwi)](#-desenho-esquemático)
- [Como Configurar e Executar](#-como-configurar-e-executar)
  - [Pré-requisitos](#pré-requisitos)
  - [Compilando o Firmware](#compilando-o-firmware)
  - [Executando o Software](#executando-o-software)
- [Documentação do Circuito](#-documentação-do-circuito)
- [Autores e Agradecimentos](#-autores-e-agradecimentos)
- [Licença](#-licença)

---

## 📖 Sobre o Projeto

Descreva com mais detalhes o contexto do projeto:
- **Objetivo:** O que o sistema automatiza, monitora ou executa.
- **Público-alvo ou Aplicação:** Projeto universitário, protótipo de IoT, automação industrial, etc.
- **Funcionalidades Principais:**
  - [x] Leitura de dados de sensores em tempo real.
  - [x] Controle de atuadores (motores, relés, displays).
  - [x] Comunicação via rede / Serial / Bluetooth.
  - [x] Interface de usuário (Dashboard web, app móvel ou CLI).

---

## 🏗 Arquitetura do Sistema

```text
  [ Sensores / Atuadores ] 
             │
             ▼ (GPIO / I2C / SPI)
   [ Microcontrolador / ESP32 ] ──(Wi-Fi / Serial)──► [ Software / Server ]
             │                                                  │
             ▼                                                  ▼
     [ Display / Status ]                              [ Dashboard / Banco ]

```

---

## 🔧 Hardware & Componentes

| Componente | Função no projeto |
|---|---|
| **ESP32** | Responsável pelo controle e processamento das informações recebidas pelos sensores, além de possuir comunicação sem fio. |
| **Sensor de proximidade (HC-SR04 P)** | Identifica a distância entre o sensor e o nível do lixo dentro da lixeira, permitindo calcular aproximadamente o percentual de ocupação. |
| **Protoboard** | Realiza as conexões entre os componentes eletrônicos para teste. Caso não seja possível soldar, ela pode ser usada diretamente na montagem final. |
| **Cabos jumper** | Realizam as conexões elétricas entre o ESP32, o sensor e os demais componentes. |
| **Bateria** | Alimenta o circuito. |
| **Lixeira (Lixeira izy Ou 15L)** | Estrutura física onde são instalados o sensor e os componentes necessários para o funcionamento do sistema. |
| **LCD Keypad Shield I2C** | Funciona como display do projeto, exibindo o percentual de ocupação do lixo. |

---

## 📂 Estrutura do Repositório

```
smart-bin/
├── docs/                                  # Documentação do projeto
├── firmware/                              # Código embarcado do ESP32
│   ├── include/                           # Cabeçalhos adicionais
│   └── src/
│       ├── src.ino                        # Arquivo principal (setup e loop)
│       ├── medicao.cpp / medicao.h        # Leitura do sensor e cálculo da ocupação
│       ├── display_utils.cpp / .h         # Funções de exibição no display LCD
│       └── notification_utils.cpp / .h    # Funções de aviso/notificação ao usuário
├── hardware/                              # Arquivos de hardware
│   ├── Wokwi/
│   │   └── Esquematico medidor de aproximacao/
│   │       ├── diagram.json               # Circuito (peças e ligações)
│   │       ├── sketch.ino                 # Código da simulação
│   │       └── wokwi-project.txt          # Referência do projeto no Wokwi
│   ├── cad/
│   │   └── 3D models/                     # Modelos 3D da estrutura
│   ├── datasheets/                        # Datasheets dos componentes
│   └── esquematico.pdf                    # Esquemático do circuito
├── software/                              # Códigos externos (bibliotecas de terceiros)
├── .gitignore
├── LICENSE                                # Licença MIT
└── README.md
```

| Pasta       | Descrição                                                                                                                      |
|-------------|--------------------------------------------------------------------------------------------------------------------------------|
| `docs/`     | Documentação e materiais de apoio do projeto.                                                                                  |
| `firmware/` | Código embarcado do ESP32: leitura do sensor, cálculo do percentual, controle de LEDs, display e avisos ao usuário.            |
| `hardware/` | Simulação Wokwi, esquemático em PDF, modelos CAD/3D e datasheets dos componentes.                                              |
| `software/` | Códigos externos (de terceiros) utilizados no projeto, não desenvolvidos pela equipe.                                          |


## 🔌 Desenho Esquemático

Simulação do circuito feita no [Wokwi](https://wokwi.com/projects/479130465403137025). O link abre o projeto direto no navegador, sem precisar instalar nada.

![Esquemático do Smart Bin no Wokwi](https://github.com/user-attachments/assets/aa626318-6dd9-4a44-81fe-7d20e1a958a0)

### Ligações

| Componente | Pino do componente | Pino do ESP32 |
|---|---|---|
| Sensor ultrassônico HC-SR04 | TRIG | GPIO 25 |
| Sensor ultrassônico HC-SR04 | ECHO | GPIO 26 |
| Sensor ultrassônico HC-SR04 | VCC | 3V3 |
| Sensor ultrassônico HC-SR04 | GND | GND |
| Display LCD 16x2 (I2C) | SDA | GPIO 21 |
| Display LCD 16x2 (I2C) | SCL | GPIO 22 |
| Display LCD 16x2 (I2C) | VCC | 3V3 |
| Display LCD 16x2 (I2C) | GND | GND |

### Arquivos da simulação

Os arquivos do projeto Wokwi estão em [`hardware/Wokwi/Esquematico medidor de aproximacao/`](https://github.com/SilasTF/SMART-BIN---PIC-I/tree/main/hardware/Wokwi/Esquematico%20medidor%20de%20aproximacao):

## 🛠 Como Configurar e Executar

### Pré-requisitos

**Software**

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x
- Pacote de placas **esp32 by Espressif Systems** (Gerenciador de Placas)
- Biblioteca **LiquidCrystal I2C** (Gerenciador de Bibliotecas)
- As bibliotecas `Wire`, `WiFi`, `WiFiClientSecure` e `HTTPClient` já vêm com o pacote do ESP32
- Driver USB-Serial da placa (CP210x ou CH340, conforme o modelo)

**Hardware**

- ESP32, sensor ultrassônico HC-SR04 e display LCD 16x2 com módulo I2C (endereço `0x27`)
- Montagem conforme a tabela de [Ligações](#ligações)
- Cabo USB com suporte a dados

**Notificações (opcional, mas necessário para o alerta)**

- Rede Wi-Fi 2,4 GHz
- Um bot do Telegram: crie com o [@BotFather](https://t.me/BotFather) para obter o **token**. Envie uma mensagem ao bot e abra `https://api.telegram.org/bot<TOKEN>/getUpdates` para descobrir o **chat id**.

### Compilando o Firmware

1. Clone o repositório:

   ```bash
   git clone https://github.com/SilasTF/SMART-BIN---PIC-I.git
   cd SMART-BIN---PIC-I
   ```

2. Abra `firmware/src/src.ino` na Arduino IDE. Os arquivos `medicao`, `display_utils` e `notification_utils` são carregados automaticamente.
3. Em `firmware/src/notification_utils.h`, preencha suas credenciais:

   ```cpp
   #define WIFI_SSID  "SEU_WIFI"
   #define WIFI_PASS  "SUA_SENHA"
   #define BOT_TOKEN  "SEU_TOKEN"
   #define CHAT_ID    "SEU_CHAT_ID"
   ```

   > ⚠️ **Não faça commit com suas credenciais reais.** O token do bot dá acesso total ao bot.

4. Em `firmware/src/src.ino`, ajuste as medidas da sua lixeira (em cm):

   ```cpp
   #define ALTURA_LIXEIRA_VAZIA 45   // distância do sensor (tampa) até o fundo
   #define ALTURA_LIXEIRA_CHEIA 5    // distância mínima do conteúdo ao sensor
   #define NIVEL_ALERTA_LIXEIRA 80   // % a partir da qual o aviso é enviado
   ```

5. Selecione a placa (**Ferramentas → Placa → esp32 → ESP32 Dev Module**) e a porta.
6. Clique em **Verificar** e depois em **Carregar**.
7. Abra o **Monitor Serial** em `9600` baud para acompanhar as leituras.

| Arquivo                                 | Responsabilidade                                                                    |
|-----------------------------------------|-------------------------------------------------------------------------------------|
| `src.ino`                               | `setup()` e `loop()`: fluxo principal, limites de alerta e pinos                    |
| `medicao.cpp` / `medicao.h`             | Leitura do HC-SR04, filtro de leituras discrepantes e cálculo da porcentagem        |
| `display_utils.cpp` / `display_utils.h` | Mensagens do LCD (inicialização, erro, nível de ocupação)                           |
| `notification_utils.cpp` / `.h`         | Conexão Wi-Fi e envio de mensagem pelo bot do Telegram                              |

### Executando o Software

Com o ESP32 gravado e alimentado, o sistema funciona sozinho:

1. O LCD mostra **"Smart Bin / Inicializando..."** por 2 segundos.
2. A cada ciclo (cerca de 1 s), o firmware faz **10 medições** com o HC-SR04, descarta as discrepantes (mediana + MAD) e calcula a média das válidas.
3. A distância vira porcentagem de ocupação e é exibida no LCD (`Nivel: XX%`).
4. Quando a ocupação passa de **80%**, o ESP32 conecta ao Wi-Fi, envia o aviso pelo Telegram e desconecta.
5. Se houver menos de 3 leituras válidas, o LCD exibe **"Erro na leitura! / Verificar sensor"**.

**Simulação no Wokwi:** o `sketch.ino` da pasta `hardware/Wokwi/` é apenas um "Hello, ESP32!", então ele só valida o circuito. Para simular o sistema, copie os arquivos de `firmware/src/` para o projeto no Wokwi e adicione a biblioteca `LiquidCrystal I2C` no `libraries.txt`. O envio pelo Telegram não funciona em Wokwi sem o plano com gateway Wi-Fi.

---

## 📑 Documentação do Circuito

- **Esquemático:** [`hardware/esquematico.pdf`](hardware/esquematico.pdf)
- **Simulação:** [`hardware/Wokwi/Esquematico medidor de aproximacao/`](hardware/Wokwi/Esquematico%20medidor%20de%20aproximacao/)
- **Modelos 3D:** [`hardware/cad/3D models/`](hardware/cad/3D%20models/): `Compartimento Frontal.stl`, `hc-sr04-ultrasonic-v11.stl` e variações em `Alternatives/` (`FrontCase-Aumentado.stl`, `FrontCase-Aumentado-FurosDiminuidos.stl`)
- **Datasheets:** [`hardware/datasheets/`](hardware/datasheets/)

**Princípio de funcionamento**

O firmware envia um pulso de 10 µs no TRIG (GPIO 25), mede a duração do eco no ECHO (GPIO 26, timeout de 30 ms) e calcula:

```
distância (cm) = duração do eco (µs) × 0,0344 / 2
ocupação (%)   = (altura_vazia − distância) × 100 / (altura_vazia − altura_cheia)   // limitada a 0–100
```

Para reduzir ruído, cada leitura final é a média de até 10 medições, depois de descartar valores a mais de 3 desvios absolutos medianos (MAD) da mediana.

| Situação              | Comportamento                                     |
|-----------------------|---------------------------------------------------|
| Ocupação ≤ 80%        | Exibe o percentual no LCD                         |
| Ocupação > 80%        | Exibe o percentual e envia aviso pelo Telegram    |
| < 3 leituras válidas  | Exibe erro no LCD ("Verificar sensor")            |

---

- `diagram.json`: circuito (peças e ligações)
- `sketch.ino`: código da simulação
- `wokwi-project.txt`: referência do projeto no Wokwi

- ## 👥 Autores e Agradecimentos

| Nome            | 			GitHub                         |
|-----------------|----------------------------------------------------|	       
| Silas           | [@SilasTF](https://github.com/SilasTF) 	       | 
| Pedro Heraclito | [@pedroheraclito](https://github.com/pedroheraclito)|
| Kevin           | [@KevinAMMl](https://github.com/KevinAMMl)  |                                  

Agradecimentos à Vinicius Fernandes Soares Mota pelo apoio no desenvolvimento do nosso projeto na materia Projeto Integrado de Computação I.

## 📄 Licença

Este projeto está licenciado sob a **Licença MIT**. Consulte o arquivo [LICENSE](LICENSE) para mais detalhes.
