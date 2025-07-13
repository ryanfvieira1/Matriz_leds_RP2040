# RP2040 — Geração de Animações em Matriz de LEDs 5x5

## Objetivo
Desenvolver duas animações interativas em uma matriz de LEDs 5x5 (WS2812), controladas via botões A e B da placa BitDogLab, utilizando o microcontrolador Raspberry Pi Pico W e o simulador Wokwi. Implementar debounce por software nos botões e, opcionalmente, emitir som com buzzer.

---

## Descrição das Animações

### Animação 1 — Desenho flecha
Animação representando uma flecha, que se forma gradualmente, acionada pelo botão **B** (GPIO 6).

### Animação 2 — explosão
Animação em 5 frames representando uma explosão, acionada pelo botão **A** (GPIO 5).

---

## Especificações Técnicas
- Microcontrolador: Raspberry Pi Pico W
- Matriz de LEDs: 5x5 WS2812
- Botões: GPIO 5 (B) e GPIO 6 (A)
- Linguagem: C
- Ambiente: VS Code + Pico SDK + simulador Wokwi
- Debounce por software implementado
- Código modular com função `desenho_pio` para exibição de frames

---

## Como Executar
1. Clone este repositório:
    ```bash
    git clone https://github.com/ryanfvieira1/Matriz_leds_RP2040.git
    ```

2. Compile o projeto com o **Pico SDK**.
3. Execute a simulação no **Wokwi** ou grave o firmware no Raspberry Pi Pico W.
4. Pressione o botão **B** para executar a animação da flecha.
5. Pressione o botão **A** para executar a animação da explosão.

---

## Organização da Equipe
- **Líder de Projeto:** [RYAN FARIAS] — responsável pelo gerenciamento do repositório e revisão dos pull requests.
- **Desenvolvedor 1:** [JOÃO VICTOR GUIMARÃES] — responsável pela animação da Flecha.
- **Desenvolvedor 2:** [JOÃO VICTOR TEIXEIRA] — responsável pela animação de Explosão.

---

## Demonstração em Vídeo

 [Link para o vídeo]
 https://www.canva.com/design/DAGtFa2kbBE/uXWfOpzzE2DsljQDr_9jNg/edit?utm_content=DAGtFa2kbBE&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton

## Conclusão
A atividade foi concluída com sucesso, implementando as animações propostas, debounce por software, e integração completa com a matriz de LEDs WS2812. O código está versionado no GitHub, e o vídeo de apresentação demonstra o resultado final.

---

## Licença
Este projeto é destinado exclusivamente para fins acadêmicos na disciplina **ELET0021 — Microcontroladores e Microprocessadores**.

---
