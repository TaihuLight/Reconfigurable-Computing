/*
 * matix.h
 * 本实验考虑正好分整分和不能整分的两种情况
 *  Created on: 2016年4月22日
 */

//原矩阵的大小——行列数，为实现可扩展性，应定义四个变量控制下标
#define rawm 4
#define rawn 4
#define rawp 4
//子矩阵的大小为S行*T列
#define S 2  //块矩阵的行
#define T 2  //块矩阵的列

typedef float data_type;//定义为32位浮点数
//函数声明
void R_MultA(data_type *A, data_type *B, data_type *C, int m, int n, int p);
