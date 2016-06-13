#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix_SM.h"

//�ֿ���Ӿ���ĸ���h*l,A�����ΪS*T���Ӿ���B�����ΪT*S���Ӿ���
unsigned int AC_M = 1;
unsigned int AR_N = 1;
unsigned int BR_P = 1;

//����A��B�ֿ�󣬲���ȫ�ֿ�ʱ�����һ�к����һ�е��Ӿ���Ĵ�С
unsigned int AC_last = 1;
unsigned int AR_last = 1;
unsigned int BR_last = 1;

//����ֿ鴦������ֿ���Ӿ����ĸ���
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

//�ֿ����ӿ����˷�
void SMblock_Mult(int si,int sj,int sk,int subm,int subn,int subp) {
	unsigned int i, j, k;
	for (i = 0; i < subm; i++) { //�к�
		for (j = 0; j < subn; j++) { //�к�
			for (k = 0; k < subp; k++) { //����
				//����˷�C[m][p]=A[m][n]*D[n][p],����rawA[i]��ʾrawA�Ƕ�ά�����i����Ԫ�صĵ�ַ
				rawC[si*S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawD[sj*T + j][sk * S + k];//�Ӿ���˷�
				//ת�þ���ĳ˷���C[m][p]=A*B'=A[m][n]*B'[p][n]
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
			}
		}
	}
}

//�ֿ����ӿ������C[m][n]=A[m][n].*B[m][n]
void SMblock_MultDot(int si, int sj, int subm, int subn) {
	unsigned int i, j;
	for (i = 0; i < subm; i++) { //�к�
		for (j = 0; j < subn; j++) { //�к�
			rawCDot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //�Ӿ���ĵ��
		}
	}
}

//�ֿ�������㣺���ó˷�ʵ�ַֿ����3�־���˷��;���������
void Mult_blk() {
	unsigned int i, j, k;
	for (i = 0; i < AC_M; i++) {
		for (j = 0; j < AR_N; j++) {
			//Ƶ��ʹ�õļĴ�������
			unsigned int mblk = S, nblk = T;
			//���㵱ǰ�ӿ�Ĵ�С
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

//2.1 �Ӿ���˷� C=A'*B
void SMblock_MultCAOB(int si,int sj,int sk,int subm,int subn,int subp) {
	unsigned int i, j, k;
	for (j = 0; j < subn; j++){ //�к�
	   for (i = 0; i < subm; i++) { //�к�
			for (k = 0; k < subp; k++) { //����
				rawCAOB[sj * T + j][sk * T + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * T + k];

			}
		}
	}
}
//ʵ�ַֿ����˷� C=A'*B���������ڷֿ����ʱѭ������ΪN*N*M��������M*M*N
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
				//�ֿ����˷�C=A'*B
				SMblock_MultCAOB(i, j, k, mblk, nblk,pblk);
				}
			}
		}
}
