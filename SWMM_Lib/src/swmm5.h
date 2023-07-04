//-----------------------------------------------------------------------------
//   swmm5.h
//
//   Project: EPA SWMM5
//   Version: 5.1
//   Date:    03/24/14  (Build 5.1.001)
//            08/01/16  (Build 5.1.011)
//   Author:  L. Rossman
//
//   Prototypes for SWMM5 functions exported to swmm5.dll.
//
//-----------------------------------------------------------------------------

#ifndef SWMM5_H
#define SWMM5_H


// --- define WINDOWS

#undef WINDOWS
#ifdef _WIN32
  #define WINDOWS
#endif
#ifdef __WIN32__
  #define WINDOWS
#endif

// --- define DLLEXPORT

#ifdef WINDOWS
    #define MiddleWare __declspec(dllexport) __stdcall
    #define DLLEXPORT __declspec(dllexport) //__stdcall
#else
    #define MiddleWare
#endif

// --- use "C" linkage for C++ programs

#ifdef __cplusplus
extern "C" {
#endif

int  MiddleWare   swmm_run(char* f1, char* f2, char* f3);
int  MiddleWare   swmm_open(const char* f1,const char* f2,const char* f3);
int  MiddleWare   swmm_start(int saveFlag);
int  MiddleWare   swmm_step(double* elapsedTime);
int  MiddleWare   swmm_end(void);
int  MiddleWare   swmm_report(void);
int  MiddleWare   swmm_getMassBalErr(float* runoffErr, float* flowErr,
                 float* qualErr);
int  MiddleWare   swmm_close(void);
int  MiddleWare   swmm_getVersion(void);
int  MiddleWare   swmm_getError(char* errMsg, int msgLen);
int  MiddleWare   swmm_getWarnings(void);

int MiddleWare swmm_getCouplePoints(double blx, double bly, int row, int col, double delta,
int* indexs, int* rows, int* cols, double* cqAs, double* weirBs, double* pondedA); 
int MiddleWare swmm_getCouplePointsN(double blx, double bly, int row, int col, double delta);                                                              //MZ-202007
//int  SWMM_EXPORT   swmm_getSWMMSimTime(void);//use after swmm_start                                                                                            //MZ-202007
double MiddleWare swmm_getOverflow(int);
int MiddleWare InflowToLatFlow(int index, double q);
//double SWMM_EXPORT getOverflowByIdx(int index);

//int  SWMM_EXPORT   swmm_setOption_allowPonding(int flag);                                                                                                      //MZ-202007
//double SWMM_EXPORT swmm_getNodeHead(int index, int* flag);																									   //MZ-202007
//double SWMM_EXPORT routing_getRoutingStep(int routingModel, double fixedStep);
//void SWMM_EXPORT swmm_nodeFlood(char*);
int MiddleWare swmm_getCount();
double MiddleWare get_coordinate_x(int);
double MiddleWare get_coordinate_y(int);

#ifdef __cplusplus
}   // matches the linkage specification from above */
#endif


#endif //SWMM5_H
