#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

#define DEMENSION 5  //����ά�ȣ�������������x��ά�Ⱥͱ�ǩy

//�����ṹ��
struct record {
	double x[DEMENSION];
	int y;
};

//��ȡ�ļ�����
void getData(fstream &datafile, vector<record> &dataset) {
	while (!datafile.eof()) {
		record curRecord;
		curRecord.x[0] = 1; //�����ʼ����ֵ����Ϊ1
		int i;
		for (i = 1; i < DEMENSION; i++)
			datafile >> curRecord.x[i];
		datafile >> curRecord.y;
		dataset.push_back(curRecord);
	}
	datafile.close();
}

//����signֵ
int sign(double x) {
	if (x <= 0)
		return -1;
	else
		return 1;
}

//�������������ڻ����ж��Ƿ���Ҫ����
double multiply(double *v1, double *v2) {
	int i;
	double temp = 0.0;
	for (i = 0; i < DEMENSION; i++)
		temp += v1[i] * v2[i];
	return temp;
}

//�������أ���������v������num�Ļ������ڼ���y*x��yΪ+1��-1��xΪ������
void multiply(double *result, double *v, int num) {
	int i;
	for (i = 0; i < DEMENSION; i++)
		result[i] = num * v[i];
}

//�����������ĺͷ���result�У����ڼ���w(i+1)=w(i)+y*x
void add(double *v1, double *v2) {
	int i;
	for (i = 0; i < DEMENSION; i++)
		v1[i] += v2[i];
}

//���������
double getErrorRate(double *weight, vector<record> dataset) {
	int n = dataset.size(); //���е���������Ϊn
	cout << "��������Ϊ�� " << n << endl;
	double errorRate = 0.0;
	int i;
	for (i = 0; i < n; i++)
		if (sign(multiply(weight, dataset[i].x)) != dataset[i].y)
			errorRate++;
	cout << "error rate : " << errorRate << endl;
	return errorRate / n;
}

//�ڴ�PLA�㷨��ʽ�е�iteration�ǵ�����������ÿ����ѵ���������ĸ�������������ÿ��ѵ��֮ǰ����һ�����������Ĺ��̣�����ÿ�ε�50�������ǲ�һ����
void pocketPLA(double *pocketWeights, double *weights,
		vector<record> trainingSet, int iteration) {
	int index = 0;
	int iter = 1;
	int n = trainingSet.size();
	while (iter <= iteration) {
		if (sign(multiply(trainingSet[index].x, weights))
				!= trainingSet[index].y) //�жϱ�ǩ��һ�£�׼��correct
				{
			double temp[DEMENSION];
			multiply(temp, trainingSet[index].x, trainingSet[index].y);
			int i;
			for (i = 0; i < DEMENSION; i++)
				weights[i] += temp[i]; //Ҳ������add(weight,temp)���
			if (getErrorRate(weights, trainingSet)
					< getErrorRate(pocketWeights, trainingSet)) {
				//����������Ȩ�ؼ��ȿڴ����Ҫ�ã���ѿڴ����Ȩ�ؼ��������
				int j;
				for (j = 0; j < DEMENSION; j++)
					pocketWeights[j] = weights[j];
			}
			iter++;
			cout << "iter: " << iter << endl;
		}
		if (index == n - 1)
			index = 0;
		else
			index++;
	}
}

int main() {
	vector<record> trainingSet;
	vector<record> testSet;

	fstream datafile1("training_data.dat");
	fstream datafile2("test_data.dat");

	if (datafile1.is_open() && datafile2.is_open()) {
		getData(datafile1, trainingSet);
		getData(datafile2, testSet);
	} else {
		cout << "can not open file!" << endl;
		exit(1);
	}
	double weights[DEMENSION], pocketWeights[DEMENSION];
	double ave_error = 0.0;
	int j;
	for (j = 0; j < 2000; j++) //ÿ��ѵ���õ�Ȩ�ؼ�ȥ�������Ȩ�ؼ���data(����)Ϊtest_data�ϵı������(�����ʶ���)
			{
		random_shuffle(trainingSet.begin(), trainingSet.end()); //ֻȡǰ50��������ѵ����ÿ�ζ��������˳���ú���random_shuffle
		int i;
		for (i = 0; i < DEMENSION; i++) {  //ע�⣬������Ҫ��ʼ������������ʼ��ֵ�����룬�����������
			weights[i] = 0.0;
			pocketWeights[i] = 0.0;
		}
		pocketPLA(pocketWeights, weights, trainingSet, 50);
		double trainingError = getErrorRate(pocketWeights, trainingSet);
		double testError = getErrorRate(pocketWeights, testSet);
		ave_error += testError;
		cout << "��" << j << "��ʵ��---" << "training error:" << trainingError
				<< " test error:" << testError << endl;
	}
	cout << "average error rate:" << ave_error / 2000 << endl;
	return 0;
}

int readwritefile() {
	string filename = "test.txt";

	// Write to File
	ofstream fout(filename.c_str());  // default mode is ios::out | ios::trunc
	if (!fout) {
		cerr << "error: open file for output failed!" << endl;
		abort();  // in <cstdlib> header
	}
	fout << "apple" << endl;
	fout << "orange" << endl;
	fout << "banana" << endl;
	fout.close();

	// Read from file
	ifstream fin(filename.c_str());  // default mode ios::in
	if (!fin) {
		cerr << "error: open file for input failed!" << endl;
		abort();
	}
	char ch;
	while (fin.get(ch)) {  // till end-of-file
		cout << ch;
	}
	fin.close();
	return 0;
}
