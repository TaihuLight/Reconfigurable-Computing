/*
 * 矩阵的四种基本运算
 *  Created on: 2016年4月12日
 * 分块矩阵的转置分两次进行：按一般元素矩阵转置（行列依次互换）后，各子块矩阵再转置。
 * 分块矩阵的加法和点积，要求是同型矩阵，且分块方法相同。
 */
#include <stdio.h>
#include <stdlib.h>
#include "matix.h"

//矩阵转置：遍历上/下三角矩阵，并进行行列互换
void Mat_trans(data_type *matx[],int m,int n) {
	data_type tmp = 0;
	int i,j;
	for (i = 0; i < m; i++) { //行号
		for (j = 0; j < n; j++) { //列号
			if (i < j) {//元素互换
				tmp = matx[i][j];
				matx[i][j] = matx[j][i];
				matx[j][i] = tmp;
			}
		}
	}
}

//矩阵并行暴力乘法
//@data_type A[m][n], data_type B[n][p], data_type C[m][p]
void R_Mult1(data_type *A, data_type *B, data_type *C,int m,int n,int p) {
	int i, j, k;
	for (i = 0; i < m; i++) { //m行号
		data_type sum[p];
		for (j = 0; j < n; j++) { //列号
			for (k = 0; k < p; k++) {//并行
				sum[k] += A[i * p + j] * B[j*p+k];
				printf("\t sum[%d]+=A[%d]*B[%d]=%f",k,i * p + j,j*p+k,sum[k]);
			}
			C = sum;
			//C[i*p+k]= sum[k];
			printf("\t t**sum[%d]=%f", k-2,sum[k-2]);
			printf("\t t**sum[%d]=%f", k-1,sum[k-1]);
			printf("\n");
		}
	}
}

//点积 C=A.*B
//@data_type A[m][n], data_type B[m][n], data_type C[m][n]
void MatrixDot(data_type *A, data_type *B, data_type *C, int m, int n) {
	int i, j;
	for (i = 0; i < m; i++) { //m行号
		for (j = 0; j < n; j++) { //并行
			data_type sum=0.0;  //这里一定要显式定义和说明，否则会出现莫名其妙的问题。
			sum = A[i * n + j] * B[i * n + j];
			C[i * n + j]=sum;
		}
	}
}


//转置矩阵乘法3-1：C=A'*B,注意矩阵行号列号的变化
//@data_type A[n][m], data_type B[n][p], data_type C[m][p]
void R_MultA(data_type *A, data_type *B, data_type *C, int m, int n, int p) {
	int i, j, k;
	for (j = 0; j < m; j++) { //矩阵A转置前的列号，即转置后的行号
		for (i = 0; i < n; i++) { //B的列号
			for (k = 0; k < p; k++) { //并行
				//printf("C[%d]=%f",j * p + k,C[j * p + k]);//检测部分数组的初始值不是0.0
				//若不添加此句，部分C的初值不一定为0.
				//if(i==0){
					//C[j * p + k]=0.0;
				//}
				C[j * p + k]+= A[i*m + j] * B[i * p + k];
				//printf("\t C[%d][%d]=A[%d][%d]*B[%d][%d]+=%f*%f=%f \n",j,k,i,j,i,k,A[i*m + j],B[i * p + k],C[j*p+k]);
			}
		}
	}
}

//转置矩阵乘法4：C=A*B'，结果正确 6-12
//@data_type A[m][n], data_type B[p][n], data_type C[m][p]
void R_MultB(data_type *A, data_type *B, data_type *C, int m, int n, int p) {
	int i, j, k;
	for (i = 0; i < m; i++) { //m行号
		for (k = 0; k < p; k++) { //列号
			data_type sum=0.0;
			for (j = 0; j < n; j++) { //并行
				sum += A[i * n + j] * B[k * n + j];
				printf("\t测试 C[%d][%d]+=A[%d]*B[%d]=%f*%f=%f",i,k,i*n+j,k*n+j,A[i * n + j] , B[k * n + j],sum);
			}
			C[i * p + k]=sum;
			printf("\n");
		}
	}
}
