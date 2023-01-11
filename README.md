# Simplified Shell

## Lista de afazeres
- [ ] Comandos internos
    - [x] `ajuda`
    - [ ] `amb`
        - [ ] `amb $VAR`
        - [ ] `amb VAR=<valor>`
            - [x] `HOST`
            - [x] `PRONTO`
            - [x] `SHELL`
    - [x] `cd <diretório>`
    - [x] `limpa`
    - [x] `sair`
- [ ] Ler configuração a partir do .meushell.rec
- [ ] Executar comandos externos com fork() e exec()
- [ ] Histórico dos comandos executados em .meushell.hst
- [ ] Executar comandos a partir de um arquivo de entrada
- [ ] Conteúdo Extra

<br>

## Fazendo no momento
- Luiz Fernando
    - [x] Execução de comandos externos
    - [x] comando `cd`
- Pedro Augusto
    - [ ] Comando `amb` (detalhes a serem finalizados)
    - [ ] Histórico dos comandos executados em .meushell.hst
    - [x] shell_setup e shell_exit
    - [x] Criação e realocação de env_vars

<br>

## Questões
- DTA deve ser uma variavel de ambiente?
    - Se for o caso, deve ser inserida em env_vars
- Definir um formato para PRONTO
    - `“$DTA \> ”`
    - `“/DTA~$ ”`
    - ...
