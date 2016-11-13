#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

#define DEMENSION 5  //数据维度，包括特征属性x的维度和标签y

//样本结构体
struct record {
	double x[DEMENSION];
	int y;
};

//读取文件数据
void getData(fstream &datafile, vector<record> &dataset) {
	while (!datafile.eof()) {
		record curRecord;
		curRecord.x[0] = 1; //这里初始化的值必须为1
		int i;
		for (i = 1; i < DEMENSION; i++)
			datafile >> curRecord.x[i];
		datafile >> curRecord.y;
		dataset.push_back(curRecord);
	}
	datafile.close();
}

//计算sign值
int sign(double x) {
	if (x <= 0)
		return -1;
	else
		return 1;
}

//计算两个向量内积，判断是否需要修正
double multiply(double *v1, double *v2) {
	int i;
	double temp = 0.0;
	for (i = 0; i < DEMENSION; i++)
		temp += v1[i] * v2[i];
	return temp;
}

//函数重载，计算向量v与整数num的积，用于计算y*x（y为+1或-1，x为向量）
void multiply(double *result, double *v, int num) {
	int i;
	for (i = 0; i < DEMENSION; i++)
		result[i] = num * v[i];
}

//计算两向量的和放入result中，用于计算w(i+1)=w(i)+y*x
void add(double *v1, double *v2) {
	int i;
	for (i = 0; i < DEMENSION; i++)
		v1[i] += v2[i];
}

//计算错误率
double getErrorRate(double *weight, vector<record> dataset) {
	int n = dataset.size(); //所有的样本个数为n
	cout << "样本个数为： " << n << endl;
	double errorRate = 0.0;
	int i;
	for (i = 0; i < n; i++)
		if (sign(multiply(weight, dataset[i].x)) != dataset[i].y)
			errorRate++;
	cout << "error rate : " << errorRate << endl;
	return errorRate / n;
}

//口袋PLA算法，式中的iteration是迭代次数，是每次做训练的样本的个数，但是由于每次训练之前都有一个打乱样本的过程，所以每次的50个样本是不一样的
void pocketPLA(double *pocketWeights, double *weights,
		vector<record> trainingSet, int iteration) {
	int index = 0;
	int iter = 1;
	int n = trainingSet.size();
	while (iter <= iteration) {
		if (sign(multiply(trainingSet[index].x, weights))
				!= trainingSet[index].y) //判断标签不一致，准备correct
				{
			double temp[DEMENSION];
			multiply(temp, trainingSet[index].x, trainingSet[index].y);
			int i;
			for (i = 0; i < DEMENSION; i++)
				weights[i] += temp[i]; //也可以用add(weight,temp)替代
			if (getErrorRate(weights, trainingSet)
					< getErrorRate(pocketWeights, trainingSet)) {
				//如果修正后的权重集比口袋里的要好，则把口袋里的权重集换成这个
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
	for (j = 0; j < 2000; j++) //每次训练好的权重集去测试这个权重集在data(资料)为test_data上的表现如何(错误率多少)
			{
		random_shuffle(trainingSet.begin(), trainingSet.end()); //只取前50个样本做训练，每次都随机打乱顺序，用函数random_shuffle
		int i;
		for (i = 0; i < DEMENSION; i++) {  //注意，这里需要初始化！！！不初始化值会乱码，程序出错！！！
			weights[i] = 0.0;
			pocketWeights[i] = 0.0;
		}
		pocketPLA(pocketWeights, weights, trainingSet, 50);
		double trainingError = getErrorRate(pocketWeights, trainingSet);
		double testError = getErrorRate(pocketWeights, testSet);
		ave_error += testError;
		cout << "第" << j << "次实验---" << "training error:" << trainingError
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
