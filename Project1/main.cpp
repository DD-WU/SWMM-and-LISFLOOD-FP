#include <stdio.h>
#include<iostream>
#include <conio.h>
#include <math.h>
#include <map>
#include "bmi_swmm.h"
#include "bmi_lisflood.h"
#include"consts.h"
#include"space.h"
#include"swmm5.h"
#include"Tools.h"
#include <sstream>
int main() {
//[initialize]
	// set path and read file
	string swmm_inp = "F:\\SWMM_LISFLOOD0622\\Project1\\test.inp";
	string swmm_out = "F:\\SWMM_LISFLOOD0622\\Project1\\test.rpt";
	string swmm_rpt = "F:\\SWMM_LISFLOOD0622\\Project1\\test.out";
	string lisflood_conf = "F:\\SWMM_LISFLOOD0622\\Project1\\dem.par";
	vector<string> argv_lisflood;
	argv_lisflood.push_back(lisflood_conf);
	vector<string> argv_swmm;
	argv_swmm.push_back(swmm_inp);
	argv_swmm.push_back(swmm_out);
	argv_swmm.push_back(swmm_rpt);
	//INIT_MODEL
	initialize_swmm(argv_swmm);
	initialize_lisflood(argv_lisflood);
//[spatial and time]
	//read lisflood time
	double endtime = 0, starttime = 0, initstep = 0, t = 0;
	get_end_time_lisflood(&endtime);
	get_start_time_lisflood(&starttime);
	get_time_step_lisflood(&initstep);
	get_current_time_lisflood(&t);

	//read swmm time
	double ElapsedTime = 0, time = 0;
	get_current_time_swmm(&ElapsedTime);
	time = ElapsedTime * MSECperDAY / 1000;
	bool start_flag = true;
	int i, j, k, ix, iy;
	char* PS_name = NULL;

	//read swmm pipe node
	int p_number = Tools::readCommonPoint(
		"F:\\SWMM_LISFLOOD0622\\Project1\\test.inp",
		PS_name); 

	//read lisflood pipe node
	int num = get_PSNum();
	double* dx_ = get_dx();//get dx 
	double* p_val = new double[p_number] {0};//overflow 
	double* q_in = new double[p_number] {0};//inflow	

//[LOOP]
	while ((start_flag||ElapsedTime > 0) && t <= endtime)
	{
		start_flag = false;
		if (abs(time - t)==0) {
			//same time 
			std::cout << "time:" << t << "\n";
//[Data exchange]	
			for (i = 0; i < p_number; i++) p_val[i] = 0;
			set_inflow_swmm(q_in);
			get_overflow_swmm(p_val);


			for (j = 0; j < num; j++)
				for (i = 0; i < p_number; i++)
					// overflow to LISFLOOD-FP
					if (strcmp((get_PSName() + j * 80),
						PS_name + i * 80) == 0)
						//same point
					{
						set_inflow_lisflood(j, p_val[i] / (*dx_) / (*dx_));
					}


			double q_j = 0;
			for (i = 0; i < p_number; i++) q_in[i] = 0;
			for (j = 0; j < num; j++)
				for (i = 0; i < p_number; i++)
					// get SWMM inflow
					if (strcmp((get_PSName() + j * 80),
						PS_name + i * 80) == 0)
					{
					
						//get position
						ix = get_gird_x(j);
						iy = get_gird_y(j);
						q_j = Tools::waterDepthToInflow(get_waterdepth(ix, iy), 0.4, 0.75) ; //  leaving LISFLOOD  
						q_in[i] = q_in[i] + q_j; // into SWMM 
						if (p_val[i] > 0)
						{
							q_in[i] = 0;
						}
						else
						{
							set_inflow_lisflood(j, -q_j);
						}
					}
		}

//[Update]	
		// update model
		update_swmm(ElapsedTime);
		update_lisflood(initstep);
		// get time
		get_time_step_lisflood(&initstep);
		get_current_time_lisflood(&t);
		get_current_time_swmm(&ElapsedTime);
		time = ElapsedTime * MSECperDAY / 1000;
	};
	while (ElapsedTime > 0)
	{
		update_swmm(ElapsedTime);
		get_current_time_swmm(&ElapsedTime);
		time = ElapsedTime * MSECperDAY / 1000;
	}
	while (t < endtime)
	{
		update_lisflood(initstep);
		get_current_time_lisflood(&t);
	}
//[finialize]	
	finalize_swmm();
	finalize_lisflood();
}