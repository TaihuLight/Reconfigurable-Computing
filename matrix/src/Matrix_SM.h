/*
 * Matrix_SM.h
 */

#ifndef MATRIX_SM_H_
#define MATRIX_SM_H_

typedef float data_type;//����Ϊ16λ������

//ԭ����Ĵ�С����������
#define rawm 128
#define rawn 256
#define rawp 128
//�Ӿ���Ĵ�СΪS��*T��
#define S 4  //��������
#define T 6  //��������

data_type rawA[rawm][rawn];
data_type rawB[rawm][rawn];
data_type rawD[rawn][rawp];
data_type rawC[rawm][rawp]; //����˷���C=A*D����ά��������Ԫ�س�ʼ��Ϊ0����ʽΪ{{0.0}}
data_type rawCDot[rawm][rawn]; //��������CDot=A.*B
data_type rawCAOB[rawn][rawn]; //ת�þ���˷���CAOB=A��*B
data_type rawCABO[rawm][rawm]; //ת�þ���˷���CABO=A*B��
//��������
void MatixPros();//����ֿ�Ԥ������
void Mult_blk();//�ֿ��������
void MatixPrin(data_type * R, int m, int n);//�����ӡ�������
void Mult_blkCAOB(data_type *A, data_type *B, data_type *C) ;
#endif /* MATRIX_SM_H_ */
