#pragma once
#include <string>
#include"bmi_lisflood.h"

class Tools
{
public:
	static double waterDepthToInflow(double waterDepth, double A, double C1);//���ܼ���������С��λ��m^3/s��,����ֵΪ����ֵ
	static int readCommonPoint(char* inputFile,char* &PS_name);//����ֵΪ��ϵ����
	static int* calcCommonPoint(int* c_count, char* PS_name, int p_number, int num);//ȷ����Ϲ�ϵ��һ��һ����һ�Զ�
};

