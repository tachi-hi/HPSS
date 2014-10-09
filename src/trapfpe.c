#define _GNU_SOURCE 1
#include <fenv.h>
static void __attribute__ ((constructor))
trapfpe ()
{
  feenableexcept (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
}
          
