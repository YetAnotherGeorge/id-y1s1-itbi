#!/bin/bash

# if the bin directory does not exist, create it
if [ ! -d "./bin" ]; then
   mkdir ./bin
fi

printf "Compiling mat_print...";
gcc ./main_print.c -o ./bin/mat_print;
printf "done\n"

printf "Compiling mat_test...";
gcc ./main_test.c -o ./bin/mat_test;
printf "done\n"

printf "Compiling mat_sum...";
gcc ./main_sum.c -o ./bin/sum;
printf "done\n"

printf "Compiling mat_prod...";
gcc ./main_prod.c -o ./bin/pro;
printf "done\n"

# TEST: suma matrici ./files/matrix_3x3_1.bmat ./files/matrix_3x3_2.bmat
MAT_3x3_1="./files/matrix_3x3_1.bmat"
MAT_3x3_2="./files/matrix_3x3_2.bmat"
SUM_RESULT="./files/matrix_temp_res_sum_3x3.bmat"
printf "\nADUNARE MATRICI:\n"

./bin/mat_print $MAT_3x3_1
./bin/mat_print $MAT_3x3_2

./bin/sum $MAT_3x3_1 $MAT_3x3_2 $SUM_RESULT

printf "\n>>>>>>>>>>>>>> REZULTATUL ADUNARII:\n"
./bin/mat_print $SUM_RESULT

# TEST: inmultirea matrici ./files/matrix_3x4.bmat ./files/matrix_4x3.bmat
MAT_3x4="./files/matrix_3x4.bmat"
MAT_4x3="./files/matrix_4x3.bmat"
PROD_RESULT="./files/matrix_temp_res_prod_3x3.bmat"
printf "\nINMULTIRE MATRICI:\n"

./bin/mat_print $MAT_3x4
./bin/mat_print $MAT_4x3
./bin/pro $MAT_3x4 $MAT_4x3 $PROD_RESULT
printf "\n>>>>>>>>>>>>>> REZULTATUL INMULTIRII:\n"
./bin/mat_print $PROD_RESULT

echo "Note: IMPLEMENTARILE pentru functiile new, get, set, sum, pro se afla in bin_mat_cerinte.c"
echo "FINAL EXECUTIE"