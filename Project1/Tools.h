#pragma once
#include <string>
#include"bmi_lisflood.h"

class Tools
{
public:
	static double waterDepthToInflow(double waterDepth, double A, double C1);//功能计算入流大小单位（m^3/s）,返回值为入流值
	static int readCommonPoint(char* inputFile,char* &PS_name);//返回值为耦合点个数
	static int* calcCommonPoint(int* c_count, char* PS_name, int p_number, int num);//确定耦合关系是一对一还是一对多
};

