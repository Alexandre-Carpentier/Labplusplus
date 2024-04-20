set terminal pdf
set encoding utf8
set output "2024-04-20-test.pdf"
set encoding utf8
#set xrange [0.0:600.0]
#set xrange [0.0:20.0]
set key outside
set style line 1 linecolor rgb '#DB7093' linewidth 0.1
set style line 2 linecolor rgb '#008000' linewidth 0.1
set style line 3 linecolor rgb '#FFE4B5' linewidth 0.1
set style line 4 linecolor rgb '#F4A460' linewidth 0.1
set style line 5 linecolor rgb '#A0522D' linewidth 0.1
set style line 6 linecolor rgb '#FFB6C1' linewidth 0.1
set style line 7 linecolor rgb '#DA70D6' linewidth 0.1
set style line 8 linecolor rgb '#6A5ACD' linewidth 0.1
set style line 9 linecolor rgb '#4169E1' linewidth 0.1
set title "2024-04-20-test.lab"
set xlabel "T(s)"
set ylabel "Amplitude"
plot "2024-04-20-test.lab" using 1:2 w l linestyle 1 title "Analog0", "2024-04-20-test.lab" using 1:3 w l linestyle 2 title "Analog1", "2024-04-20-test.lab" using 1:4 w l linestyle 3 title "Analog2", "2024-04-20-test.lab" using 1:5 w l linestyle 4 title "Analog3", "2024-04-20-test.lab" using 1:6 w l linestyle 5 title "Analog4", "2024-04-20-test.lab" using 1:7 w l linestyle 6 title "Analog5", "2024-04-20-test.lab" using 1:8 w l linestyle 7 title "Analog6", "2024-04-20-test.lab" using 1:9 w l linestyle 8 title "Analog7", "2024-04-20-test.lab" using 1:10 w l linestyle 9 title "Pace 6000"
