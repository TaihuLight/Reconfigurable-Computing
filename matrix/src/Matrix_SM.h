/*
 * Matrix_SM.h
 */

#ifndef MATRIX_SM_H_
#define MATRIX_SM_H_

typedef float data_type;//定义为16位浮点数

//原矩阵的大小——行列数
#define rawm 128
#define rawn 256
#define rawp 128
//子矩阵的大小为S行*T列
#define S 4  //块矩阵的行
#define T 6  //块矩阵的列

data_type rawA[rawm][rawn];
data_type rawB[rawm][rawn];
data_type rawD[rawn][rawp];
data_type rawC[rawm][rawp]; //矩阵乘法：C=A*D，二维数组所有元素初始化为0的形式为{{0.0}}
data_type rawCDot[rawm][rawn]; //矩阵点积：CDot=A.*B
data_type rawCAOB[rawn][rawn]; //转置矩阵乘法：CAOB=A’*B
data_type rawCABO[rawm][rawm]; //转置矩阵乘法：CABO=A*B’
//函数声明
void MatixPros();//矩阵分块预处理函数
void Mult_blk();//分块矩阵运算
void MatixPrin(data_type * R, int m, int n);//矩阵打印输出函数
void Mult_blkCAOB(data_type *A, data_type *B, data_type *C) ;
#endif /* MATRIX_SM_H_ */
