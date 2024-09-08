set terminal pdf
set encoding utf8
set output "2024-09-08-test_to_delete.pdf"
set encoding utf8
#set xrange [0.0:600.0]
#set xrange [0.0:20.0]
set key outside
set style line 1 linecolor rgb '#DB7093' linewidth 0.1
set title "2024-09-08-test_to_delete.lab"
set xlabel "T(s)"
set ylabel "Amplitude"
plot "2024-09-08-test_to_delete.lab" using 1:2 w l linestyle 1 title "test0"
