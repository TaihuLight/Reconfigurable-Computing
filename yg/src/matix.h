/*
 * matix.h
 * ��ʵ�鿼�����÷����ֺͲ������ֵ��������
 *  Created on: 2016��4��22��
 */

//ԭ����Ĵ�С������������Ϊʵ�ֿ���չ�ԣ�Ӧ�����ĸ����������±�
#define rawm 4
#define rawn 4
#define rawp 4
//�Ӿ���Ĵ�СΪS��*T��
#define S 2  //��������
#define T 2  //��������

typedef float data_type;//����Ϊ32λ������
//��������
void R_MultA(data_type *A, data_type *B, data_type *C, int m, int n, int p);
