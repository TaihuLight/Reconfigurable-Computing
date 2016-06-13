/*
 * 根据矩阵元素的下标进行分块乘法
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matix.h"

//void R_MultA(data_type *A, data_type *B, data_type *C, int m, int n, int p);
//void R_MultB(data_type *A, data_type *B, data_type *C, int m, int n, int p);
//将矩阵定义为全局常量和变量
//data_type rawA[rawm][rawn] = {4,2,3,3,2,1,4,2};
//data_type rawB[rawp][rawn] = {3,2,3,3,2,1,1,4};

//data_type rawA[rawm][rawn] = {3,1,4,5,2,3};
//data_type rawB[rawp][rawn] = {2,4,3,3,1,2};
/*方阵*/
data_type rawA[rawm][rawn] = { 3, 2, 2, 3, 3, 1, 3, 3, 4, 4, 3, 3, 2, 3, 3, 4 };
data_type rawB[rawp][rawn] = { 1, 2, 3, 1, 1, 2, 4, 1, 1, 2, 4, 3, 1, 4, 4, 1 };
//data_type rawA[rawm][rawn] = { 3, 2, 2, 3, 3, 1, 3, 3, 4, 4, 3, 3, 2, 3, 3 };
//data_type rawB[rawn][rawp] = { 1, 2, 3, 1, 1, 2, 4, 1, 1, 2, 4, 3, 1, 4, 4 };
//将数组元素初始化为全0,防止出现莫名其妙的运算结果。
//二维数组初始化为0的形式为{{0.0}}，一维数组初始化为0的形式为{0.0}
data_type rawC[rawn][rawp]={{0.0}}; //乘法：C=A*B
data_type rawCDot[rawn][rawp]={{0.0}}; //点积：C=A.*B
data_type rawCAOB[rawn][rawn]={{0.0}}; //乘法：C=A’*B
data_type rawCABO[rawm][rawp]={{0.0}}; //乘法：C=A*B’

//函数声明，否则在函数定义之前调用会出现警告信息
void MatixPrin(data_type *A,int m,int n);

//分块后子矩阵的个数h*l,A矩阵分为S*T的子矩阵，B矩阵分为T*S的子矩阵
static int AC_M = 1;
static int AR_N = 1;
static int BR_P = 1;

//矩阵A、B分块后，不能全分块时，最后一行和最后一列的子矩阵的大小
static int AC_last = 1;
static int AR_last = 1;
static int BR_last = 1;

//矩阵分块处理：计算分块后子矩阵块的个数
void MatixPros(){
	//将矩阵rawA[rawm][rawn]分为AC_M*AR_N个大小为S*T的子块，ceil(x)函数返回不小于x的最小整数
	if (rawm % S == 0) {
		AC_M = rawm / S;
	} else {
		AC_M = rawm / S + 1;
	}
	//AC_M = ceil((double) rawm / (double) S); //矩阵A分块后的行数
	//使用ceil函数需要包含头文件math.h
	AR_N = ceil((double) rawn / (double) T); //矩阵A分块后的列数，即矩阵B分块后的行数
	//将矩阵rawB[rawn][rawp]分为AR_N*BR_P个大小为T*S的子块
	BR_P = ceil((double) rawp / (double) S); //矩阵B分块后的列数
	//矩阵A、B分块后，不能全分块时，最后一行和最后一列的子矩阵大小
	 AC_last = rawm-(AC_M-1)*S;
	 AR_last = rawn-(AR_N-1)*T;
	 BR_last = rawp-(BR_P-1)*S;
}

//1.1 考虑分块后的子块矩阵乘法
//@ si,sj,sk表示当前子矩阵第一行首元素的行下标和列下标(子矩阵的编号，如A11,A12)
//@ 当前子矩阵的大小subm,subn,subp分别对应当前子块矩阵的行数和列数，这里默认是所有子矩阵均是S*T或T*S的大小，不存在不规则的子矩阵
void SMblock_Mult(int si,int sj,int sk,int subm,int subn,int subp) {
	//计算各子块矩阵第一个元素与原矩阵对应元素间的位置关系，即子块下标转换为原矩阵元素下标表示
	int i, j, k;
	for (i = 0; i < subm; i++) { //行号
		for (j = 0; j < subn; j++) { //列号
			for (k = 0; k < subp; k++) { //并行
				//rawA[i]表示rawA是二维数组第i行首元素的地址
				rawC[si*S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sj*T + j][sk * S + k];//子矩阵乘法C=A*B
				//if(k==0){//与k的循环无关，只k==0时执行一次即可
				//	rawCDot[si*S + i][sj * T + j] = rawA[si*S + i][sj * T + j]* rawB[si*S + i][sj * T + j];//子矩阵的点积
				//}
				//转置矩阵的乘法：C=A'*B
				//rawCAOB[sj * T + j][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * S + k];
				//转置矩阵的乘法：C=A*B'
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
				//printf("  乘积值C=%f",rawC[si*S + i][sk * S + k]);
				//printf("\t C[%d][%d]+=A[%d][%d]*B[%d][%d]+=    ", si*S + i, sk * S +k ,si*S + i, sj* T + j, sj*T + j, sk * S + k);
			}
			//printf("\n");
		}
	}
}

//1.2 考虑分块后的子块矩阵点积
//@ si,sj,sk表示当前子矩阵第一行首元素的行下标和列下标(子矩阵的编号，如A11,A12)
//@ 当前子矩阵的大小subm,subn,subp分别对应当前子块矩阵的行数和列数，这里默认是所有子矩阵均是S*T或T*S的大小，不存在不规则的子矩阵
void SMblock_MultDot(int si, int sj, int subm, int subn) {
	//计算各子块矩阵第一个元素与原矩阵对应元素间的位置关系，即子块下标转换为原矩阵元素下标表示
	int i, j;
	for (i = 0; i < subm; i++) { //行号
		for (j = 0; j < subn; j++) { //列号
			rawCDot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //子矩阵的点积
		}
	}
}

//1.2 分块矩阵运算：调用乘法实现分块矩阵乘法、点积
void Mult_blk(data_type *A, data_type *B, data_type *C) {
	int i, j, k;
	int count = 0; //循环计数
	for (i = 0; i < AC_M; i++) {
		for (j = 0; j < AR_N; j++) {
			////////////////////////////////
			int mblk = S, nblk = T, pblk = S; //默认当前参与运算的两个子矩阵块的大小，必须每次循环重新赋初值
			//计算当前子块的大小
			if ((i == AC_M - 1)) {
				mblk = AC_last;
			}
			if (j == AR_N - 1) {
				nblk = AR_last;
			}
			///////////////////////////////
			for (k = 0; k < BR_P; k++) {
				printf("\t 第%d层循环：  ", ++count);
				//（1）调用分块的矩阵乘法运算函数
				//当AB矩阵均可以按照子块整分时
				//SMblock_Mult(i,j,k,S,T,S);
				//当AB矩阵存在不能整分的子块时
				//printf("\t 测试：AC_M=%d,AR_N=%d,BR_P=%d \n", AC_M, AR_N, BR_P);
				pblk = S;
				if ((k == BR_P - 1)) {
					pblk = BR_last;
				}
				//分块矩阵乘法C=A*B
				//printf(" 分块乘法：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",i,k,i,j,j,k);
				SMblock_Mult(i, j, k, mblk, nblk, pblk);
			}
			//分块矩阵的点积运算
			SMblock_MultDot(i, j, mblk, nblk);
			//printf(" 分块点积：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n", i, j, i, j, i,j);
		}
	}
	printf("\t 输出C=A*B的结果 \n");
	MatixPrin(*rawC,rawm, rawp);  //因为rawC是二维数组，所以地址引用的方法是 rawC[0]
	printf("\t 输出C=A.*B的结果 \n");
	MatixPrin(*rawCDot,rawm, rawn);
	printf("\t 输出C=A'*B的结果 \n");
	MatixPrin(*rawCAOB,rawn,rawn);
	printf("\t 输出C=A*B'的结果 \n");
	MatixPrin(*rawCABO,rawm,rawp);
}

//3.1 子矩阵乘法 C[m][p]=A*B'=A[m][n]*B'[p][n]
void SMblock_MultCABO(int si,int sj,int sk,int subm,int subn,int subp) {
	int i, j, k;
		for (i = 0; i < subm; i++){ //列号
			//data_type sum=0.0;
			for (k = 0; k < subp; k++) { //并行
				for (j = 0; j < subn; j++) { //行号
				//C[i * p + k]+= A[i * n + j] * B[k * n + j]; //参考
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
			}
			//rawCAOB[si*S + i][sk * S + k]=sum;
		}
	}
}
//3.2 分块矩阵运算：调用乘法实现分块矩阵乘法 C=A*B'
void Mult_blkCABO(data_type *A, data_type *B, data_type *C) {
		int i, j, k;
		int count=0;//循环计数
	for (i = 0; i < AC_M; i++) {
		for (k = 0; k < BR_P; k++) {
			for (j = 0; j < AR_N; j++) {
				printf("\t 第%d层循环：  ",++count);
				printf(" 分块乘法：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",i,k,i,j,k,j);
				int mblk=S,nblk=T,pblk=S;//默认当前参与运算的两个子矩阵块的大小，必须每次循环重新赋初值
				//计算当前子块的大小
				if ((i == AC_M - 1)) {
					mblk = AC_last;
				}
				if (j == AR_N - 1) {
					nblk = AR_last;
				}
				if ((k == BR_P - 1)) {
					pblk = BR_last;
				}
				//分块矩阵乘法C=A*B'
				SMblock_MultCABO(i, j, k, mblk, nblk, pblk);
				}
			}
		}
		printf("\t 输出C=A*B'的结果 \n");
		MatixPrin(*rawCABO,rawm,rawp);
}

// 计算结果打印输出函数
void MatixPrin(data_type *A,int m,int n){
    int i,j;
    for(i = 0; i <n ; i++){
        for(j = 0; j <m ; j++)
         printf("C[%d][%d]=%f  ", i,j,A[i*n+j]);
        printf("\n");
    }
}

//2.1 子矩阵乘法 C=A'*B
void SMblock_MultCAOB(int si,int sj,int sk,int subm,int subn,int subp) {
	int i, j, k;
	for (j = 0; j < subn; j++){ //列号
	   for (i = 0; i < subm; i++) { //行号
			for (k = 0; k < subp; k++) { //并行
				//printf("子块乘：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",sj * T + j,sk * S + k,si*S + i,sj * T + j,si * T + i,sk*S + k);
				//C[j * p + k]+= A[i*m + j] * B[i * p + k];  //参考
				rawCAOB[sj * T + j][sk * T + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * T + k];
			}
		}
	}
}
//2.2 分块矩阵运算：调用乘法实现分块矩阵乘法 C=A'*B，区别在于分块调用时循环次数为N*N*M，而不是M*M*N
//@data_type A[M][N], data_type B[M][N], data_type C[N][N]
void Mult_blkCAOB(data_type *A, data_type *B, data_type *C) {
	int i, j, k;
	int count=0;//循环计数
	for (j = 0; j < AR_N; j++) {
		for (i = 0; i < AC_M; i++) {
				for (k = 0; k < AR_N; k++)  {
				printf("\t 第%d层循环：  ",++count);
				printf(" 分块乘法：C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",j,k,i,j,i,k);
				int mblk=S,nblk=T,pblk=T;//默认当前参与运算的两个子矩阵块的大小，必须每次循环重新赋初值
				//计算当前子块的大小为mblk*nblk
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
		printf("\t 输出C=A'*B的结果 \n");
		MatixPrin(*rawCAOB,rawn,rawn);
}


int main(void) {
	MatixPros();//矩阵分块处理
	//Mult_blk(*rawA,*rawB,*rawC);  //分块矩阵运算，含分块矩阵乘法 C=A*B、点积 C=A.*B
	//Mult_blkCAOB(*rawA, *rawB, *rawCAOB);  //分块矩阵的乘法 C=A'*B
	//Mult_blkCABO(*rawA, *rawB, *rawCABO);  //分块矩阵的乘法 C=A*B'

	//转置矩阵的暴力C[m][p]=A'[n][m]*B[n][p]，注意矩阵下标的对应关系发生了变化
	R_MultA(*rawA, *rawB, *rawC, rawn, rawm, rawn);
	printf("C=A'*B的暴力计算结果\n");
	MatixPrin(rawC[0],rawn,rawn);

	//暴力矩阵运算测试(*正确*)
	//R_MultB(*rawA, *rawB, *rawC, rawm, rawn, rawp);
	//printf("C=A*B'的暴力计算结果\n");
	//MatixPrin(rawC[0],rawm,rawp);
	//R_MultB(rawA[0], rawB[0], rawCDot[0], rawm, rawn, rawp);
	//MatixPrin(rawCDot[0],rawn,rawn);
    return 0;
}

