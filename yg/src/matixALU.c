/*
 * ��������ֻ�������
 *  Created on: 2016��4��12��
 * �ֿ�����ת�÷����ν��У���һ��Ԫ�ؾ���ת�ã��������λ������󣬸��ӿ������ת�á�
 * �ֿ����ļӷ��͵����Ҫ����ͬ�;����ҷֿ鷽����ͬ��
 */
#include <stdio.h>
#include <stdlib.h>
#include "matix.h"

//����ת�ã�������/�����Ǿ��󣬲��������л���
void Mat_trans(data_type *matx[],int m,int n) {
	data_type tmp = 0;
	int i,j;
	for (i = 0; i < m; i++) { //�к�
		for (j = 0; j < n; j++) { //�к�
			if (i < j) {//Ԫ�ػ���
				tmp = matx[i][j];
				matx[i][j] = matx[j][i];
				matx[j][i] = tmp;
			}
		}
	}
}

//�����б����˷�
//@data_type A[m][n], data_type B[n][p], data_type C[m][p]
void R_Mult1(data_type *A, data_type *B, data_type *C,int m,int n,int p) {
	int i, j, k;
	for (i = 0; i < m; i++) { //m�к�
		data_type sum[p];
		for (j = 0; j < n; j++) { //�к�
			for (k = 0; k < p; k++) {//����
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

//��� C=A.*B
//@data_type A[m][n], data_type B[m][n], data_type C[m][n]
void MatrixDot(data_type *A, data_type *B, data_type *C, int m, int n) {
	int i, j;
	for (i = 0; i < m; i++) { //m�к�
		for (j = 0; j < n; j++) { //����
			data_type sum=0.0;  //����һ��Ҫ��ʽ�����˵������������Ī����������⡣
			sum = A[i * n + j] * B[i * n + j];
			C[i * n + j]=sum;
		}
	}
}


//ת�þ���˷�3-1��C=A'*B,ע������к��кŵı仯
//@data_type A[n][m], data_type B[n][p], data_type C[m][p]
void R_MultA(data_type *A, data_type *B, data_type *C, int m, int n, int p) {
	int i, j, k;
	for (j = 0; j < m; j++) { //����Aת��ǰ���кţ���ת�ú���к�
		for (i = 0; i < n; i++) { //B���к�
			for (k = 0; k < p; k++) { //����
				//printf("C[%d]=%f",j * p + k,C[j * p + k]);//��ⲿ������ĳ�ʼֵ����0.0
				//������Ӵ˾䣬����C�ĳ�ֵ��һ��Ϊ0.
				//if(i==0){
					//C[j * p + k]=0.0;
				//}
				C[j * p + k]+= A[i*m + j] * B[i * p + k];
				//printf("\t C[%d][%d]=A[%d][%d]*B[%d][%d]+=%f*%f=%f \n",j,k,i,j,i,k,A[i*m + j],B[i * p + k],C[j*p+k]);
			}
		}
	}
}

//ת�þ���˷�4��C=A*B'�������ȷ 6-12
//@data_type A[m][n], data_type B[p][n], data_type C[m][p]
void R_MultB(data_type *A, data_type *B, data_type *C, int m, int n, int p) {
	int i, j, k;
	for (i = 0; i < m; i++) { //m�к�
		for (k = 0; k < p; k++) { //�к�
			data_type sum=0.0;
			for (j = 0; j < n; j++) { //����
				sum += A[i * n + j] * B[k * n + j];
				printf("\t���� C[%d][%d]+=A[%d]*B[%d]=%f*%f=%f",i,k,i*n+j,k*n+j,A[i * n + j] , B[k * n + j],sum);
			}
			C[i * p + k]=sum;
			printf("\n");
		}
	}
}
