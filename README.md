# 🌊 CaioX — Monitoramento e Simulação do Aquecimento Oceânico

> Projeto de iniciação científica desenvolvido no **IFSP – Campus Capivari**, com foco em monitorização térmica de água via ESP8266.

---

## 👥 Autores

| Nome | Instituição |
|------|-------------|
| Fernando Jesus | IFSP – Campus Capivari |
| Gabriel Machado | IFSP – Campus Capivari |
| Augusto Damaceno | IFSP – Campus Capivari |
| Eduardo Gritti | IFSP – Campus Capivari |
| Matheus Caravita | IFSP – Campus Capivari |

**Orientador:** Prof. Caio Hamamura  
**Ano:** 2025

---

## 📖 Sumário

1. [Introdução](#introdução)
2. [Fundamentação Teórica](#fundamentação-teórica)
3. [O Projeto CaioX](#o-projeto-caiox)
4. [Arquitetura do Sistema](#arquitetura-do-sistema)
5. [Estrutura do Projeto](#estrutura-do-projeto)
6. [Como Usar](#como-usar)
7. [Desenvolvimento e Desafios](#desenvolvimento-e-desafios)
8. [Conclusão](#conclusão)
9. [Agradecimentos](#agradecimentos)
10. [Referências Bibliográficas](#referências-bibliográficas)

---

## Introdução

O aquecimento dos oceanos é uma das consequências mais críticas e menos visíveis das alterações climáticas antropogênicas. A queima de combustíveis fósseis, o desmatamento e as atividades agropecuárias elevaram a concentração de gases de efeito estufa na atmosfera, resultando na absorção de cerca de **90% do calor excedente** do sistema climático pelos mares.

Esta elevação de temperatura altera processos físicos e biológicos fundamentais. O **Projeto CaioX** visa monitorizar este aumento de temperatura através de um sistema tecnológico dedicado, com visualização em tempo real e simulação controlada.

---

## Fundamentação Teórica

### 2.1 Regulação Térmica (Clima)

O clima é um sistema interconectado composto por ar, oceanos, gelo, solo e vegetação. Os principais mecanismos de regulação são:

- **Efeito Estufa Natural:** Mantém a temperatura média global em 14°C; sem ele, seria de aproximadamente -27,5°C.
- **Regulação Oceânica de Calor:** Os oceanos redistribuem calor e vapor de água por todo o planeta através de correntes oceânicas.
- **Forçantes Climáticas Naturais:** Alterações na atividade solar, órbita terrestre ou placas tectônicas que mantêm o sistema em equilíbrio.

### 2.2 Regulação do Carbono e Sistema Carbonato

O oceano é o principal reservatório de CO₂ da Terra. A absorção ocorre por dois mecanismos:

1. **Bomba Física:** Relacionada com a solubilidade do CO₂ na água do mar.
2. **Bomba Biológica:** Assimilação do gás durante a fotossíntese marinha, transportando o carbono para o oceano profundo.

**Sistema Carbonato (Efeito Tampão):** Regula o pH da água do mar (entre 7,8 e 8,3). O excesso de CO₂ atmosférico enfraquece este efeito, levando à **acidificação dos oceanos**, o que prejudica corais e outros organismos marinhos.

---

## O Projeto CaioX

O projeto consiste num sistema de monitorização com controle de aquecimento de água utilizando um microcontrolador **ESP8266 NodeMCU**.

### 3.1 Funcionalidades e Operação

| Funcionalidade | Descrição |
|----------------|-----------|
| 🌡️ **Monitorização** | Mede temperatura em 4 pontos via sensores DS18B20 |
| 📊 **Visualização** | Gráfico interativo em tempo real com Plotly.js via Wi-Fi |
| 🔥 **Simulação** | Relé ativa resistência elétrica simulando aquecimento gradativo |

### 3.2 Especificações Técnicas

| Componente | Especificação |
|------------|---------------|
| **Microcontrolador** | ESP8266 NodeMCU |
| **Modo de rede** | Access Point — SSID: `feira_ciencias` |
| **Sensores** | DS18B20 (protocolo OneWire) |
| **Atuador** | Módulo relé com isolamento, lógica ACTIVE LOW |
| **Interface** | Servidor web embarcado no próprio módulo |
| **Biblioteca de gráficos** | Plotly.js v3.1.0 |

### 3.3 Endpoints da API Embarcada

| Endpoint | Método | Descrição |
|----------|--------|-----------|
| `/` | GET | Página principal com dashboard |
| `/dados` | GET | Retorna leituras de temperatura em JSON |
| `/relay/on` | GET | Liga a resistência de aquecimento |
| `/relay/off` | GET | Desliga a resistência de aquecimento |

---

## Arquitetura do Sistema

```
┌─────────────────────────────────────────────┐
│              ESP8266 NodeMCU                │
│                                             │
│  ┌──────────┐    ┌───────────────────────┐  │
│  │ DS18B20  │───▶│   Leitura OneWire     │  │
│  │ (x4)     │    │   (4 sensores)        │  │
│  └──────────┘    └──────────┬────────────┘  │
│                             │               │
│  ┌──────────┐    ┌──────────▼────────────┐  │
│  │  Relé    │◀───│   Servidor Web HTTP   │  │
│  │ + Resist.│    │   /dados /relay/on    │  │
│  └──────────┘    └──────────┬────────────┘  │
│                             │               │
│                    Access Point Wi-Fi        │
│                   "feira_ciencias"           │
└─────────────────────────────────────────────┘
                              │
                    ┌─────────▼──────────┐
                    │  Navegador (Wi-Fi) │
                    │  Dashboard Plotly  │
                    └────────────────────┘
```

---

## Estrutura do Projeto

```
Caiox01/
├── platformio.ini          # Configuração da plataforma PlatformIO
├── src/
│   └── main.cpp            # Firmware principal do ESP8266
├── data/
│   ├── index.html          # Interface web (dashboard)
│   ├── esti.css            # Estilos da interface
│   ├── plotly-3.1.0.min.js # Biblioteca de gráficos (offline)
│   └── img/
│       └── bob_image.webp  # Imagem do projeto
├── include/
│   └── README
├── lib/
│   └── README
└── test/
    └── README
```

---

## Como Usar

### Pré-requisitos

- [PlatformIO IDE](https://platformio.org/) (VS Code + extensão)
- ESP8266 NodeMCU
- 4x Sensores DS18B20
- Módulo relé
- Resistência elétrica ("rabo quente")

### Upload do Firmware

```bash
# Compilar e enviar o firmware
pio run --target upload

# Enviar os arquivos da interface web para o SPIFFS
pio run --target uploadfs
```

### Acesso ao Dashboard

1. Conecte-se à rede Wi-Fi **`feira_ciencias`**
2. Acesse no navegador: **`http://192.168.4.1`**
3. O gráfico atualiza as leituras de temperatura em tempo real

---

## Desenvolvimento e Desafios

Durante a execução, foram enfrentadas e resolvidas as seguintes dificuldades:

| Problema | Causa | Solução |
|----------|-------|---------|
| **Instabilidade na comunicação** | Ausência de resistor de pull-up | Verificação e adição de resistor 4.7kΩ no barramento OneWire |
| **Leituras incorretas (-127°C)** | Falha de inicialização dos sensores | Tratamento de erro com validação de leitura |
| **Interferência elétrica** | Ruído do relé no circuito | Módulo relé com isolamento óptico e lógica ACTIVE LOW |

---

## Conclusão

O sistema CaioX demonstra ser eficaz para a monitorização térmica em escala educacional. Embora não resolva diretamente o problema das alterações climáticas, atua como uma **ferramenta essencial de observação e simulação**, permitindo:

- Análise de dados de temperatura em tempo real
- Simulação controlada do aquecimento de água
- Conscientização sobre o impacto do aquecimento oceânico

O projeto abre caminho para desenvolvimentos futuros, como integração com plataformas IoT em nuvem, histórico de dados e alertas automáticos.

---

## Agradecimentos

Agradecemos ao **Prof. Caio Hamamura** pela orientação e apoio durante o desenvolvimento do projeto, e ao **Instituto Federal de São Paulo – Campus Capivari** pelo suporte técnico e educacional.

---

## Referências Bibliográficas

JACOBI, Pedro Roberto et al. (org.). **Temas atuais em mudanças climáticas para os ensinos fundamental e médio.** São Paulo: IEE – USP, 2015. 112 p.

MONTEIRO, Bárbara Segato. **Acidificação oceânica: impactos e mecanismos de mudança.** 2021. Trabalho de Conclusão de Curso (Pós-graduação Lato Sensu em Análise Ambiental e Desenvolvimento Sustentável) – Instituto CEUB de Pesquisa e Desenvolvimento, Centro Universitário de Brasília, Brasília, 2021.

OLIVEIRA, Raquel Renó de. **Impactos das mudanças climáticas no Oceano Atlântico Equatorial e Sul: dos parâmetros biogeoquímicos à sensibilidade dos ecossistemas costeiros.** 2025. 138 f. Tese (Doutorado em Oceanografia) – Faculdade de Oceanografia, Universidade do Estado do Rio de Janeiro, Rio de Janeiro, 2025.

OLIVEIRA, Raquel R. et al. **Fonte ou sumidouro? Uma revisão sobre os fluxos de CO₂ na Plataforma Continental Brasileira.** Química Nova, v. 46, n. 6, p. 561–572, 2023. Disponível em: https://doi.org/10.21577/0100-4042.20170970. Acesso em: 23 out. 2023.

---

<div align="center">
  <sub>Desenvolvido com ❤️ no IFSP – Campus Capivari · 2025</sub>
</div>
