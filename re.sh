#!bin/bash
sudo rmmod hallo
make clean 
make
sudo insmod hallo.ko

echo - > /proc/oper1 
echo 21 > /proc/a
echo 2 > /proc/b
cat /proc/result
