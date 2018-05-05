#ifndef __c1_model_simulink_h__
#define __c1_model_simulink_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc1_model_simulinkInstanceStruct
#define typedef_SFc1_model_simulinkInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c1_sfEvent;
  boolean_T c1_isStable;
  boolean_T c1_doneDoubleBufferReInit;
  uint8_T c1_is_active_c1_model_simulink;
  real_T *c1_v;
  real_T *c1_VR;
  real_T *c1_w;
  real_T *c1_VL;
} SFc1_model_simulinkInstanceStruct;

#endif                                 /*typedef_SFc1_model_simulinkInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c1_model_simulink_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c1_model_simulink_get_check_sum(mxArray *plhs[]);
extern void c1_model_simulink_method_dispatcher(SimStruct *S, int_T method, void
  *data);

#endif
