set datafile sep ','
set title "File size v.s. Execution time, Graph 2"
set xlabel "File size(bytes)"
set ylabel "Execution time(nanoseconds)"
set key left top
set term svg
set output "../svg/graph2.svg"
plot "../csv/time_results2.csv" using 1:2 with lines title "Buffer size: 512 bytes", "../csv/time_results2.csv" using 1:3 with lines title "Buffer size: 1024 bytes", "../csv/time_results2.csv" using 1:4 with lines title "Buffer size: 2048 bytes", "../csv/time_results2.csv" using 1:5 with lines title "Buffer size: 4096 bytes", "../csv/time_results2.csv" using 1:6 with lines title "Buffer size: 8192 bytes", "../csv/time_results2.csv" using 1:7 with lines title "Buffer size: 16384 bytes"
