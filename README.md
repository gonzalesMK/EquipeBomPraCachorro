# EquipeBomPraCachorro
Design of a ball launcher for pets.

### Descrição Alto-Nível: 
Nosso projeto consiste em desenvolver um sistema embarcado com um a dois graus (dependendo do modelo) de liberdade que deverá esperar um sensor presencial ler a presença da bolinha na posição necessária. Alguns instantes depois, um motor DC deverá efetuar uma revolução (ou um pré determinado valor de ângulo em graus) para lançar a bolinha a uma distância dependente do valor do ângulo de lançamento. Após isso, o eixo do motor deverá retornar para a posição neutra e aguardar um novo reposicionamento da bolinha. Esse projeto será desenvolvido com o propósito de obter um conhecimento prático na discplina de Sistemas Embarcados do Curso de Engenharia Mecatrônica.

<p align="center">
  <img width="460" height="300" src=imagem.jpeg>
</p>


### Integrantes
- Allan Caetano Vaz Silva,
- Leonardo Domingues Toresin,
- Murilo Vicentin Giordano,
- Juliano Decico Negri


### Materiais
- 1 motor DC
- Sensor de presença
- Beagle black Bone
- Braço de Alumínio
- Meia esfera de Alumínio
- Base Fixa da Alumínio

### Requisitos Funcionais 
1. O sistema deve reagir a um sinal de um sensor e efetuar o lançamento da bolinha 
2. O sistema deve ter um mecanismo de inserção da bolinha que será lançada
3. O cachorro deve ser capaz de recolocar a bolinha no mecanismo sem necessidade de intervenção 
4. O mecanismo deve ser portátil e não necessitar ligação direta na energia para funcionar

### Requisitos Não-Funcionais 
1. A força da bolinha lançada não deve ser suficiente para machucar o usuário ou o animal 
2. A bateria da máquina deve durar pelo menos 3 horas
3. O dispositivo deve ser capaz de lançar uma bola a cada 5 segundos.
4. Pode jogar a bola com velocidade variável 

### Possíveis Soluções para Requisitos
1. O sensor de presença terá como papel apenas perceber a existencia da bola, que será lançada, a contagem de tempo para o lançamento será feita através do código.
2. O acionamento do lançamento será feito pelo motor DC, o objetivo é lançar a bola dependendo do setup de ângulo colocado no lançador.
3. Planeja-se manufaturar todo o conjunto com alumínio, como colocado nos materiais, as chapas e partes não precisarão ser robustas, já que o conjunto não tem que aguentar esforços significativos
4. O dimensionamento será feito com medidas aproximadas com objetivo de lançar uma bola de tênis, sem foco na possibilidade de diferentes tamanhos de bolas para serem lançadas.

Projeto desenvolvido para a discplina de Sistemas Embarcados (SEM0544, 1o semestre de 2020) do Curso de Engenharia Mecatrônica EESC-USP.



