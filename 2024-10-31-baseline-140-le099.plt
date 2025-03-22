set terminal pdf
set encoding utf8
set output "2024-10-31-baseline-140-le099.pdf"
set encoding utf8
#set xrange [0.0:600.0]
#set yrange [0.0:20.0]
set key outside
set style line 1 linecolor rgb '#DB7093' linewidth 0.1
set style line 2 linecolor rgb '#008000' linewidth 0.1
set style line 3 linecolor rgb '#FFE4B5' linewidth 0.1
set title "2024-10-31-baseline-140-le099.lab"
set xlabel "T(s)"
set ylabel "Amplitude"
plot "2024-10-31-baseline-140-le099.lab" every ::1 using 1:2 w l linestyle 1 title "10sccmle140", "2024-10-31-baseline-140-le099.lab" using 1:3 w l linestyle 2 title "10nccmle099", "2024-10-31-baseline-140-le099.lab" using 1:4 w l linestyle 3 title "PACE6000"
