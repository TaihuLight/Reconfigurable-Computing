/*
 * ���ݾ���Ԫ�ص��±���зֿ�˷�
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matix.h"

//void R_MultA(data_type *A, data_type *B, data_type *C, int m, int n, int p);
//void R_MultB(data_type *A, data_type *B, data_type *C, int m, int n, int p);
//��������Ϊȫ�ֳ����ͱ���
//data_type rawA[rawm][rawn] = {4,2,3,3,2,1,4,2};
//data_type rawB[rawp][rawn] = {3,2,3,3,2,1,1,4};

//data_type rawA[rawm][rawn] = {3,1,4,5,2,3};
//data_type rawB[rawp][rawn] = {2,4,3,3,1,2};
/*����*/
data_type rawA[rawm][rawn] = { 3, 2, 2, 3, 3, 1, 3, 3, 4, 4, 3, 3, 2, 3, 3, 4 };
data_type rawB[rawp][rawn] = { 1, 2, 3, 1, 1, 2, 4, 1, 1, 2, 4, 3, 1, 4, 4, 1 };
//data_type rawA[rawm][rawn] = { 3, 2, 2, 3, 3, 1, 3, 3, 4, 4, 3, 3, 2, 3, 3 };
//data_type rawB[rawn][rawp] = { 1, 2, 3, 1, 1, 2, 4, 1, 1, 2, 4, 3, 1, 4, 4 };
//������Ԫ�س�ʼ��Ϊȫ0,��ֹ����Ī���������������
//��ά�����ʼ��Ϊ0����ʽΪ{{0.0}}��һά�����ʼ��Ϊ0����ʽΪ{0.0}
data_type rawC[rawn][rawp]={{0.0}}; //�˷���C=A*B
data_type rawCDot[rawn][rawp]={{0.0}}; //�����C=A.*B
data_type rawCAOB[rawn][rawn]={{0.0}}; //�˷���C=A��*B
data_type rawCABO[rawm][rawp]={{0.0}}; //�˷���C=A*B��

//���������������ں�������֮ǰ���û���־�����Ϣ
void MatixPrin(data_type *A,int m,int n);

//�ֿ���Ӿ���ĸ���h*l,A�����ΪS*T���Ӿ���B�����ΪT*S���Ӿ���
static int AC_M = 1;
static int AR_N = 1;
static int BR_P = 1;

//����A��B�ֿ�󣬲���ȫ�ֿ�ʱ�����һ�к����һ�е��Ӿ���Ĵ�С
static int AC_last = 1;
static int AR_last = 1;
static int BR_last = 1;

//����ֿ鴦������ֿ���Ӿ����ĸ���
void MatixPros(){
	//������rawA[rawm][rawn]��ΪAC_M*AR_N����СΪS*T���ӿ飬ceil(x)�������ز�С��x����С����
	if (rawm % S == 0) {
		AC_M = rawm / S;
	} else {
		AC_M = rawm / S + 1;
	}
	//AC_M = ceil((double) rawm / (double) S); //����A�ֿ�������
	//ʹ��ceil������Ҫ����ͷ�ļ�math.h
	AR_N = ceil((double) rawn / (double) T); //����A�ֿ���������������B�ֿ�������
	//������rawB[rawn][rawp]��ΪAR_N*BR_P����СΪT*S���ӿ�
	BR_P = ceil((double) rawp / (double) S); //����B�ֿ�������
	//����A��B�ֿ�󣬲���ȫ�ֿ�ʱ�����һ�к����һ�е��Ӿ����С
	 AC_last = rawm-(AC_M-1)*S;
	 AR_last = rawn-(AR_N-1)*T;
	 BR_last = rawp-(BR_P-1)*S;
}

//1.1 ���Ƿֿ����ӿ����˷�
//@ si,sj,sk��ʾ��ǰ�Ӿ����һ����Ԫ�ص����±�����±�(�Ӿ���ı�ţ���A11,A12)
//@ ��ǰ�Ӿ���Ĵ�Сsubm,subn,subp�ֱ��Ӧ��ǰ�ӿ���������������������Ĭ���������Ӿ������S*T��T*S�Ĵ�С�������ڲ�������Ӿ���
void SMblock_Mult(int si,int sj,int sk,int subm,int subn,int subp) {
	//������ӿ�����һ��Ԫ����ԭ�����ӦԪ�ؼ��λ�ù�ϵ�����ӿ��±�ת��Ϊԭ����Ԫ���±��ʾ
	int i, j, k;
	for (i = 0; i < subm; i++) { //�к�
		for (j = 0; j < subn; j++) { //�к�
			for (k = 0; k < subp; k++) { //����
				//rawA[i]��ʾrawA�Ƕ�ά�����i����Ԫ�صĵ�ַ
				rawC[si*S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sj*T + j][sk * S + k];//�Ӿ���˷�C=A*B
				//if(k==0){//��k��ѭ���޹أ�ֻk==0ʱִ��һ�μ���
				//	rawCDot[si*S + i][sj * T + j] = rawA[si*S + i][sj * T + j]* rawB[si*S + i][sj * T + j];//�Ӿ���ĵ��
				//}
				//ת�þ���ĳ˷���C=A'*B
				//rawCAOB[sj * T + j][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * S + k];
				//ת�þ���ĳ˷���C=A*B'
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
				//printf("  �˻�ֵC=%f",rawC[si*S + i][sk * S + k]);
				//printf("\t C[%d][%d]+=A[%d][%d]*B[%d][%d]+=    ", si*S + i, sk * S +k ,si*S + i, sj* T + j, sj*T + j, sk * S + k);
			}
			//printf("\n");
		}
	}
}

//1.2 ���Ƿֿ����ӿ������
//@ si,sj,sk��ʾ��ǰ�Ӿ����һ����Ԫ�ص����±�����±�(�Ӿ���ı�ţ���A11,A12)
//@ ��ǰ�Ӿ���Ĵ�Сsubm,subn,subp�ֱ��Ӧ��ǰ�ӿ���������������������Ĭ���������Ӿ������S*T��T*S�Ĵ�С�������ڲ�������Ӿ���
void SMblock_MultDot(int si, int sj, int subm, int subn) {
	//������ӿ�����һ��Ԫ����ԭ�����ӦԪ�ؼ��λ�ù�ϵ�����ӿ��±�ת��Ϊԭ����Ԫ���±��ʾ
	int i, j;
	for (i = 0; i < subm; i++) { //�к�
		for (j = 0; j < subn; j++) { //�к�
			rawCDot[si * S + i][sj * T + j] = rawA[si * S + i][sj * T + j]* rawB[si * S + i][sj * T + j]; //�Ӿ���ĵ��
		}
	}
}

//1.2 �ֿ�������㣺���ó˷�ʵ�ַֿ����˷������
void Mult_blk(data_type *A, data_type *B, data_type *C) {
	int i, j, k;
	int count = 0; //ѭ������
	for (i = 0; i < AC_M; i++) {
		for (j = 0; j < AR_N; j++) {
			////////////////////////////////
			int mblk = S, nblk = T, pblk = S; //Ĭ�ϵ�ǰ��������������Ӿ����Ĵ�С������ÿ��ѭ�����¸���ֵ
			//���㵱ǰ�ӿ�Ĵ�С
			if ((i == AC_M - 1)) {
				mblk = AC_last;
			}
			if (j == AR_N - 1) {
				nblk = AR_last;
			}
			///////////////////////////////
			for (k = 0; k < BR_P; k++) {
				printf("\t ��%d��ѭ����  ", ++count);
				//��1�����÷ֿ�ľ���˷����㺯��
				//��AB��������԰����ӿ�����ʱ
				//SMblock_Mult(i,j,k,S,T,S);
				//��AB������ڲ������ֵ��ӿ�ʱ
				//printf("\t ���ԣ�AC_M=%d,AR_N=%d,BR_P=%d \n", AC_M, AR_N, BR_P);
				pblk = S;
				if ((k == BR_P - 1)) {
					pblk = BR_last;
				}
				//�ֿ����˷�C=A*B
				//printf(" �ֿ�˷���C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",i,k,i,j,j,k);
				SMblock_Mult(i, j, k, mblk, nblk, pblk);
			}
			//�ֿ����ĵ������
			SMblock_MultDot(i, j, mblk, nblk);
			//printf(" �ֿ�����C[%d][%d]+=A[%d][%d]*B[%d][%d] \n", i, j, i, j, i,j);
		}
	}
	printf("\t ���C=A*B�Ľ�� \n");
	MatixPrin(*rawC,rawm, rawp);  //��ΪrawC�Ƕ�ά���飬���Ե�ַ���õķ����� rawC[0]
	printf("\t ���C=A.*B�Ľ�� \n");
	MatixPrin(*rawCDot,rawm, rawn);
	printf("\t ���C=A'*B�Ľ�� \n");
	MatixPrin(*rawCAOB,rawn,rawn);
	printf("\t ���C=A*B'�Ľ�� \n");
	MatixPrin(*rawCABO,rawm,rawp);
}

//3.1 �Ӿ���˷� C[m][p]=A*B'=A[m][n]*B'[p][n]
void SMblock_MultCABO(int si,int sj,int sk,int subm,int subn,int subp) {
	int i, j, k;
		for (i = 0; i < subm; i++){ //�к�
			//data_type sum=0.0;
			for (k = 0; k < subp; k++) { //����
				for (j = 0; j < subn; j++) { //�к�
				//C[i * p + k]+= A[i * n + j] * B[k * n + j]; //�ο�
				rawCABO[si * S + i][sk * S + k] += rawA[si*S + i][sj * T + j]* rawB[sk*S + k][sj * T + j];
			}
			//rawCAOB[si*S + i][sk * S + k]=sum;
		}
	}
}
//3.2 �ֿ�������㣺���ó˷�ʵ�ַֿ����˷� C=A*B'
void Mult_blkCABO(data_type *A, data_type *B, data_type *C) {
		int i, j, k;
		int count=0;//ѭ������
	for (i = 0; i < AC_M; i++) {
		for (k = 0; k < BR_P; k++) {
			for (j = 0; j < AR_N; j++) {
				printf("\t ��%d��ѭ����  ",++count);
				printf(" �ֿ�˷���C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",i,k,i,j,k,j);
				int mblk=S,nblk=T,pblk=S;//Ĭ�ϵ�ǰ��������������Ӿ����Ĵ�С������ÿ��ѭ�����¸���ֵ
				//���㵱ǰ�ӿ�Ĵ�С
				if ((i == AC_M - 1)) {
					mblk = AC_last;
				}
				if (j == AR_N - 1) {
					nblk = AR_last;
				}
				if ((k == BR_P - 1)) {
					pblk = BR_last;
				}
				//�ֿ����˷�C=A*B'
				SMblock_MultCABO(i, j, k, mblk, nblk, pblk);
				}
			}
		}
		printf("\t ���C=A*B'�Ľ�� \n");
		MatixPrin(*rawCABO,rawm,rawp);
}

// ��������ӡ�������
void MatixPrin(data_type *A,int m,int n){
    int i,j;
    for(i = 0; i <n ; i++){
        for(j = 0; j <m ; j++)
         printf("C[%d][%d]=%f  ", i,j,A[i*n+j]);
        printf("\n");
    }
}

//2.1 �Ӿ���˷� C=A'*B
void SMblock_MultCAOB(int si,int sj,int sk,int subm,int subn,int subp) {
	int i, j, k;
	for (j = 0; j < subn; j++){ //�к�
	   for (i = 0; i < subm; i++) { //�к�
			for (k = 0; k < subp; k++) { //����
				//printf("�ӿ�ˣ�C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",sj * T + j,sk * S + k,si*S + i,sj * T + j,si * T + i,sk*S + k);
				//C[j * p + k]+= A[i*m + j] * B[i * p + k];  //�ο�
				rawCAOB[sj * T + j][sk * T + k] += rawA[si*S + i][sj * T + j]* rawB[si*S + i][sk * T + k];
			}
		}
	}
}
//2.2 �ֿ�������㣺���ó˷�ʵ�ַֿ����˷� C=A'*B���������ڷֿ����ʱѭ������ΪN*N*M��������M*M*N
//@data_type A[M][N], data_type B[M][N], data_type C[N][N]
void Mult_blkCAOB(data_type *A, data_type *B, data_type *C) {
	int i, j, k;
	int count=0;//ѭ������
	for (j = 0; j < AR_N; j++) {
		for (i = 0; i < AC_M; i++) {
				for (k = 0; k < AR_N; k++)  {
				printf("\t ��%d��ѭ����  ",++count);
				printf(" �ֿ�˷���C[%d][%d]+=A[%d][%d]*B[%d][%d] \n",j,k,i,j,i,k);
				int mblk=S,nblk=T,pblk=T;//Ĭ�ϵ�ǰ��������������Ӿ����Ĵ�С������ÿ��ѭ�����¸���ֵ
				//���㵱ǰ�ӿ�Ĵ�СΪmblk*nblk
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
		printf("\t ���C=A'*B�Ľ�� \n");
		MatixPrin(*rawCAOB,rawn,rawn);
}


int main(void) {
	MatixPros();//����ֿ鴦��
	//Mult_blk(*rawA,*rawB,*rawC);  //�ֿ�������㣬���ֿ����˷� C=A*B����� C=A.*B
	//Mult_blkCAOB(*rawA, *rawB, *rawCAOB);  //�ֿ����ĳ˷� C=A'*B
	//Mult_blkCABO(*rawA, *rawB, *rawCABO);  //�ֿ����ĳ˷� C=A*B'

	//ת�þ���ı���C[m][p]=A'[n][m]*B[n][p]��ע������±�Ķ�Ӧ��ϵ�����˱仯
	R_MultA(*rawA, *rawB, *rawC, rawn, rawm, rawn);
	printf("C=A'*B�ı���������\n");
	MatixPrin(rawC[0],rawn,rawn);

	//���������������(*��ȷ*)
	//R_MultB(*rawA, *rawB, *rawC, rawm, rawn, rawp);
	//printf("C=A*B'�ı���������\n");
	//MatixPrin(rawC[0],rawm,rawp);
	//R_MultB(rawA[0], rawB[0], rawCDot[0], rawm, rawn, rawp);
	//MatixPrin(rawCDot[0],rawn,rawn);
    return 0;
}

