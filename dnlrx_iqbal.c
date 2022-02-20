#define S_FUNCTION_NAME  dnlrx_iqbal
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"
#include "stdlib.h"
#include "math.h"

#define U(element) (*uPtrs[element]) 

static real_T r[1][7] = {{8.69894689978302e-05,-6.44524084132453e-05,2.91469955823579e-05,-1.40193609776909e-05,1.80459333318332e-05,-3.77841659869284e-05,4.89208227288781e-05} };

static real_T theda[1][7] = {{0.00831922072208818,-1.71067880823788e-05,0.000927523218803022,0.00423876177844888,6.18211852463252e-05}};

static real_T d[5] = {0.315718317376219,47.9078860852233,7.36430476306737,1.52883083515932,111.107636334345 };

static real_T delta[7] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

static void mdlInitializeSizes(SimStruct* S)
{
    ssSetNumSFcnParams(S, 0);
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 12);

    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 1);

    ssSetInputPortDirectFeedThrough(S, 0, 1);
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}

static void mdlInitializeSampleTimes(SimStruct* S)
{
    ssSetSampleTime(S, 0, 0);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}
#define MDL_INITIALIZE_CONDITIONS

static void mdlInitializeConditions(SimStruct* S)
{
    real_T* x0 = ssGetRealDiscStates(S);
    int_T  lp;

    for (lp = 0; lp < 12; lp++) {
        *x0++ = 0.0;
    }
}

static void mdlOutputs(SimStruct* S, int_T tid)
{
    int_T    i;
    real_T   tmp;
    real_T   sgn;
    real_T               *x = ssGetRealDiscStates(S);
    real_T               *y = ssGetOutputPortRealSignal(S, 0);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S, 0);

    y[0] = *uPtrs[0];
    for (i = 0; i < 5; i++)
    {
        tmp = U(0) - x[0] + x[7 + i];
        if (tmp < 0)
        {
            sgn = -1.0;
            tmp = -1.0 * tmp;
        }
        else
        {
            sgn = 1.0;
        }
        if (tmp <= d[i])
        {
            delta[i] = sgn * tmp;
        }
        else
        {
            delta[i] = sgn * d[i];
        }
    }
    y[0] = r[0][0] * U(0) + r[0][0] * x[0] + r[0][1] * x[1] + r[0][2] * x[2] + r[0][3] * x[3] + r[0][4] * x[4] + r[0][5] * x[5] + r[0][6] * x[6];
    y[0] = y[0] + theda[0][0] * delta[0] + theda[0][1] * delta[1] + theda[0][2] * delta[2] + theda[0][3] * delta[3];
    y[0] = y[0] + theda[0][4] * delta[4] + 0.007903672855934;
}
#define MDL_UPDATE

static void mdlUpdate(SimStruct* S, int_T tid)
{
    real_T *x = ssGetRealDiscStates(S);
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs(S, 0);
    x[0] = U(0);
    x[1] = x[0];
    x[2] = x[1];
    x[3] = x[2];
    x[4] = x[3];
    x[5] = x[4];
    x[6] = x[5];
    x[7] = delta[0];
    x[8] = delta[1];
    x[9] = delta[2];
    x[10] = delta[3];
    x[11] = delta[4];
}
static void mdlTerminate(SimStruct* S)
{
    UNUSED_ARG(S);
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
