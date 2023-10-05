// SampleCppLibrary.cpp : Defines the exported functions for the DLL application.
//

#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include "bmi_lisflood.h"
#include "lisflood.h"
#include "global.h"
#include "initialize.h"
#include "update.h"
#include "finalize.h"

// define some arrays for exchange
// list of variables to be potentially exposed via BMI can be found in lisflood.h
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

  ysz,
  xsz,
  allowPonding,
  //steadyCount,
  // lmy end
  // if variable is not found
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

  if (variable == "ysz") return ysz;
  if (variable == "xsz") return xsz;
  if (variable == "allowPonding") return allowPonding;

  //if (variable == "steadyCount") return steadyCount;
  //lmy e
  //if (variable == "FArea") return FArea;
  else return NOVAR;
};
BMI_Function_Lisflood variable2label2(string variable)
{
  if (variable == "initialize_lisflood") return initializeLisflood;

  if (variable == "finalize_lisflood") return finalizeLisflood;
  if (variable == "get_start_time_lisflood") return getStartTimeLisflood;

  if (variable == "get_end_time_lisflood") return getEndTimeLisflood;
  if (variable == "get_current_time_lisflood") return getCurrentTimeLisflood;

  if (variable == "get_time_step_lisflood") return getTimeStepLisflood;
  if (variable == "get_var_shape_lisflood") return getVarShapeLisflood;

  if (variable == "get_var_rank_lisflood") return getVarRankLisflood;
  if (variable == "get_var_type_lisflood") return getVarTypeLisflood;

  if (variable == "get_var_count_lisflood") return getVarCountLisflood;
  if (variable == "get_var_name_lisflood") return getVarNameLisflood;

  if (variable == "get_var_lisflood") return getVarLisflood;
  if (variable == "update_lisflood") return updateLisflood;

  if (variable == "set_var_lisflood") return setVarLlisflood;
  if (variable == "set_var_slice_lisflood") return setVarSliceLisflood;

  if (variable == "throwWaterDepth") return throwWaterDepth2;
  if (variable == "SurfaceRunoffModule") return SurfaceRunoffModule2;
}
BMI_PAR_LISFLOOD par_lisflood2label(string par)
{
  if (par == "inflow") return INFLOW_LISFLOOD_PAR;
  if (par == "floodDepth") return FLOOD_DEPTH_LISFLOOD_PAR;
}
/* Store callback */
//Logger logger = NULL;

/* Logger function */
void _log(Level_lis level, std::string msg);

extern "C" int init(int, const char* []);
extern "C" int init_iterateq();

extern "C" {
  //功能函数，string转double
  double stringToNum(const string& str)
  {
    istringstream iss(str);
    double num;
    iss >> num;
    return num;
  }

  BMI_API int initialize_lisflood(vector<string> config)
  {
    const char* argv[] = {
      "bmi",
      "-v",
      config[0].c_str()
    };
    int result = init(3, argv);
    init_iterateq();
    return result;
  }

  BMI_API int update_lisflood(double dt)
  {
    // set timestep and do:
    double oldStep = Solverptr->Tstep;
    if (dt != -1) {
      Solverptr->Tstep = dt;
    }
    iterateq_step();
    // restore dt to default
    Solverptr->Tstep = oldStep;
    return 0;
  }

  BMI_API int finalize_lisflood()
  {
    final_iterateq();
    final();
    return 0;
  }

  BMI_API void get_start_time_lisflood(double* t)
  {
    *t = 0;
  }

  BMI_API void get_end_time_lisflood(double* t)
  {
    *t = Solverptr->Sim_Time;
  }

  BMI_API void get_current_time_lisflood(double* t)
  {
    *t = Solverptr->t;
  }

  BMI_API double get_time_step_lisflood(double* dt)
  {
    if (dt != nullptr)
    {
      *dt = Solverptr->Tstep;
    }
    return Solverptr->Tstep;
  }

  BMI_API char* get_PSName() {
    return BCptr->PS_Name;
  }
  BMI_API int get_PSNum() {
    return BCptr->numPS;
  }
  BMI_API int* get_BCPSNum()
  {
      return &(BCptr->numPS);
  }
  BMI_API double* get_BCVar(int j) {
    int nbdy = BCptr->PS_Val[j];
    return BCptr->BCVarlist[nbdy];
  };
  BMI_API double* get_dx() {
    return Arrptr->dx;
  };

  BMI_API int get_gird_x(int j) {
    return BCptr->xpi[j];
  };
  BMI_API int get_gird_y(int j) {
    return BCptr->ypi[j];
  }
  BMI_API double get_waterdepth(int row, int col)
  {
    return Arrptr->H[row + col * (Parptr->xsz)];
  }
  BMI_API double get_elevation(int location)
  {
    return Arrptr->DEM[location];
  }
  BMI_API int get_river_gird(char* PSNAME)
  {
    int temp = 0;
    for (int i = 0; i < ChannelSegmentsVecPtr->size(); i++)
    {
      ChannelSegmentType* tmp_chan = &(ChannelSegmentsVecPtr->front()) + i;
      for (int i = 0; i < tmp_chan->chsz; i++) {
        if (strcmp((tmp_chan->Q_Name + i * 80), PSNAME) == 0) {
          int pxi = tmp_chan->ChanX[tmp_chan->chsz - 1];
          int pyi = tmp_chan->ChanY[tmp_chan->chsz - 1];
          temp = tmp_chan->ChanX[i] + tmp_chan->ChanY[i] * Parptr->xsz;
          break;
        }
      }
    }
    return temp;
  }
  BMI_API void set_inflow_lisflood(int j, double value)
  {
	  double* data = get_BCVar(j);   // GET BCVAR
	  int k = 0;
	  while (data[k] != -1)//change Q
	  {
		  if (k % 2 == 0) {
			  data[k] = value;// overflow  into LISFLOOD unit m^2/s
		  }
		  k++;
	  }
  }
}
