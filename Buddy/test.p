set terminal png
set output "speed.png"

set title "SPEED BENCHMARK FOR BALLOC"

set key right center

set xlabel "size of block"

set ylabel "time  in s"

plot "freq.dat" 
