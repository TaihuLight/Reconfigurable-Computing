#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix_SM.h"

//分块后子矩阵的个数h*l,A矩阵分为S*T的子矩阵，B矩阵分为T*S的子矩阵
unsigned int AC_M = 1;
unsigned int AR_N = 1;
unsigned int BR_P = 1;

//矩阵A、B分块后，不能全分块时，最后一行和最后一列的子矩阵的大小
unsigned int AC_last = 1;
unsigned int AR_last = 1;
unsigned int BR_last = 1;

//矩阵分块处理：计算分块后子矩阵块的个数
void MatixPros(){
	if (rawm % S == 0) {
		AC_M = rawm / S;
	} else {
		AC_M = rawm / S + 1;
	}
	AR_N = ceil((double) rawn / (double) T);
	BR_P = ceil((double) rawp / (double) S);
	 AC_last = rawm-(AC_M-1)*S;
	 AR_last = rawn-(AR_N-1)*T;
	 BR_last = rawp-(BR_P-1)*S;
}

//分块后的子块矩阵乘法
void SMblock_Mult(int si,int sj,int sk,int subm,int subn,int subp) {
	unsigned int i, j, k;
	for (i = 0; i < subm; i++) { //行号
		for (j = 0; j < subn; j++) { //列号
			for (k = 0; k < subp; k++) { //并行
				//矩阵乘法C[m][p]=A[m][n]*D[n][p],其中rawA[i]表示rawA是二维数组第i行首元素的地址
				rawC[si*S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawD[sj*T + j][sk * S + k];//子矩阵乘法
				//转置矩阵的乘法：C[m][p]=A*B'=A[m][n]*B'[p][n]
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
			}
		}
	}
}

//分块后的子块矩阵点积C[m][n]=A[m][n].*B[m][n]
void SMblock_MultDot(int si, int sj, int subm, int subn) {
	unsigned int i, j;
	for (i = 0; i < subm; i++) { //行号
		for (j = 0; j < subn; j++) { //列号
			rawCDot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //子矩阵的点积
		}
	}
}

//分块矩阵运算：调用乘法实现分块矩阵3种矩阵乘法和矩阵点积运算
void Mult_blk() {
	unsigned int i, j, k;
	for (i = 0; i < AC_M; i++) {
		for (j = 0; j < AR_N; j++) {
			//频繁使用的寄存器变量
			unsigned int mblk = S, nblk = T;
			//计算当前子块的大小
			if ((i == AC_M - 1)) {
				mblk = AC_last;
			}
			if (j == AR_N - 1) {
				nblk = AR_last;
			}
			for (k = 0; k < BR_P; k++) {
				unsigned int pblk = S;
				if ((k == BR_P - 1)) {
					pblk = BR_last;
				}
				SMblock_Mult(i, j, k, mblk, nblk, pblk);
			}
			SMblock_MultDot(i, j, mblk, nblk);
		}
	}
}

//2.1 子矩阵乘法 C=A'*B
void SMblock_MultCAOB(int si,int sj,int sk,int subm,int subn,int subp) {
	unsigned int i, j, k;
	for (j = 0; j < subn; j++){ //列号
	   for (i = 0; i < subm; i++) { //行号
			for (k = 0; k < subp; k++) { //并行
				rawCAOB[sj * T + j][sk * T + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * T + k];

			}
		}
	}
}
//实现分块矩阵乘法 C=A'*B，区别在于分块调用时循环次数为N*N*M，而不是M*M*N
void Mult_blkCAOB(data_type *A, data_type *B, data_type *C) {
	unsigned int i, j, k;
	for (j = 0; j < AR_N; j++) {
		for (i = 0; i < AC_M; i++) {
				for (k = 0; k < AR_N; k++)  {
				unsigned int mblk=S,nblk=T,pblk=T;
				if ((i == AC_M - 1)) {
					mblk = AC_last;
				}
				if (j == AR_N - 1) {
					nblk = AR_last;
				}
				if (k == AR_N - 1) {
					pblk = AR_last;
				}
				//分块矩阵乘法C=A'*B
				SMblock_MultCAOB(i, j, k, mblk, nblk,pblk);
				}
			}
		}
}
