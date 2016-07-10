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
#define S 8  //块矩阵的行
#define T 12  //块矩阵的列

#endif /* MATRIX_SM_H_ */
