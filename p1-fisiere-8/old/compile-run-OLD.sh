#!/bin/bash
# # PRINT
# echo "Compiling mat_print...";
# gcc ./main_print.c -o mat_print;
# for f in ./files/*.bmat; do 
#    ./mat_print $f; 
# done

# SUM
echo "";
echo "Compiling sum...";
gcc ./main_sum.c -o sum;

echo "Example for sum: ";
echo "";
echo "Matrix 1: ";
./mat_print ./files/matrix_3x3_1.bmat
echo "";
echo "Matrix 2: ";
./mat_print ./files/matrix_3x3_2.bmat
echo "";

./sum ./files/matrix_3x3_1.bmat ./files/matrix_3x3_2.bmat ./files/matrix_result_3x3.bmat
echo "Verifying result: ";
./mat_print ./files/matrix_result_3x3.bmat

# PRODUCT
echo "";
echo "Compiling product...";
gcc ./main_prod.c -o pro;

echo "Example for product: ";
PROD_MAT_1=./files/matrix_3x4.bmat
PROD_MAT_2=./files/matrix_4x3.bmat
PROD_MAT_RESULT=./files/matrix_result_3x3.bmat
echo "";
echo "Matrix 1: ";
./mat_print $PROD_MAT_1
echo "";
echo "Matrix 2: ";
./mat_print $PROD_MAT_2
echo "";
echo "";
./pro $PROD_MAT_1 $PROD_MAT_2 $PROD_MAT_RESULT
echo "Verifying result: ";
./mat_print $PROD_MAT_RESULT