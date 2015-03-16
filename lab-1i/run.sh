#!/bin/bash

make clean && make 

c=0
while [ $c -le 45 ]
do
    qemu-arm -L /usr/arm-linux-gnueabihf ./fibseq <<< $c
    (( c++ ))
done
