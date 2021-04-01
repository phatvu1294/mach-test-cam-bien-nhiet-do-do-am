#include <math.h>
#include "ntc.h"

float NTC_getTemp(long resistance25, int beta, long resistance) {
  float t;
  t = 1 / (((float)log((float)resistance / resistance25) / beta) + (1 / 298.15));
  t = t - 273.15;
  return t;
}
