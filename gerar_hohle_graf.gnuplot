# Arquivo de script para a geração do gráfico do AC.
# Para gerar o gráfico, executar o comando gnuplot> load 'gerar_graf.gnuplot' 
unset label
set terminal png truecolor nocrop enhanced font arial 10
set output "graficoHohle.png"
set title "AC (Höhle)" font "Arial,16"
set xlabel "Geração"
set ylabel "Indivíduos"
set multiplot
set autoscale
plot "dados_grafico_hohle.txt" using 1:2 title 'Infectados' with lines, \
"dados_grafico_hohle.txt" using 1:3 title 'Suscetíveis' with lines, \
"dados_grafico_hohle.txt" using 1:4 title 'Removidos' with lines

