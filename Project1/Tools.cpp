#include "Tools.h"
#include"bmi_swmm.h"
#define g 9.8
#define C 0.5
#define PI 3.1415926
double Tools::waterDepthToInflow(double waterDepth,double A,double C1) {
	double r = sqrt(A / PI);
	if (r /waterDepth > C1) {
		return C * 2 * PI * r * sqrt(2 * g * waterDepth)* waterDepth; //m³/s
	}
	else
	{
		return C * A * sqrt(2 * g * waterDepth); //m³/s
	}
	
}
int Tools::readCommonPoint(char* inputFile, char* &PS_name) {
	FILE* fp_swmm;
	char line[100] = "";
	PS_name=new char[50000 * 80];
	char* placeholder= get_PSName();
	float  xc, yc;
	int p_number=0;
	int i, j = 0;
	int* numps=NULL;
	numps = get_BCPSNum();
	fp_swmm = fopen(inputFile, "r");
	if (fp_swmm == NULL) { printf("no inp files for swmm5\n"); exit(0); }
	while (fgets(line, 100, fp_swmm))
	{
		line[strlen(line) - 1] = 0;

		if (strcmp("[COORDINATES]", line) == 0)                    //[COORDINATES]  nodes  outfalls 
		{
			fgets(line, 100, fp_swmm);
			fgets(line, 100, fp_swmm);
			get_pipe_name_swmm(fp_swmm, PS_name, &p_number);
			for (j = 0; j < *numps; j++)        //  check the name of PS (x,y) in inp and in bci files
			{
				for (i = 0; i < p_number; i++)
					if (strcmp((placeholder + j * 80), PS_name + i * 80) == 0) break;//BCptr->PS_NameBCptr->PS_Name
			}
			if (i == p_number) { printf("the node %s in bci file does not exist in inp file\n", (placeholder + j * 80)); exit(0); }


		}
	}        //  while loop     nodes in inp shoud be found in bci file
   //  
	fclose(fp_swmm);
	return p_number;
}
int* Tools::calcCommonPoint(int* c_count, char* PS_name, int p_number, int num) {
	for (int j = 0; j < p_number; j++)
	{
		for (int i = 0; i < num; i++)
		{
			if (strcmp((get_PSName() + i * 80), PS_name + j * 80) == 0)//找到同名点
			{
				c_count[j]++;
			}
		}
	}
	return c_count;
}