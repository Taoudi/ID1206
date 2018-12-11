set terminal png
set output "freecompare.png"

set title "SPEED FOR BALLOC AND MALLOC"

set key left top center

set xlabel "Time elapsed"
set ylabel "COST IN NS"


plot "balloc.dat" u 1:3 w linespoints title "Speed for Balloc",\
 "malloc.dat" u 1:3 w linespoints title "Speed for Malloc",\
"ballocopt.dat" u 1:3 w linespoints title "Speed for Balloc with Memory Optimization"
