#ifndef __c3_model_simulink_h__
#define __c3_model_simulink_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc3_model_simulinkInstanceStruct
#define typedef_SFc3_model_simulinkInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c3_sfEvent;
  boolean_T c3_isStable;
  boolean_T c3_doneDoubleBufferReInit;
  uint8_T c3_is_active_c3_model_simulink;
  real_T *c3_VR;
  real_T *c3_Ydot;
  real_T *c3_VL;
  real_T *c3_Xdot;
  real_T *c3_w;
  real_T *c3_Theta;
} SFc3_model_simulinkInstanceStruct;

#endif                                 /*typedef_SFc3_model_simulinkInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c3_model_simulink_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c3_model_simulink_get_check_sum(mxArray *plhs[]);
extern void c3_model_simulink_method_dispatcher(SimStruct *S, int_T method, void
  *data);

#endif
