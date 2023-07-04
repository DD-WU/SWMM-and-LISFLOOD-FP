/* -*- c-file-style: "stroustrup" -*- */
/* Please use the stroustrup coding standard: */


#ifndef BMI_SWMM_API_H
#define BMI_SWMM_API_H

#define BMI_API_VERSION_MAJOR 1
#define BMI_API_VERSION_MINOR 0

#if defined _WIN32
#define BMI_API __declspec(dllexport) __stdcall
/* Calling convention, stdcall in windows, cdecl in the rest of the world */
#define CALLCONV __stdcall
#else
#define BMI_API
#define CALLCONV
#endif


#define MAXSTRINGLEN 1024
#define MAXDIMS 6
#include <stddef.h>
#include <string>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;

#ifdef __cplusplus
extern "C" {
#endif


    /* process interface.*/
    int BMI_API  initialize_swmm(vector<string> config_file);
    int BMI_API  update_swmm(double dt);
    int BMI_API  finalize_swmm();


    /* data interface */
    double BMI_API  get_current_time_swmm(double *t);
    void BMI_API set_inflow_swmm(double* a);
    void BMI_API get_overflow_swmm(double* b);
    void BMI_API get_pipe_name_swmm(FILE* fp_swmm, char*& PS_name, int* p_number);

#ifdef __cplusplus
}
#endif


#endif
