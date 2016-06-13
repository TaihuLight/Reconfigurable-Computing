/*
 * Multi_Test.c
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Matrix_SM.h"

//��������
void VtoMat(data_type *Vr,data_type *Mat,int row,int col);
void CheckResult(data_type * targ_result, data_type * comp_result, int m, int n);

//��ȡ���������ļ��еľ���
data_type rA[] = {
#include "DataSet_Test\Mat_A.h"
		};
data_type mB[] = {
#include "DataSet_Test\Mat_B.h"
		};
data_type mD[] = {
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

//�����ļ��ж�ȡ�ľ���ֵ���������������ľ���

void VtoMat(data_type *Vr,data_type *Mat,int row,int col){
	unsigned int i,j;
    for(i = 0; i <row ; i++){
        for(j = 0; j <col ; j++){
            Mat[i * col + j]=Vr[i * col + j];
        }
    }
}

int main(void) {
    //��ȡ����Ԫ��
	VtoMat(&rA[0], rawA[0], rawm, rawn);
	VtoMat(&mB[0], rawB[0], rawm, rawn);
	VtoMat(&mD[0], rawD[0], rawn, rawp);

	MatixPros(); //����ֿ鴦��
	Mult_blk();  //�ֿ��������
	Mult_blkCAOB(*rawA, *rawB, *rawCAOB);  //�ֿ����ĳ˷� C=A'*B
	CheckResult(&rustC[0], rawC[0], rawm, rawp);
	CheckResult(&rustCDot[0], rawCDot[0], rawm, rawn);
	CheckResult(&rustCABO[0], rawCABO[0], rawm, rawm);
	CheckResult(&rustCAOB[0], rawCAOB[0], rawn, rawn);
	return 0;
}

void CheckResult(data_type * targ_result, data_type * comp_result, int m, int n) {
	unsigned int i, j, count = 0;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if(fabs(targ_result[i*n+j]-comp_result[i*n+j])>1.9){
			count++;
			}
		}
	}
	printf("Checking Results: There are %d Errors\n", count);
}
