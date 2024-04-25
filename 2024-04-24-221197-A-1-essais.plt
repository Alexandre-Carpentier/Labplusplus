set terminal pdf
set encoding utf8
set output "2024-04-24-221197-A-1-essais.pdf"
set encoding utf8
#set xrange [0.0:600.0]
#set xrange [0.0:5.0]
set key outside
set style line 1 linecolor rgb '#DB7093' linewidth 0.1
set title "2024-04-24-221197-A-1-essais.lab"
set xlabel "T(s)"
set ylabel "Amplitude"
plot "2024-04-24-221197-A-1-essais.lab" using 1:2 w l linestyle 1 title "legende pif"
