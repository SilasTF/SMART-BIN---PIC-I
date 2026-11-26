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
  - [Configuração Inicial (portal da lixeira)](#-configuração-inicial-portal-da-lixeira)
  - [Painel Web](#-painel-web)
  - [Segurança](#-segurança)
- [Documentação do Circuito](#-documentação-do-circuito)
- [Autores e Agradecimentos](#-autores-e-agradecimentos)
- [Licença](#-licença)

---

## 📖 Sobre o Projeto

Descreva com mais detalhes o contexto do projeto:
- **Objetivo:** O que o sistema automatiza, monitora ou executa.
- **Público-alvo ou Aplicação:** Projeto universitário, protótipo de IoT, automação industrial, etc.
- **Funcionalidades Principais:**
  - [x] Medição da ocupação com sensor ultrassônico, com filtro de leituras discrepantes.
  - [x] Exibição do percentual no display LCD.
  - [x] Configuração inicial pelo celular, sem editar o código (portal servido pelo próprio ESP32).
  - [x] Avisos pelo Telegram, direto da lixeira ou pelo painel.
  - [x] Painel web com histórico, previsão de enchimento, calibração remota e regras de notificação.
  - [x] Modelo de notificações baseado na RDC 222/2018 da Anvisa (opcional).

---

## 🏗 Arquitetura do Sistema

```text
  [ HC-SR04 ]──(GPIO)──► [ ESP32 ]──(I2C)──► [ Display LCD ]
                            │  ▲
        leituras (HTTPS)    │  │  configuração (resposta do envio)
                            ▼  │
                       [ Painel web (Flask + SQLite) ]──► [ Telegram ]
                            ▲
                            │ navegador (celular, tablet, PC)
                         [ Usuário ]
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

A lixeira funciona **sozinha** (display e aviso direto pelo Telegram). O painel é opcional e acrescenta histórico, previsão, regras de notificação e configuração remota.

## 📂 Estrutura do Repositório

```
smart-bin/
├── docs/                                  # Documentação do projeto
├── firmware/                              # Código embarcado do ESP32
│   ├── include/                           # Cabeçalhos adicionais
│   └── src/
│       ├── setup_portal.cpp / .h          # Configuração salva na flash e portal de configuração inicial
│       └── setup_page.h                   # Página HTML do portal
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
- Biblioteca **ArduinoJson** 7.x, de Benoit Blanchon (Gerenciador de Bibliotecas), usada para ler a configuração do painel
- As bibliotecas `WebServer` e `Preferences` também já vêm com o pacote do ESP32
- Para o painel: **Python 3.8+**

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
3. **Não há credenciais no código.** Wi-Fi, bot do Telegram, calibração e endereço do painel são informados no [portal de configuração](#-configuração-inicial-portal-da-lixeira), depois de gravar o firmware.
4. Em `firmware/src/setup_portal.h`, troque a senha da rede de configuração (`PORTAL_SENHA`, mínimo de 8 caracteres). Em `firmware/src/src.ino`, se quiser, ajuste o nível do aviso direto pelo Telegram:

   ```cpp
   #define NIVEL_ALERTA_LIXEIRA 80   // % a partir da qual o aviso é enviado (sem painel)
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


## 📲 Configuração Inicial (portal da lixeira)

Na primeira vez que a lixeira liga, ela abre uma rede Wi-Fi própria para ser configurada pelo celular:

1. O display mostra `SmartBin-Setup` e `192.168.4.1`.
2. Conecte o celular na rede **SmartBin-Setup** (senha definida em `PORTAL_SENHA`). Se o Android avisar que a rede não tem internet, escolha manter a conexão.
3. Abra `http://192.168.4.1` no navegador e preencha:

| Seção | Campos | Obrigatória |
|---|---|---|
| 1. Calibração | Com a lixeira **vazia e a tampa fechada**, toque em *Medir lixeira vazia*. Ajuste a distância cheia se precisar (padrão: 5 cm). | Sim |
| 2. Wi-Fi e Telegram | Rede (2,4 GHz), senha, token do bot e chat ID | Não |
| 3. Painel online | Endereço do painel e chave da API | Não |

4. Ao salvar, os dados vão para a memória flash e a lixeira reinicia. Eles **sobrevivem a regravar o firmware**.

**Reconfigurar:** ligue a lixeira com a mão a menos de 3 cm do sensor para reabrir o portal.

**Comportamento depois de configurada**

| Configuração | O que a lixeira faz |
|---|---|
| Sem painel | Acima de `NIVEL_ALERTA_LIXEIRA` (80%), conecta ao Wi-Fi e envia o aviso direto pelo Telegram (se configurado). |
| Com painel | Envia a leitura ao painel a cada *intervalo de envio* (padrão: 15 min; se falhar, tenta de novo em 1 min), aplica a configuração que ele devolve e **deixa os avisos para o painel**. |

## 🖥 Painel Web

Servidor em **Python (Flask) + SQLite** com uma interface de página única, com tema claro e escuro.

**Funcionalidades**

- Tela de seleção de lixeiras, com um ícone que "enche" conforme a ocupação.
- Visão geral: percentual, previsão de quando enche, bateria, último esvaziamento e gráfico (24 h, 7 ou 30 dias).
- Esvaziamento detectado automaticamente (queda de 30 pontos percentuais ou mais) ou registrado à mão.
- Configurações: nome, Wi-Fi, calibração, intervalo de envio, horário silencioso e regras de notificação.
- Alertas pelo Telegram, enviados pelo servidor.

### Regras de notificação

Cada regra pode ser ativada ou desativada, e é adicionada pela interface escolhendo o tipo:

| Tipo | Parâmetros | Quando avisa |
|---|---|---|
| Capacidade | nível (%) | Quando a ocupação chega ao nível. Se vários níveis forem ultrapassados de uma vez, só o mais alto envia mensagem. |
| Tempo desde o esvaziamento | horas | Quando passam X horas desde o último esvaziamento. |
| Horário do dia | horário, ocupação mínima | Uma vez por dia, no horário (dentro de 1 h), se a ocupação estiver acima do mínimo. |

Cada regra avisa **uma vez por ciclo** e volta a valer quando a lixeira é esvaziada. O horário silencioso vale para todas as regras.

**Modelo RDC 222/2018 (Anvisa).** O botão *Modelo RDC 222/2018* substitui as regras atuais por um atalho para resíduos de serviços de saúde do grupo A. O art. 14 prevê a troca do saco ao atingir **2/3 da capacidade** ou a cada **48 horas**, independentemente do volume. O modelo cria:

| Regra | Padrão |
|---|---|
| Trocar o saco com 2/3 da capacidade (67%) | Ativa |
| Trocar o saco após 48 h sem troca | Ativa |
| Resíduo de fácil putrefação (24 h) | Desativada |

> É apenas um atalho de configuração. Não substitui o PGRSS do serviço, e o percentual só equivale a 2/3 do saco se a calibração "cheia" estiver no limite de enchimento dele.

### Executando o painel

**Com Python**

```bash
cd painel
python3 -m venv .venv
.venv/bin/pip install -r requirements.txt
cp run.sh.example run.sh      # edite o run.sh e preencha as variáveis
./run.sh
```

Acesse `http://localhost:8000`. O usuário fica **em branco** e a senha é `DASHBOARD_PASSWORD`.

### Variáveis de ambiente

| Variável | Função |
|---|---|
| `API_KEY` | Chave usada pela lixeira para enviar leituras. Use um valor longo e aleatório. |
| `DASHBOARD_PASSWORD` | Senha de acesso ao site. Se vazia, o site fica sem senha. |
| `TELEGRAM_TOKEN` e `TELEGRAM_CHAT_ID` | Bot que envia os avisos do painel. Sem eles, as regras não enviam nada. |
| `DB_PATH` | Caminho do banco SQLite (padrão: `lixeira.db`). |
| `PORT` | Porta do servidor (padrão: `8000`). |

O servidor usa o **fuso horário do computador** nas regras de horário.

### Acesso de fora de casa (Tailscale Funnel)

O ESP32 e o navegador precisam alcançar o servidor pela internet. O [Tailscale Funnel](https://tailscale.com/kb/1223/tailscale-funnel) publica o servidor em um endereço HTTPS fixo, sem abrir portas no roteador:

```bash
sudo tailscale funnel --bg 8000
tailscale funnel status          # mostra o endereço público
```

No portal da lixeira, informe esse endereço (com `https://` e **sem barra no fim**) e a mesma `API_KEY`. O computador precisa estar ligado com o servidor rodando. Para encerrar a publicação: `sudo tailscale funnel reset`.

### API

| Método e rota | Uso | Autenticação |
|---|---|---|
| `POST /api/ingest` | A lixeira envia `{bin_id, distance_cm, empty_cm, full_cm}` e recebe a configuração atual | Cabeçalho `X-API-Key` |
| `GET /api/bins` | Lista as lixeiras com o estado atual | Senha do site |
| `GET /api/bins/<id>?hours=24` | Detalhes, histórico e esvaziamentos | Senha do site |
| `PUT /api/bins/<id>/config` | Altera a configuração e as regras | Senha do site |
| `POST /api/bins/<id>/emptied` | Registra um esvaziamento | Senha do site |

A lixeira aplica a configuração devolvida (calibração, intervalo de envio e Wi-Fi) no **próximo envio**. Uma troca de Wi-Fi só é gravada se a lixeira conseguir conectar na nova rede.

### Limitações conhecidas

- Os avisos de capacidade saem no envio em que o nível é ultrapassado, então o atraso máximo é o intervalo de envio.
- Com o painel configurado, a lixeira não envia avisos sozinha: se o servidor estiver fora do ar, nada é notificado.
- O firmware usa HTTPS sem validar o certificado do servidor (`setInsecure()`). O tráfego é criptografado, mas isso não protege contra um servidor falso.
- O servidor usa o servidor de desenvolvimento do Flask, adequado a uso doméstico e acadêmico, não a exposição prolongada na internet.

## 🔒 Segurança

- **Não faça commit de segredos.** O `run.sh`, o `.env` e o banco (`*.db`) devem estar no `.gitignore`, junto com `.venv/` e `__pycache__/`. Versione apenas o `run.sh.example`.
- Use uma `DASHBOARD_PASSWORD` longa e uma `API_KEY` aleatória.
- Se um segredo for enviado ao repositório por engano, **troque-o**: apagar o arquivo em um novo commit não remove o valor do histórico.
- O Wi-Fi, o token do bot e a chave da API ficam gravados na flash do ESP32, e não no código.
- Troque `PORTAL_SENHA` e evite deixar a lixeira em modo de configuração sem necessidade.

### Executando o Software

Com o ESP32 gravado e alimentado, o sistema funciona sozinho:

1. O LCD mostra **"Smart Bin / Inicializando..."** por 2 segundos.
2. A cada ciclo (cerca de 1 s), o firmware faz **10 medições** com o HC-SR04, descarta as discrepantes (mediana + MAD) e calcula a média das válidas.
3. A distância vira porcentagem de ocupação e é exibida no LCD (`Nivel: XX%`).
4. Se o painel não estiver configurado e a ocupação passar de **80%**, o ESP32 conecta ao Wi-Fi, envia o aviso pelo Telegram e desconecta. Com o painel configurado, ele envia as leituras ao servidor, que decide os avisos.
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
