# 🚀 Smart Bin

> **Projeto Integrado:** O Smart Bin é uma lixeira inteligente capaz de identificar a própria lotação e gerar avisos para o usuário.

---

## 📌 Sumário
- [Sobre o Projeto](#-sobre-o-projeto)
- [Arquitetura do Sistema](#-arquitetura-do-sistema)
- [Hardware & Componentes](#-hardware--componentes)
- [Estrutura do Repositório](#-estrutura-do-repositório)
- [Como Configurar e Executar](#-como-configurar-e-executar)
  - [Prerequisites](#pré-requisitos)
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
| **Sensor de proximidade (ultrassônico)** | Identifica a distância entre o sensor e o nível do lixo dentro da lixeira, permitindo calcular aproximadamente o percentual de ocupação. |
| **LEDs** | Indicam o nível de preenchimento da lixeira, com cores diferentes para os níveis baixo, médio e alto. |
| **Protoboard** | Realiza as conexões entre os componentes eletrônicos para teste. Caso não seja possível soldar, ela pode ser usada diretamente na montagem final. |
| **Cabos jumper** | Realizam as conexões elétricas entre o ESP32, o sensor e os demais componentes. |
| **Resistores** | Controlam a corrente elétrica dos LEDs e protegem os componentes. |
| **Bateria** | Alimenta o circuito. |
| **Lixeira** | Estrutura física onde são instalados o sensor e os componentes necessários para o funcionamento do sistema. |
| **LCD Keypad Shield** | Funciona como display do projeto, exibindo o percentual de ocupação do lixo. |

---

## 📂 Estrutura do Repositório

```text
smart-bin/
├── docs/                  # Documentação do projeto
├── firmware/              # Código embarcado do ESP32
│   ├── include/
│   │   └── medicao.h      # Declarações das funções de medição
│   └── src/
│       ├── medicao.cpp    # Lógica de leitura do sensor e cálculo da ocupação
│       ├── medicao.h
│       └── src.ino        # Arquivo principal (setup e loop)
├── hardware/              # Arquivos de hardware
│   ├── cad/               # Modelos CAD da estrutura
│   ├── datasheets/        # Datasheets dos componentes
│   └── esquematico.pdf    # Esquemático do circuito
├── software/              # Códigos externos (bibliotecas de terceiros) utilizados no projeto
├── .gitignore
└── README.md
```

| Pasta | Descrição |
|---|---|
| `docs/` | Documentação e materiais de apoio do projeto. |
| `firmware/` | Código embarcado que roda no ESP32, responsável pela leitura do sensor, cálculo do percentual e controle de LEDs e display. |
| `hardware/` | Esquemático, modelos CAD e datasheets dos componentes. |
| `software/` | Códigos externos (de terceiros) utilizados no projeto, não desenvolvidos pela equipe. |
