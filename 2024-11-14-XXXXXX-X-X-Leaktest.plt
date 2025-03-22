set terminal pdf
set encoding utf8
set output "2024-11-14-XXXXXX-X-X-Leaktest.pdf"
set encoding utf8
#set xrange [0.0:600.0]
#set yrange [0.0:20.0]
set key outside
set style line 1 linecolor rgb '#DB7093' linewidth 0.1
set style line 2 linecolor rgb '#008000' linewidth 0.1
set style line 3 linecolor rgb '#FFE4B5' linewidth 0.1
set style line 4 linecolor rgb '#F4A460' linewidth 0.1
set style line 5 linecolor rgb '#A0522D' linewidth 0.1
set title "2024-11-14-XXXXXX-X-X-Leaktest.lab"
set xlabel "T(s)"
set ylabel "Amplitude"
plot "2024-11-14-XXXXXX-X-X-Leaktest.lab" every ::1 using 1:2 w l linestyle 1 title "mac", "2024-11-14-XXXXXX-X-X-Leaktest.lab" using 1:3 w l linestyle 2 title "T1", "2024-11-14-XXXXXX-X-X-Leaktest.lab" using 1:4 w l linestyle 3 title "T2", "2024-11-14-XXXXXX-X-X-Leaktest.lab" using 1:5 w l linestyle 4 title "Digital0", "2024-11-14-XXXXXX-X-X-Leaktest.lab" using 1:6 w l linestyle 5 title "Digital1"
