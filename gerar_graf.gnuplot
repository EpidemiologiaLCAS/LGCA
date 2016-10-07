# Arquivo de script para a geração do gráfico do AC.
# Para gerar o gráfico, executar o comando gnuplot> load 'gerar_graf.gnuplot' 
unset label
set terminal png truecolor nocrop enhanced font arial 10
set output "graficoIR.png"
set title "LGCA (Fuks/Lawniczack)" font "Arial,16"
set xlabel "Geração"
set ylabel "Indivíduos"
set multiplot
set autoscale
plot "lgca_dados_grafico_IR.txt" using 1:2 title 'Infectados' with lines, \
"lgca_dados_grafico_IR.txt" using 1:3 title 'Removidos' with lines

