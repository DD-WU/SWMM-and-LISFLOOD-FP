/* -*- c-file-style: "stroustrup" -*- */
/* Please use the stroustrup coding standard: */


#ifndef BMI_LISFLOOD_API_H
#define BMI_LISFLOOD_API_H

#define BMI_API_VERSION_MAJOR 1
#define BMI_API_VERSION_MINOR 0

#if defined _WIN32
#define BMI_API __declspec(dllexport)
/* Calling convention, stdcall in windows, cdecl in the rest of the world */
#define CALLCONV __stdcall
#else
#define BMI_API
#define CALLCONV
#endif


#define MAXSTRINGLEN 1024
#define MAXDIMS 6
#include <stddef.h>
#include <vector>
#include <string>
using std::vector;
using std::string;
typedef enum {
  //ALL,
  /*DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL*/
} Level_lis;
//对BMI的补充，优化，符合城市雨洪的模型耦合优化
enum BMI_Function_Lisflood
{
  initializeLisflood,

  finalizeLisflood,
  getStartTimeLisflood,

  getEndTimeLisflood,
  getCurrentTimeLisflood,

  getTimeStepLisflood,
  getVarShapeLisflood,

  getVarRankLisflood,
  getVarTypeLisflood,

  getVarCountLisflood,
  getVarNameLisflood,

  getVarLisflood,
  updateLisflood,

  setVarLlisflood,
  setVarSliceLisflood,

  throwWaterDepth2,
  SurfaceRunoffModule2,
};

BMI_Function_Lisflood variable2label2(string variable);

enum BMI_PAR_LISFLOOD
{
  INFLOW_LISFLOOD_PAR,
  FLOOD_DEPTH_LISFLOOD_PAR,
};

BMI_PAR_LISFLOOD par_lisflood2label(string par);


#ifdef __cplusplus
extern "C" {
#endif
  /* control functions. These return an error code. */
  BMI_API int initialize_lisflood(vector<string> config_file);
  BMI_API int update_lisflood(double dt);
  BMI_API int finalize_lisflood();

  /* time control functions */
  BMI_API void get_start_time_lisflood(double* t);//get_start_time
  BMI_API void get_end_time_lisflood(double* t);//get_end_time
  BMI_API void get_current_time_lisflood(double* t);//get_current_time
  BMI_API double get_time_step_lisflood(double* dt);//get_time_step
   
  /* spatial control functions */
  BMI_API int get_gird_x(int j);
  BMI_API int get_gird_y(int j);

  //data exchange control functions
  BMI_API double get_waterdepth(int row, int col);
  BMI_API void set_inflow_lisflood(int j, double value);
  BMI_API double* get_BCVar(int j);
  BMI_API double* get_dx();
  BMI_API char* get_PSName();
  BMI_API int get_PSNum();
  BMI_API int* get_BCPSNum();
#ifdef __cplusplus
}

#endif

#endif
