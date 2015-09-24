gcc -o test main.c -mavx -lm -w && ./main

cd benchmark && gnuplot benchmark.dem

cd correctness && gnuplot correctness.dem

