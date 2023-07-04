
#include <cstdio>
#include <string>
#include <sstream>
//#include <string>
//#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "bmi_swmm.h"

extern "C" {
    #include "headers.h"
    #include "swmm5.h"
}


using namespace std;


enum VARIABLE_LABEL {
    H,
    DEM,
    Qx,
    Qy,
    rain,
    dx,
    dx_sqrt,
    dy,
    dA,
    SGCwidth,
    SGCQin,
    SGCz,
    blx,
    bly,
    BC_numPS,
    BC_xpi,
    BC_ypi,
    QxSGold,
    QySGold,

    //lmy start
    Sim_Time,
    killsim,
    itrn_time_now,
    killsim_time,
    steadycheck,
    t,
    steadyTotal,
    //steadyCount,
    // lmy end
    // if variable is not found
    elapsed_time,
    NOVAR
};

VARIABLE_LABEL variable2label(std::string const& variable) {
    if (variable == "H") return H;
    if (variable == "DEM") return DEM;
    if (variable == "Qx") return Qx;
    if (variable == "Qy") return Qy;
    if (variable == "rain") return rain;
    if (variable == "dx") return dx;
    if (variable == "dx_sqrt") return dx_sqrt;
    if (variable == "dy") return dy;
    if (variable == "dA") return dA;
    if (variable == "SGCwidth") return SGCwidth;
    if (variable == "SGCQin") return SGCQin;
    if (variable == "SGCz") return SGCz;
    if (variable == "blx") return blx;
    if (variable == "bly") return bly;
    if (variable == "BC.numPS") return BC_numPS;
    if (variable == "BC.xpi") return BC_xpi;
    if (variable == "BC.ypi") return BC_ypi;
    if (variable == "QxSGold") return QxSGold;
    if (variable == "QySGold") return QySGold;
    //lmy s
    if (variable == "Sim_Time") return Sim_Time;
    if (variable == "killsim") return killsim;
    if (variable == "itrn_time_now") return itrn_time_now;
    if (variable == "killsim_time") return killsim_time;
    if (variable == "steadycheck") return steadycheck;
    if (variable == "t") return t;
    if (variable == "steadyTotal") return steadyTotal;

    if (variable == "elapsed_time") return elapsed_time;
    //if (variable == "steadyCount") return steadyCount;
    //lmy e
    //if (variable == "FArea") return FArea;
    else return NOVAR;
};

/* Store callback */
//Logger logger = NULL;

/* Logger function */
//void _log(Level_swmm level, std::string msg);

extern "C" int init(int, const char* []);
extern "C" int init_iterateq();

//功能函数，string转double
double stringToNum(const string& str)
{
    istringstream iss(str);
    double num;
    iss >> num;
    return num;
}

void trim(string & s)
{
    int index = 0;
    if (!s.empty())
    {
        while ((index = s.find(' ', index)) != string::npos)
        {
            s.erase(index, 1);
        }
    }    
}


int BMI_API  initialize_swmm(vector<string> config_file)
{
    int result = swmm_open(config_file[0].c_str(), config_file[1].c_str(), config_file[2].c_str());
    swmm_start(1);
    return result;
}

int BMI_API update_swmm(double dt)
{
    // set timestep and do:
    double oldStep = dt;
    //if (dt != -1) {
    //    Solverptr->Tstep = dt;
    //}
    //iterateq_step();
    //// restore dt to default
    //Solverptr->Tstep = oldStep;
    swmm_step(&oldStep);
    dt = oldStep;
    ElapsedTime = oldStep;
    return 1;
}

int BMI_API  finalize_swmm()
{
    swmm_end();
    swmm_report();
    swmm_close();
    return 0;
}
double BMI_API get_current_time_swmm(double *dt)
{
    if (dt != nullptr)
    {
        *dt = ElapsedTime;
    }
    return ElapsedTime;
}

void BMI_API get_pipe_name_swmm(FILE* fp_swmm, char*& PS_name, int* p_number)
{
    float  xc, yc;
    int i = 0;
    for (i = 0; i < 100000; i++)     //  NOT p_number
    {
        fscanf(fp_swmm, " %s %f  %f ", PS_name + i * 80, &xc, &yc);
        if (strcmp("[VERTICES]", PS_name + i * 80) == 0)  break;          // may changed according to inp file         
    }
    *p_number = i;
}

void BMI_API set_inflow_swmm(double* a) {
    int     j, p;
    double  q, w;
    TExtInflow* inflow;
    DateTime currentDate = getDateTime(NewRoutingTime);    
    TTableEntry* P;
    int type;
    int tseries = -1;
    // --- for each node with a defined external inflow
    for (j = 0; j < Nobjects[NODE]; j++)
    {
        inflow = Node[j].extInflow;
        if (!inflow) continue;

        // --- get flow inflow
        q = 0.0;
        while (inflow)
        {
            type = inflow->type;
            tseries = inflow->tSeries;
            if (inflow->type == FLOW_INFLOW)
            {
                double x = 0;
                double y = 0;
                table_getFirstEntry(&Tseries[tseries], &x, &y);
                Tseries[tseries].thisEntry->y = a[j];
                while (table_getNextEntry(&Tseries[tseries], &x, &y))
                {
                    Tseries[tseries].thisEntry->y = a[j];
                    
                }
                table_tseriesInit(&Tseries[tseries]);
                q = inflow_getExtInflow(inflow, currentDate);
                break;
            }
            else inflow = inflow->next;
        }
        if (fabs(q) < FLOW_TOL) q = 0.0;

        // --- add flow inflow to node's lateral inflow
        Node[j].newLatFlow += q;
        /*massbal_addInflowFlow(EXTERNAL_INFLOW, q);*/

        // --- add on any inflow (i.e., reverse flow) through an outfall
        if (Node[j].type == OUTFALL && Node[j].oldNetInflow < 0.0)
        {
            q = q - Node[j].oldNetInflow;
        }    
    }
}

void BMI_API get_overflow_swmm( double* b)
{
    int j;
    for (j = 0; j < Nobjects[NODE]; j++)
    {
        b[j] = Node[j].overflow * UCF(FLOW);
    }
}
