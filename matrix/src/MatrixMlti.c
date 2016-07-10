#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix_SM.h"

//�ǲ��еĴ��룬������ڲ��Դ�����
//����ֿ鴦������ֿ���Ӿ����ĸ���
void MatixPros(unsigned short int blkp[6]){
	//�ֿ���Ӿ���ĸ���h*l,A�����ΪS*T���Ӿ���B�����ΪT*S���Ӿ���
	//unsigned short int��ʾ�޷�����������ռ��2���ֽڣ�unsigned int��int��ռ4���ֽ�
//AC_M
	if (rawm % S == 0) {
		blkp[0] = rawm / S;
	} else {
		blkp[0] = rawm / S + 1;
	}
	blkp[1] = ceil((double) rawn / (double) T);  //AR_N
	blkp[2] = ceil((double) rawp / (double) S);  //BR_P
	//����A��B�ֿ�󣬲���ȫ�ֿ�ʱ�����һ�к����һ�е��Ӿ���Ĵ�С
	blkp[3] = rawm-(blkp[0]-1)*S;   //AC_last
	blkp[4] = rawn-(blkp[1]-1)*T;  //AR_last
	blkp[5] = rawp-(blkp[2]-1)*S;  //BR_last
}

//�ֿ����ӿ����˷�
void SMblock_Mult(data_type rawA[rawm][rawn], data_type rawB[rawm][rawn],
		data_type rawD[rawn][rawp], data_type rawCAD[rawm][rawp],
		data_type rawCABO[rawm][rawm], unsigned short int si, unsigned short int sj, unsigned short int sk, unsigned short int subm,
		unsigned short int subn, unsigned short int subp,unsigned short int flag,data_type rawCABot[rawm][rawn]) {
	unsigned short int i, j, k;
	for (i = 0; i < subm; i++) { //�к�
		for (j = 0; j < subn; j++) { //�к�
			for (k = 0; k < subp; k++) { //����
				//����˷�C[m][p]=A[m][n]*D[n][p],����rawA[i]��ʾrawA�Ƕ�ά�����i����Ԫ�صĵ�ַ
				rawCAD[si*S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawD[sj*T + j][sk * S + k];//�Ӿ���˷�
				//ת�þ���ĳ˷���C[m][p]=A*B'=A[m][n]*B'[p][n]
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
			}
			//
			if(flag==0){
				rawCABot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //�Ӿ���ĵ��
			}
		}
	}
}

//�ֿ����ӿ������C[m][n]=A[m][n].*B[m][n]���ѽ��ú���д����SMblock_Mult�������ۺ�ʱ���������������Զ�ת������
/*
void SMblock_MultDot(data_type rawA[rawm][rawn],data_type rawB[rawm][rawn],data_type rawCABot[rawm][rawn],int si, int sj, int subm, int subn) {
	unsigned short int i, j;
	for (i = 0; i < subm; i++) { //�к�
		for (j = 0; j < subn; j++) { //�к�
			rawCABot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //�Ӿ���ĵ��
		}
	}
}
*/

//�ֿ�������㣺���ó˷�ʵ�ַֿ����3�־���˷��;���������
void Mult_blk(data_type rawA[rawm][rawn],data_type rawB[rawm][rawn],data_type rawD[rawn][rawp],data_type rawCAD[rawm][rawp],
		data_type rawCABot[rawm][rawn], data_type rawCABO[rawm][rawm],unsigned short int blkb[]) {
	unsigned short int i, j, k;
	for (i = 0; i < blkb[0]; i++) {  //AC_M
		for (j = 0; j < blkb[1]; j++) {  //AR_N
			//Ƶ��ʹ�õļĴ�������
			unsigned short  int mblk = S, nblk = T;
			//���㵱ǰ�ӿ�Ĵ�С
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
				flag=1;  //��ִ��һ��
			}
			//SMblock_MultDot(&rawA[0],&rawB[0],&rawCABot[0],i, j, mblk, nblk);
		}
	}
}

//2.1 �Ӿ���˷� C=A'*B
void SMblock_MultCAOB(data_type rawA[rawm][rawn],data_type rawB[rawm][rawn],data_type rawCAOB[rawn][rawn],unsigned short int si,unsigned short int sj,unsigned short int sk,unsigned short int subm,unsigned short int subn,unsigned short int subp) {
	unsigned short int i, j, k;
	for (j = 0; j < subn; j++){ //�к�
	   for (i = 0; i < subm; i++) { //�к�
			for (k = 0; k < subp; k++) { //����
				rawCAOB[sj * T + j][sk * T + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * T + k];

			}
		}
	}
}
//ʵ�ַֿ����˷� C=A'*B���������ڷֿ����ʱѭ������ΪN*N*M��������M*M*N
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
				//�ֿ����˷�C=A'*B
				SMblock_MultCAOB(&rawA[0],&rawB[0],&rawCAOB[0],i, j, k, mblk, nblk,pblk);
				}
			}
		}
}
//���㺯��
void Matix_Blkmulti(data_type RA[rawm][rawn], data_type RB[rawm][rawn],
		data_type RD[rawn][rawp], data_type rawCAD[rawm][rawp],
		data_type rawCABot[rawm][rawn], data_type rawCAOB[rawn][rawn],
		data_type rawCABO[rawm][rawm]) {

	//����һ�������ʾ�ֿ���ӿ���±���ӿ��С
	unsigned short int mblk[6] = { 1, 1, 1, 1, 1, 1 };
	MatixPros(mblk); //����ֿ鴦��

	Mult_blk(&RA[0], &RB[0], &RD[0], &rawCAD[0],&rawCABot[0], &rawCABO[0], mblk);  //�ֿ��������
	Mult_blkCAOB(&RA[0], &RB[0], &rawCAOB[0], mblk);  //�ֿ����ĳ˷� C=A'*B
}
