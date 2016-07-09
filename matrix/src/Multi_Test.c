/*
 * Multi_Test.c
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Matrix_SM.h"

//函数声明
void VtoMat(data_type *Vr,data_type *Mat,unsigned short int row,unsigned short int col);
void CheckResult(data_type * targ_result, data_type * comp_result, unsigned short int m, unsigned short int n);

//读取测试数据文件中的矩阵
data_type rA[] = {
#include "DataSet_Test\Mat_A.h"
		};
data_type rB[] = {
#include "DataSet_Test\Mat_B.h"
		};
data_type rD[] = {
#include "DataSet_Test\Mat_D.h"
		};
data_type rustC[]  = {
#include "DataSet_Test\MultAD.h"
		};
data_type rustCDot[]  = {
#include "DataSet_Test\MultDot.h"
		};
data_type rustCAOB[]  = {
#include "DataSet_Test\MultAOB.h"
		};
data_type rustCABO[]  = {
#include "DataSet_Test\MultABO.h"
		};

//将从文件中读取的矩阵值赋给参与矩阵运算的矩阵
void VtoMat(data_type *Vr,data_type *Mat,unsigned short int Mrow,unsigned short int Mcol){
	unsigned short int i,j;
    for(i = 0; i <Mrow ; i++){
        for(j = 0; j <Mcol ; j++){
            Mat[i * Mcol + j]=Vr[i * Mcol + j];
        }
    }
}

int main(void) {
    //读取矩阵元素
	VtoMat(&rA[0], rawA[0], rawm, rawn);
	VtoMat(&rB[0], rawB[0], rawm, rawn);
	VtoMat(&rD[0], rawD[0], rawn, rawp);

	//顶层函数
	Matix_Blkmulti(rawA[0],rawB[0],rawD[0]);

	CheckResult(&rustC[0], rawC[0], rawm, rawp);
	CheckResult(&rustCDot[0], rawCDot[0], rawm, rawn);
	CheckResult(&rustCABO[0], rawCABO[0], rawm, rawm);
	CheckResult(&rustCAOB[0], rawCAOB[0], rawn, rawn);
	return 0;
}

void CheckResult(data_type * targ_result, data_type * comp_result, unsigned short int m, unsigned short int n) {
	unsigned short int i, j, count = 0;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if(fabs(targ_result[i*n+j]-comp_result[i*n+j])>1.9){
			count++;
			}
		}
	}
	printf("Checking Results: There are %d Errors\n", count);
}
