set datafile sep ','
set title "File size v.s. Execution time, Graph 1"
set xlabel "File size(bytes)"
set ylabel "Execution time(nanoseconds)"
set key left top
set term svg
set output "../svg/graph1.svg"
plot "../csv/time_results1.csv" using 1:2 with lines title "Buffer size: no buffer", "../csv/time_results1.csv" using 1:3 with lines title "Buffer size: 2 bytes", "../csv/time_results1.csv" using 1:4 with lines title "Buffer size: 4 bytes", "../csv/time_results1.csv" using 1:5 with lines title "Buffer size: 8 bytes", "../csv/time_results1.csv" using 1:6 with lines title "Buffer size: 16 bytes", "../csv/time_results1.csv" using 1:7 with lines title "Buffer size: 32 bytes", "../csv/time_results1.csv" using 1:8 with lines title "Buffer size: 64 bytes", "../csv/time_results1.csv" using 1:9 with lines title "Buffer size: 128 bytes", "../csv/time_results1.csv" using 1:10 with lines title "Buffer size: 256 bytes", "../csv/time_results1.csv" using 1:11 with lines title "Buffer size: 512 bytes"
