#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix_SM.h"

//非并行的代码，建议放在测试代码中
//矩阵分块处理：计算分块后子矩阵块的个数
void MatixPros(unsigned short int blkp[6]){
	//分块后子矩阵的个数h*l,A矩阵分为S*T的子矩阵，B矩阵分为T*S的子矩阵
	//unsigned short int表示无符号整数，仅占用2个字节，unsigned int和int均占4个字节
//AC_M
	if (rawm % S == 0) {
		blkp[0] = rawm / S;
	} else {
		blkp[0] = rawm / S + 1;
	}
	blkp[1] = ceil((double) rawn / (double) T);  //AR_N
	blkp[2] = ceil((double) rawp / (double) S);  //BR_P
	//矩阵A、B分块后，不能全分块时，最后一行和最后一列的子矩阵的大小
	blkp[3] = rawm-(blkp[0]-1)*S;   //AC_last
	blkp[4] = rawn-(blkp[1]-1)*T;  //AR_last
	blkp[5] = rawp-(blkp[2]-1)*S;  //BR_last
}

//分块后的子块矩阵乘法
void SMblock_Mult(data_type rawA[rawm][rawn], data_type rawB[rawm][rawn],
		data_type rawD[rawn][rawp], data_type rawCAD[rawm][rawp],
		data_type rawCABO[rawm][rawm], unsigned short int si, unsigned short int sj, unsigned short int sk, unsigned short int subm,
		unsigned short int subn, unsigned short int subp,unsigned short int flag,data_type rawCABot[rawm][rawn]) {
	unsigned short int i, j, k;
	for (i = 0; i < subm; i++) { //行号
		for (j = 0; j < subn; j++) { //列号
			for (k = 0; k < subp; k++) { //并行
				//矩阵乘法C[m][p]=A[m][n]*D[n][p],其中rawA[i]表示rawA是二维数组第i行首元素的地址
				rawCAD[si*S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawD[sj*T + j][sk * S + k];//子矩阵乘法
				//转置矩阵的乘法：C[m][p]=A*B'=A[m][n]*B'[p][n]
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
			}
			//
			if(flag==0){
				rawCABot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //子矩阵的点积
			}
		}
	}
}

//分块后的子块矩阵点积C[m][n]=A[m][n].*B[m][n]，已将该函数写入了SMblock_Mult，避免综合时遇到的内联函数自动转换问题
/*
void SMblock_MultDot(data_type rawA[rawm][rawn],data_type rawB[rawm][rawn],data_type rawCABot[rawm][rawn],int si, int sj, int subm, int subn) {
	unsigned short int i, j;
	for (i = 0; i < subm; i++) { //行号
		for (j = 0; j < subn; j++) { //列号
			rawCABot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //子矩阵的点积
		}
	}
}
*/

//分块矩阵运算：调用乘法实现分块矩阵3种矩阵乘法和矩阵点积运算
void Mult_blk(data_type rawA[rawm][rawn],data_type rawB[rawm][rawn],data_type rawD[rawn][rawp],data_type rawCAD[rawm][rawp],
		data_type rawCABot[rawm][rawn], data_type rawCABO[rawm][rawm],unsigned short int blkb[]) {
	unsigned short int i, j, k;
	for (i = 0; i < blkb[0]; i++) {  //AC_M
		for (j = 0; j < blkb[1]; j++) {  //AR_N
			//频繁使用的寄存器变量
			unsigned short  int mblk = S, nblk = T;
			//计算当前子块的大小
			if (i == blkb[0] - 1) { //AC_M
				mblk = blkb[3]; // AC_last
			}
			if (j == blkb[1] - 1) {//AR_N
				nblk = blkb[4]; //AR_last
			}
			unsigned int flag=0;
			for (k = 0; k < blkb[2]; k++) { //BR_P
				unsigned short  int pblk = S;
				if (k == blkb[2] - 1) {//BR_P
					pblk = blkb[5];  //BR_last
				}
				SMblock_Mult(&rawA[0],&rawB[0],&rawD[0],&rawCAD[0],&rawCABO[0],i, j, k, mblk, nblk, pblk,flag,&rawCABot[0]);
				flag=1;  //仅执行一次
			}
			//SMblock_MultDot(&rawA[0],&rawB[0],&rawCABot[0],i, j, mblk, nblk);
		}
	}
}

//2.1 子矩阵乘法 C=A'*B
void SMblock_MultCAOB(data_type rawA[rawm][rawn],data_type rawB[rawm][rawn],data_type rawCAOB[rawn][rawn],unsigned short int si,unsigned short int sj,unsigned short int sk,unsigned short int subm,unsigned short int subn,unsigned short int subp) {
	unsigned short int i, j, k;
	for (j = 0; j < subn; j++){ //列号
	   for (i = 0; i < subm; i++) { //行号
			for (k = 0; k < subp; k++) { //并行
				rawCAOB[sj * T + j][sk * T + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * T + k];

			}
		}
	}
}
//实现分块矩阵乘法 C=A'*B，区别在于分块调用时循环次数为N*N*M，而不是M*M*N
void Mult_blkCAOB(data_type rawA[rawm][rawn], data_type rawB[rawm][rawn], data_type rawCAOB[rawn][rawn],unsigned short int blkaob[]) {
	unsigned short int i, j, k;
	for (j = 0; j < blkaob[1]; j++) { //AR_N
		for (i = 0; i < blkaob[0]; i++) { //AC_M
				for (k = 0; k < blkaob[1]; k++)  { //AR_N
				unsigned short int mblk=S,nblk=T,pblk=T;
				if (i == blkaob[0] - 1) {  // AC_M
					mblk = blkaob[3];  //AC_last
				}
				if (j == blkaob[1] - 1) {  //AR_N
					nblk = blkaob[4];  //AR_last
				}
				if (k == blkaob[1] - 1) { //AR_N
					pblk = blkaob[4];  //AR_last
				}
				//分块矩阵乘法C=A'*B
				SMblock_MultCAOB(&rawA[0],&rawB[0],&rawCAOB[0],i, j, k, mblk, nblk,pblk);
				}
			}
		}
}
//顶层函数
void Matix_Blkmulti(data_type RA[rawm][rawn], data_type RB[rawm][rawn],
		data_type RD[rawn][rawp], data_type rawCAD[rawm][rawp],
		data_type rawCABot[rawm][rawn], data_type rawCAOB[rawn][rawn],
		data_type rawCABO[rawm][rawm]) {

	//定义一个数组表示分块后子块的下标和子块大小
	unsigned short int mblk[6] = { 1, 1, 1, 1, 1, 1 };
	MatixPros(mblk); //矩阵分块处理

	Mult_blk(&RA[0], &RB[0], &RD[0], &rawCAD[0],&rawCABot[0], &rawCABO[0], mblk);  //分块矩阵运算
	Mult_blkCAOB(&RA[0], &RB[0], &rawCAOB[0], mblk);  //分块矩阵的乘法 C=A'*B
}
