/*
 * File: test1_initialize.c
 *
 * MATLAB Coder version            : 2.6
 * C/C++ source code generated on  : 24-Feb-2016 16:09:40
 */

/* Include files */
#include "rt_nonfinite.h"
#include "test1.h"
#include "test1_initialize.h"
#include "eml_rand_shr3cong_stateful.h"
#include "eml_rand_mcg16807_stateful.h"
#include "eml_rand.h"
#include "eml_rand_mt19937ar_stateful.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void test1_initialize(void)
{
  rt_InitInfAndNaN(8U);
  state_not_empty_init();
  eml_rand_init();
  eml_rand_mcg16807_stateful_init();
  eml_rand_shr3cong_stateful_init();
}

/*
 * File trailer for test1_initialize.c
 *
 * [EOF]
 */
