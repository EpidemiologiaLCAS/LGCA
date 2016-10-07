#!/usr/bin/python3

import matplotlib.pyplot as plt

nome_arquivo = "ResultadoMonteCarlo.txt"
arquivo = open(nome_arquivo, 'r')
linhas = [linha.split(' ') for linha in arquivo]
eixo_x = [i for i in range(70)]
s = [float(i[0]) for i in linhas]
i = [float(i[1]) for i in linhas]
r = [float(i[2]) for i in linhas]
plt.plot(eixo_x, s, color="green", label="S", linewidth=2)
plt.plot(eixo_x, i, color="red", label="I", linewidth=2)
plt.plot(eixo_x, r, color="blue", label="R", linewidth=2)
plt.legend(loc="upper right")
plt.show()

