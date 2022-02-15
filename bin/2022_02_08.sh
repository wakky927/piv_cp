#!/bin/bash

cd
cd CLionProjects/piv_c/cmake-build-debug

./piv_c ../cfg/lfc/1.ini &
./piv_c ../cfg/lfc/2.ini &
./piv_c ../cfg/lfc/3.ini &
./piv_c ../cfg/lfc/4.ini &
./piv_c ../cfg/lfc/5.ini &
./piv_c ../cfg/lfc/6.ini &
./piv_c ../cfg/lfc/7.ini &
./piv_c ../cfg/lfc/8.ini &

wait

./piv_c ../cfg/lfc/9.ini &
./piv_c ../cfg/lfc/10.ini &
./piv_c ../cfg/lfc/11.ini &
./piv_c ../cfg/lfc/12.ini &
./piv_c ../cfg/lfc/13.ini &
./piv_c ../cfg/lfc/14.ini &
./piv_c ../cfg/lfc/15.ini &
./piv_c ../cfg/lfc/16.ini &

wait

./piv_c ../cfg/lfc/17.ini &
./piv_c ../cfg/lfc/18.ini &
./piv_c ../cfg/lfc/19.ini &
./piv_c ../cfg/lfc/20.ini &
./piv_c ../cfg/lfc/22.ini &
./piv_c ../cfg/lfc/23.ini &
./piv_c ../cfg/lfc/24.ini &
./piv_c ../cfg/lfc/25.ini &

wait

./piv_c ../cfg/lfc/26.ini &
./piv_c ../cfg/lfc/27.ini &
./piv_c ../cfg/lfc/28.ini &
./piv_c ../cfg/lfc/29.ini &
./piv_c ../cfg/lfc/30.ini &
./piv_c ../cfg/lfc/31.ini &
./piv_c ../cfg/lfc/32.ini &
./piv_c ../cfg/lfc/33.ini &

wait

./piv_c ../cfg/lfc/34.ini &
./piv_c ../cfg/lfc/35.ini &
./piv_c ../cfg/lfc/36.ini &
./piv_c ../cfg/lfc/37.ini &
./piv_c ../cfg/lfc/38.ini &
./piv_c ../cfg/lfc/39.ini &
./piv_c ../cfg/lfc/40.ini &
./piv_c ../cfg/lfc/41.ini &

wait

./piv_c ../cfg/lfc/42.ini &
./piv_c ../cfg/lfc/43.ini &
./piv_c ../cfg/lfc/44.ini &
./piv_c ../cfg/lfc/45.ini &
./piv_c ../cfg/lfc/46.ini &

wait

echo "program fin."
