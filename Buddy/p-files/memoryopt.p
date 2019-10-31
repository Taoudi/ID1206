set terminal png
set output "memoryopt.png"

set title "MEMORY ALLOCATION BALLOC"

set key right center

set xlabel "Time in NS"
set ylabel "Size in Bytes"


plot "memoryopt.dat" u 1:2 w linespoints title "Total memory allocated",\
  "memoryopt.dat" u 1:3 w linespoints title "Unused Memory",\
  "memoryopt.dat" u 1:4 w linespoints title "Taken Memory" 
