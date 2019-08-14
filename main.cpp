#include <iostream>
//#include "automat_0.h"
//#include "automat_1.h"
#include "step_automat.h"
#include <time.h>
#include <unistd.h>
#include "boiler.h"
#include <functional>


using namespace std;
int s = 8;



int main()
{
    setlocale(LC_ALL, "Russian");
    ShipBoiler sb;
    Automat<ShipBoiler, StateStack, state_aux_boiler::d> boiler_automat;


    boiler_automat.add(state_aux_boiler::ss_start_boiler_HeavyFuel, auto_start_boiler_HeavyFuel, &sb);
    boiler_automat.add(state_aux_boiler::ss_start_boiler_LightFuel, auto_start_boiler_LightFuel, &sb);
    boiler_automat.add(state_aux_boiler::ss_start_boiler_HeavyFuel_pump, auto_start_boiler_HeavyFuel_pump, &sb);
    boiler_automat.add(state_aux_boiler::ss_start_boiler_LightFuel_pump, auto_start_boiler_LightFuel_pump, &sb);
    boiler_automat.add(state_aux_boiler::ss_stop_boiler_HeavyFuel_pump, auto_stop_boiler_HeavyFuel_pump, &sb);
    boiler_automat.add(state_aux_boiler::ss_stop_boiler_LightFuel_pump, auto_stop_boiler_LightFuel_pump, &sb);
    boiler_automat.add(state_aux_boiler::ss_start_second_AE, auto_start_second_AE, &sb);
    boiler_automat.add(state_aux_boiler::ss_stop_second_AE, auto_stop_second_AE, &sb);
    boiler_automat.add(state_aux_boiler::ss_start_boiler_FUN, auto_start_boiler_FUN, &sb);
    boiler_automat.add(state_aux_boiler::ss_stop_boiler_FUN, auto_stop_boiler_FUN, &sb);
    boiler_automat.add(state_aux_boiler::ss_shiber_open, auto_shiber_open, &sb);
    boiler_automat.add(state_aux_boiler::ss_shiber_close, auto_shiber_close, &sb);
    boiler_automat.add(state_aux_boiler::ss_start_ignition, auto_start_ignition, &sb);
    boiler_automat.add(state_aux_boiler::ss_stop_ignition, auto_stop_ignition, &sb);
    boiler_automat.add(state_aux_boiler::ss_open_LightFuel_vv, auto_open_LightFuel_vv, &sb);
    boiler_automat.add(state_aux_boiler::ss_close_LightFuel_vv, auto_close_LightFuel_vv, &sb);
    boiler_automat.add(state_aux_boiler::ss_open_HeavyFuel_vv, auto_open_HeavyFuel_vv, &sb);
    boiler_automat.add(state_aux_boiler::ss_close_HeavyFuel_vv, auto_close_HeavyFuel_vv, &sb, std::ref(s));
    boiler_automat.add(state_aux_boiler::ss_stop_boiler, auto_stop_boiler, &sb);
    boiler_automat.add(state_aux_boiler::ss_error, auto_ERROR, &sb);


    boiler_automat.exec(state_aux_boiler::ss_start_boiler_HeavyFuel, PRIOR_STORAGE);

    for (int i = 0; i < 30; ++i)
    {
        sleep(1);
//        if (i == 5)
//            boiler_automat.pushState(state_aux_boiler::ss_stop_boiler, PRIOR_STORAGE); // пример получения команды извне... в процессе выполнения
        boiler_automat.exec();
    }
    return 0;
}

