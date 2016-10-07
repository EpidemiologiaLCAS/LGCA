#!/usr/bin/python

from os import system

def ler_arquivo_ir(nome_arquivo, matriz, quant_ciclos):
	with open(nome_arquivo, "r") as arquivo:
		for linha in arquivo:
			dados = linha.split("\t")
			dados = [i.replace("\n", "") for i in dados]
			ciclo = int(dados[0])
			if ciclo < quant_ciclos:
				q_i = float(dados[1])
				q_r = float(dados[2]) 
				matriz[ciclo][1] += q_i
				matriz[ciclo][2] += q_r
			else:
				return

def ler_arquivo_s(nome_arquivo, matriz, quant_ciclos):
	with open(nome_arquivo, "r") as arquivo:
		for linha in arquivo:
			dados = linha.split("\t")
			dados = [i.replace("\n", "") for i in dados]
			ciclo = int(dados[0])
			if ciclo < quant_ciclos:
				q_s = float(dados[1])
				matriz[ciclo][0] += q_s 

def escrever_arquivo(nome_arquivo, matriz):
	with open(nome_arquivo, "w") as arquivo:
		for i in matriz:
			arquivo.write("{0} {1} {2}\n".format(str(i[0]), str(i[1]), str(i[2])))
				
if __name__ == "__main__":
	quant_mc = 100
	nome_arquivo_ir = "lgca_dados_grafico_IR.txt"
	nome_arquivo_s = "lgca_dados_grafico_S.txt"
	quant_ciclos = 70
	matriz = [[0 for i in range(3)] for j in range(quant_ciclos)]
	for mc in range(quant_mc):
		system("./RODAR_RITA.sh")
		ler_arquivo_ir(nome_arquivo_ir, matriz, quant_ciclos)
		ler_arquivo_s(nome_arquivo_s, matriz, quant_ciclos)
	for i in range(quant_ciclos):
		for j in range(3):
			matriz[i][j] /= quant_mc
	escrever_arquivo("ResultadoMonteCarlo.txt", matriz)

