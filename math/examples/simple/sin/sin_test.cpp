// ©2022 Microchip Technology Inc. and its subsidiaries
//
// Subject to your compliance with these terms, you may use this Microchip
// software and any derivatives exclusively with Microchip products. You are
// responsible for complying with third party license terms applicable to your
// use of third party software (including open source software) that may
// accompany this Microchip software. SOFTWARE IS “AS IS.” NO WARRANTIES,
// WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING
// ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR
// A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY
// INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST
// OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED,
// EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
// FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP’S TOTAL
// LIABILITY ON ALL CLAIMS LATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF
// FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. MICROCHIP
// OFFERS NO SUPPORT FOR THE SOFTWARE. YOU MAY CONTACT MICROCHIP AT
// https://www.microchip.com/en-us/support-and-training/design-help/client-support-services
// TO INQUIRE ABOUT SUPPORT SERVICES AND APPLICABLE FEES, IF AVAILABLE.

#include "math/examples/test_utils.hpp"
#include "math/examples/simple/configs.hpp"
#include "math/include/hls_sin.hpp"

#define THRESHOLD 1
using namespace hls::math;

/* Wrappers */
ap_fixpt<M_W, M_IW> sin_lut_M_wrapper(ap_fixpt<M_W, M_IW> x){
#pragma HLS function pipeline top
  return sin_lut<M_W, M_IW>(x);
}

ap_fixpt<M_W, M_IW> sin_cordic_M_wrapper(ap_fixpt<M_W, M_IW> x){
#pragma HLS function pipeline top
  return sin_cordic<M_W, M_IW, M_N_ITER>(x);
}

ap_fixpt<M_W, M_IW> sin_taylor_M_wrapper(ap_fixpt<M_W, M_IW> x){
#pragma HLS function pipeline top
  return sin_taylor<M_W, M_IW>(x);
}

double sin_cmath_wrapper(double x){
#pragma HLS function top
  return sin(x);
}


int test(double start_at, double limit, double delta, unsigned int W, int IW, int N_ITER = -1, int graph = 0, int report = 0) {
  double max_diff_cordic = 0, avg_diff_cordic = 0, actual_cordic = 0, diff_cordic = 0;
  double max_diff_taylor = 0, avg_diff_taylor = 0, actual_taylor = 0, diff_taylor = 0;
  double max_diff_lut = 0, avg_diff_lut = 0, actual_lut = 0, diff_lut = 0;
  int count = 0;
  std::string test_name = find_test_name("sin", W, IW, start_at, limit, N_ITER);

  FILE* fp = fopen(test_name.c_str(), "w");
  if (graph) fprintf(fp, "# PLOT ");
  if (report) fprintf(fp, "# REPORT ");
  fprintf(fp, "\n# from %f to %f at intervals of %f, using W: %d, IW: %d\n", start_at, limit, delta, W, IW);
  fprintf(fp, "# x, expected, sin_cordic, sin_taylor, sin_lut, sin_cordic diff, sin_taylor diff, sin_lut diff\n");

  printf("From %f to %f at intervals of %f, using W: %d, IW: %d\n", start_at, limit, delta, W, IW);

  for (double x = start_at; x <= limit; x += delta) {
    ap_fixpt<M_W, M_IW> x_fixpt = x;

    double expect;
    expect = sin_cmath_wrapper(x);
 
    actual_cordic = sin_cordic_M_wrapper(x_fixpt).to_double();
    diff_cordic = fabs(expect - actual_cordic);
    if (diff_cordic > max_diff_cordic)
      max_diff_cordic = diff_cordic;
    avg_diff_cordic += diff_cordic;

    actual_taylor = sin_taylor_M_wrapper(x_fixpt).to_double();
    diff_taylor = fabs(expect - actual_taylor);
    if (diff_taylor > max_diff_taylor)
      max_diff_taylor = diff_taylor;
    avg_diff_taylor += diff_taylor;

    actual_lut = sin_lut_M_wrapper(x_fixpt).to_double();
    diff_lut = fabs(expect - actual_lut);
    if (diff_lut > max_diff_lut)
      max_diff_lut = diff_lut;
    avg_diff_lut += diff_lut;
    count++;

    fprintf(fp, "%f,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", x, expect, actual_cordic, actual_taylor, actual_lut, diff_cordic, diff_taylor, diff_lut);
  }

  avg_diff_cordic /= count;
  avg_diff_taylor /= count;
  avg_diff_lut /= count;

  fprintf(fp, "# sin_cmath: Count: %d\tMax error: 0.000000\tAvg error: 0.000000\n", count);
  fprintf(fp, "# sin_cordic: Count: %d\tMax error: %lf\tAvg error: %lf\n", count, max_diff_cordic, avg_diff_cordic);
  fprintf(fp, "# sin_taylor: Count: %d\tMax error: %lf\tAvg error: %lf\n", count, max_diff_taylor, avg_diff_taylor);
  fprintf(fp, "# sin_lut: Count: %d\tMax error: %lf\tAvg error: %lf\n", count, max_diff_lut, avg_diff_lut);

  printf("sin_cmath: Count: %d\tMax error: 0.000000\tAvg error: 0.000000\n", count);
  printf("sin_cordic: Count: %d\tMax error: %lf\tAvg error: %lf\n", count, max_diff_cordic, avg_diff_cordic);
  printf("sin_taylor: Count: %d\tMax error: %lf\tAvg error: %lf\n", count, max_diff_taylor, avg_diff_taylor);
  printf("sin_lut: Count: %d\tMax error: %lf\tAvg error: %lf\n", count, max_diff_lut, avg_diff_lut);
  fclose(fp);

  if (avg_diff_cordic > THRESHOLD || avg_diff_taylor > THRESHOLD || avg_diff_lut > THRESHOLD)
    return 1;
  else
    return 0;
}

int main() {

  create_dir("sin_reports");

  double start;
  double limit;
  double delta;
  int RC = 0;

  start = -2 * M_PI; limit = 2 * M_PI;  delta = (0.1);
  RC |= test(start, limit, delta, M_W, M_IW, M_N_ITER, 1);
  // Medium Test
  start = -4 * M_PI; limit = 4 * M_PI;  delta = (0.1);
  RC |= test(start, limit, delta, M_W, M_IW, M_N_ITER, 0, 1); 

  if (RC != 0)  printf("Errors may have occurred. Please double-check usage.\n");
  return RC;
}

