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
#define S 8  //��������
#define T 12  //��������

//��������
void Matix_Blkmulti(data_type RA[rawm][rawn], data_type RB[rawm][rawn],
		data_type RD[rawn][rawp], data_type rawCAD[rawm][rawp],
		data_type rawCABot[rawm][rawn], data_type rawCAOB[rawn][rawn],
		data_type rawCABO[rawm][rawm]);//���㺯��
#endif /* MATRIX_SM_H_ */
