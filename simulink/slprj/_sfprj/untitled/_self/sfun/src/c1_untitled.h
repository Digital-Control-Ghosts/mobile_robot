#ifndef __c1_untitled_h__
#define __c1_untitled_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc1_untitledInstanceStruct
#define typedef_SFc1_untitledInstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c1_sfEvent;
  boolean_T c1_isStable;
  boolean_T c1_doneDoubleBufferReInit;
  uint8_T c1_is_active_c1_untitled;
  real_T *c1_v;
  real_T *c1_vr;
  real_T *c1_w;
  real_T *c1_vl;
} SFc1_untitledInstanceStruct;

#endif                                 /*typedef_SFc1_untitledInstanceStruct*/

/* Named Constants */

/* Variable Declarations */
extern struct SfDebugInstanceStruct *sfGlobalDebugInstanceStruct;

/* Variable Definitions */

/* Function Declarations */
extern const mxArray *sf_c1_untitled_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c1_untitled_get_check_sum(mxArray *plhs[]);
extern void c1_untitled_method_dispatcher(SimStruct *S, int_T method, void *data);

#endif
