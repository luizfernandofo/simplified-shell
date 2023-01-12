# Simplified Shell

## Lista de afazeres
- [ ] Comandos internos
    - [x] `ajuda`
    - [ ] `amb`
        - [x] `amb $VAR`
        - [ ] `amb VAR=<valor>`
            - [x] `HOST`
            - [x] `PRONTO`
            - [x] `SHELL`
    - [x] `cd <diretório>`
    - [x] `limpa`
    - [x] `sair`
- [ ] Ler configuração a partir do .meushell.rec
- [x] Executar comandos externos com fork() e exec()
- [ ] Histórico dos comandos executados em .meushell.hst
- [x] Executar comandos a partir de um arquivo de entrada
- [x] Conteúdo Extra
    - regex

<br>

## Fazendo no momento
- Luiz Fernando
    - [ ]
- Pedro Augusto
    - [ ] Comando `amb` (detalhes a serem finalizados)
    - [x] Histórico dos comandos executados em .meushell.hst (conferir)

<br>

## Questões
- DTA deve ser uma variavel de ambiente?
    - Se for o caso, deve ser inserida em env_vars
- Definir um formato para PRONTO
    - `“$DTA \> ”`
    - `“/DTA~$ ”`
    - ...
