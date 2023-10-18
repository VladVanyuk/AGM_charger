#include "includes.h"


struct stage_flags
{
    bool flag_stage_1 : 1; // bulk charge at max current 16-17A up to 80%
    bool flag_stage_2 : 1; // absorbtion charge 5-6Afrom 80% to 99%
    bool flag_stage_3 : 1; // topping charge 99%-100% at minimum current
    bool stop_flag    : 1;    // charging disabled
};

stage_flags STAGE_FLAGS = {};

struct flags
{
    bool AC = false;  // 230 vac
    bool CSR = false; // charge service routine
}flags;


typedef enum {
   STOP,
   STAGE_1,
   STAGE_2,
   STAGE_3
}current_stage_t;

current_stage_t current_stage =  STOP;