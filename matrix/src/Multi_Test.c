/*
 * Multi_Test.c
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Matrix_SM.h"

//��������
void VtoMat(data_type *Vr,data_type *Mat,unsigned short int row,unsigned short int col);
void CheckResult(data_type * targ_result, data_type * comp_result, unsigned short int m, unsigned short int n);

//�����ļ��ж�ȡ�ľ���ֵ���������������ľ���
void VtoMat(data_type *Vr,data_type *Mat,unsigned short int row,unsigned short int col){
	unsigned short int i,j;
    for(i = 0; i <row ; i++){
        for(j = 0; j <col ; j++){
            Mat[i * col + j]=Vr[i * col + j];
        }
    }
}

int main(void) {
	//��ȡ���������ļ��еľ���
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
    //��������
	data_type rawA[rawm][rawn];//����ֲ���ά�����ʾ����A
	data_type rawB[rawm][rawn];
	data_type rawD[rawn][rawp];
	data_type rawCAD[rawm][rawp]; //����˷���CAD=A*D����ά��������Ԫ�س�ʼ��Ϊ0����ʽΪ{{0.0}}
	data_type rawCABot[rawm][rawn]; //��������CABot=A.*B
	data_type rawCAOB[rawn][rawn]; //ת�þ���˷���CAOB=A��*B
	data_type rawCABO[rawm][rawm]; //ת�þ���˷���CABO=A*B��

	//��ȡ����Ԫ��
	VtoMat(rA, rawA[0], rawm, rawn);
	VtoMat(&rB[0], rawB[0], rawm, rawn);
	VtoMat(&rD[0], rawD[0], rawn, rawp);

	//���㺯��
	Matix_Blkmulti(&rawA[0],&rawB[0],&rawD[0],&rawCAD[0],&rawCABot[0],&rawCAOB[0],&rawCABO[0]);

	CheckResult(&rustC[0], rawCAD[0], rawm, rawp);
	CheckResult(&rustCDot[0], rawCABot[0], rawm, rawn);
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
