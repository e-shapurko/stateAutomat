#ifndef BOILER_H
#define BOILER_H

#include <iostream>
#include <unistd.h>
#include <time.h>

using namespace std;


class state_aux_boiler
{
public:
    enum d {
        noop = -1,
        ss_error,
        ss_start_boiler_HeavyFuel,
        ss_start_boiler_LightFuel,
        ss_stop_boiler,
        ss_start_boiler_HeavyFuel_pump,
        ss_start_boiler_LightFuel_pump,
        ss_stop_boiler_HeavyFuel_pump,
        ss_stop_boiler_LightFuel_pump,
        ss_start_second_AE,
        ss_stop_second_AE,
        ss_start_boiler_FUN,
        ss_stop_boiler_FUN,
        ss_shiber_open,
        ss_shiber_close,
        ss_start_ignition,
        ss_stop_ignition,
        ss_open_LightFuel_vv,
        ss_close_LightFuel_vv,
        ss_open_HeavyFuel_vv,
        ss_close_HeavyFuel_vv
    };
};




class ShipBoiler
{
private:
    bool BoilerRuning = false;
    bool is_boiler_HeavyFuel_pump_run = false;
    bool is_boiler_LightFuel_pump_run = false;
    bool is_second_AE = false;
    bool is_boiler_FUN = false;
    bool is_shiber_open = false;
    bool is_ignition = false;
    bool is_open_LightFuel_vv = false;
    bool is_open_HeavyFuel_vv = false;
    int HF_Oil_temp = 120;
    int HF_Oil_pressure = 1;
    int Water_temp = 23;
    int Steam_pressure = 1;
    int TimerIgnitor = 0;
    bool FireDetected = false;
    bool HF_mode = false;
public:
    bool get_BoilerRuning() const
    {
        return BoilerRuning;
    }

    bool get_is_boiler_HeavyFuel_pump_run() const
    {
        return is_boiler_HeavyFuel_pump_run;
    }

    bool get_is_boiler_LightFuel_pump_run() const
    {
        return is_boiler_LightFuel_pump_run;
    }

    bool get_is_second_AE() const
    {
        return is_second_AE;
    }

    bool get_is_boiler_FUN() const
    {
        return is_boiler_FUN;
    }

    bool get_is_shiber_open() const
    {
        return is_shiber_open;
    }

    bool get_is_ignition() const
    {
        return is_ignition;
    }

    bool get_is_open_LightFuel_vv() const
    {
        return is_open_LightFuel_vv;
    }

    bool get_is_open_HeavyFuel_vv() const
    {
        return is_open_HeavyFuel_vv;
    }

    int get_HF_Oil_temp() const
    {
        return HF_Oil_temp;
    }

    int get_HF_Oil_pressure() const
    {
        return HF_Oil_pressure;
    }

    int get_Water_temp() const
    {
        return Water_temp;
    }

    int get_Steam_pressure() const
    {
        return Steam_pressure;
    }

    int get_TimerIgnitor() const
    {
        return TimerIgnitor;
    }

    bool get_FireDetected() const
    {
        return FireDetected;
    }

    bool start_boiler_HeavyFuel_pump()
    {
        bool result = false;
        is_boiler_HeavyFuel_pump_run = true;
        BoilerRuning = true;
        cout<< "start_boiler_HeavyFuel_pump" << endl;
        flush(std::cout);
        result = true;
        return result;
    }

    bool start_boiler_LightFuel_pump()
    {
        bool result = false;
        is_boiler_LightFuel_pump_run = true;
        cout<< "start_boiler_LightFuel_pump" << endl;
        result = true;
        return result;
    }

    bool stop_boiler_HeavyFuel_pump()
    {
        bool result = false;
        is_boiler_HeavyFuel_pump_run = false;
        cout<< "stop_boiler_HeavyFuel_pump" << endl;
        result = true;
        return result;
    }

    bool stop_boiler_LightFuel_pump()
    {
        bool result = false;
        is_boiler_LightFuel_pump_run = false;
        cout<< "stop_boiler_LightFuel_pump" << endl;
        result = true;
        return result;
    }

    bool start_second_AE()
    {
        bool result = false;
        is_second_AE = true;
        cout<< "start_second_AE" << endl;
        result = true;
        return result;
    }

    bool stop_second_AE()
    {
        bool result = false;
        is_second_AE = false;
            cout<< "stop_second_AE" << endl;
            result = true;
        return result;
    }

    bool start_boiler_FUN()
    {
        bool result = false;
        is_boiler_FUN = true;
            cout<< "start_boiler_FUN" << endl;
            result = true;
        return result;
    }

    bool stop_boiler_FUN()
    {
        bool result = false;
        is_boiler_FUN = false;
            cout<< "stop_boiler_FUN" << endl;
            result = true;
        return result;
    }

    bool shiber_open()
    {
        bool result = false;
        is_shiber_open = true;
                cout<< "shiber_open" << endl;
                result = true;
        return result;
    }

    bool shiber_close()
    {
        bool result = false;
        is_shiber_open = false;
                cout<< "shiber_close" << endl;
                result = true;
        return result;
    }

    bool start_ignition()
    {
        int i = 0;
        time_t seconds = time(NULL);
        srand(seconds);
        i = rand()%220;
        bool result = (i > 100);
        if (result)
        {
            is_ignition = true;
            cout<< "start_ignition" << endl;
        }
        return result;
    }

    bool stop_ignition()
    {
        bool result = false;
        is_ignition = false;
            cout<< "stop_ignition" << endl;
            result = true;
        return result;
    }

    bool open_LightFuel_vv()
    {
        bool result = false;

        is_open_LightFuel_vv = true;
                cout<< "open_LightFuel_vv" << endl;
                result = true;
        return result;
    }

    bool close_LightFuel_vv()
    {
        bool result = false;
        is_open_LightFuel_vv = false;
        FireDetected = false;
            cout<< "close_LightFuel_vv" << endl;
            result = true;
        return result;
    }

    bool open_HeavyFuel_vv()
    {
        bool result = false;
        is_open_HeavyFuel_vv = true;
                cout<< "open_HeavyFuel_vv" << endl;
                result = true;
        return result;
    }

    bool close_HeavyFuel_vv()
    {
        bool result = false;
        is_open_HeavyFuel_vv = false;
        FireDetected = false;
            cout<< "close_HeavyFuel_vv" << endl;
            result = true;
        return result;
    }


    bool isFire()
    {
        bool result = false;
            cout<< "isFire" << endl;
            FireDetected = true;
            result = true;
        return result;
    }


};




inline state_aux_boiler::d auto_start_boiler_HeavyFuel(ShipBoiler * sb)
{
    if(sb->get_BoilerRuning() && sb->get_is_open_HeavyFuel_vv())
    {
        return state_aux_boiler::ss_error;
    }
    else
    {
        if (sb->start_boiler_HeavyFuel_pump())
        {
            return state_aux_boiler::ss_open_HeavyFuel_vv;
        }
    }

}

inline state_aux_boiler::d auto_start_boiler_LightFuel(ShipBoiler * sb)
{
    if(sb->get_BoilerRuning() & sb->get_is_open_LightFuel_vv())
    {
        return state_aux_boiler::ss_error;
    }
    else
    {
        if (sb->start_boiler_LightFuel_pump())
            return state_aux_boiler::ss_start_second_AE;
    }
}

inline state_aux_boiler::d auto_stop_boiler(ShipBoiler * sb)
{
    if(!sb->get_BoilerRuning())
        return state_aux_boiler::ss_error;
    else
    {
        return state_aux_boiler::ss_close_HeavyFuel_vv;
    }
}

inline state_aux_boiler::d auto_start_boiler_HeavyFuel_pump(ShipBoiler * sb)
{

    if (sb->get_is_boiler_HeavyFuel_pump_run())
    {
        return state_aux_boiler::ss_open_HeavyFuel_vv;
    }
    else if (sb->get_HF_Oil_temp() > 118)
    {
        if (sb->start_boiler_HeavyFuel_pump())
            return state_aux_boiler::ss_open_HeavyFuel_vv;
    }
    else
    {
        return state_aux_boiler::ss_error;
    }
}

inline state_aux_boiler::d auto_start_boiler_LightFuel_pump(ShipBoiler * sb)
{
    if (sb->get_is_boiler_HeavyFuel_pump_run())
    {
        return state_aux_boiler::ss_open_LightFuel_vv;
    }
    else if (true)
    {
        if (sb->start_boiler_LightFuel_pump())
            return state_aux_boiler::ss_open_LightFuel_vv;
    }
    else
    {
        return state_aux_boiler::ss_error;
    }
}

inline state_aux_boiler::d auto_stop_boiler_HeavyFuel_pump(ShipBoiler * sb)
{
    if (sb->get_is_boiler_HeavyFuel_pump_run())
    {
        if(sb->stop_boiler_HeavyFuel_pump())
            return state_aux_boiler::noop;
    }
}

inline state_aux_boiler::d auto_stop_boiler_LightFuel_pump(ShipBoiler * sb)
{
    if (sb->get_is_boiler_LightFuel_pump_run())
    {
        if(sb->stop_boiler_LightFuel_pump())
            return state_aux_boiler::noop;
    }
}

inline state_aux_boiler::d auto_start_second_AE(ShipBoiler * sb)
{
    if (sb->get_is_second_AE())
    {
        return state_aux_boiler::ss_start_boiler_FUN;
    }
    else
    {
        if (sb->start_second_AE())
            return state_aux_boiler::ss_start_boiler_FUN;
    }

}

inline state_aux_boiler::d auto_stop_second_AE(ShipBoiler * sb)
{
    if (sb->get_is_second_AE())
    {
        if (sb->get_is_boiler_FUN())
            return state_aux_boiler::ss_error;
        else
            if (sb->stop_second_AE())
            {
                if (sb->get_is_boiler_HeavyFuel_pump_run())
                    return state_aux_boiler::ss_stop_boiler_HeavyFuel_pump;
                else if (sb->get_is_boiler_HeavyFuel_pump_run())
                    return state_aux_boiler::ss_stop_boiler_LightFuel_pump;
            }
    }
    else
    {
        if (sb->get_is_boiler_HeavyFuel_pump_run())
            return state_aux_boiler::ss_stop_boiler_HeavyFuel_pump;
        else if (sb->get_is_boiler_HeavyFuel_pump_run())
            return state_aux_boiler::ss_stop_boiler_LightFuel_pump;

    }
}

inline state_aux_boiler::d auto_start_boiler_FUN(ShipBoiler * sb)
{
    if (sb->get_is_second_AE())
    {
        if (sb->start_boiler_FUN())
        {
            return state_aux_boiler::ss_shiber_open;
        }
        else
        {
            return state_aux_boiler::ss_error;
        }
    }
    else
    {
        return state_aux_boiler::ss_error;
    }
}

inline state_aux_boiler::d auto_stop_boiler_FUN(ShipBoiler * sb)
{
    if (sb->get_FireDetected())
        return state_aux_boiler::ss_error;
    if (sb->stop_boiler_FUN())
        return state_aux_boiler::ss_stop_second_AE;
    else
        return state_aux_boiler::ss_error;
}

inline state_aux_boiler::d auto_shiber_open(ShipBoiler * sb)
{
    if(sb->shiber_open())
        return state_aux_boiler::ss_shiber_close;
}

inline state_aux_boiler::d auto_shiber_close(ShipBoiler * sb)
{
    if(sb->shiber_close())
        return state_aux_boiler::ss_start_ignition;
}

inline state_aux_boiler::d auto_start_ignition(ShipBoiler * sb)
{
    bool igneted = false;
    if(sb->get_is_open_HeavyFuel_vv() | sb->get_is_open_LightFuel_vv())
        for (int i = 0; i < 10; ++i)
        {
            igneted = sb->start_ignition();
            sleep(1);
            if (igneted)
                break;

        }
    if (igneted)
    {
        cout<< "Fire exists" << endl;
        return state_aux_boiler::ss_stop_ignition;
    }
    else
        return state_aux_boiler::ss_error;


}

inline state_aux_boiler::d auto_stop_ignition(ShipBoiler * sb)
{
    if (sb->stop_ignition())
        return state_aux_boiler::noop;
}

inline state_aux_boiler::d auto_open_LightFuel_vv(ShipBoiler * sb)
{
    if (sb->get_is_boiler_LightFuel_pump_run())
    {
        if (sb->open_LightFuel_vv())
            return state_aux_boiler::ss_shiber_open;
        else
            return state_aux_boiler::ss_error;
    }
}

inline state_aux_boiler::d auto_close_LightFuel_vv(ShipBoiler * sb)
{
    if (sb->close_LightFuel_vv())
        return state_aux_boiler::ss_stop_boiler_FUN;
}

inline state_aux_boiler::d auto_open_HeavyFuel_vv(ShipBoiler * sb)
{
    if (sb->get_is_boiler_HeavyFuel_pump_run())
    {
        if (sb->open_HeavyFuel_vv())
            return state_aux_boiler::ss_start_second_AE;
        else
            return state_aux_boiler::ss_error;
    }
}

inline state_aux_boiler::d auto_close_HeavyFuel_vv(ShipBoiler * sb, int a)
//inline state_aux_boiler::d auto_close_HeavyFuel_vv(ShipBoiler * sb)
{
    cout<< a << endl;
    if (sb->close_HeavyFuel_vv())
        return state_aux_boiler::ss_stop_boiler_FUN;
}

inline state_aux_boiler::d auto_ERROR(ShipBoiler * sb)
{
    (void)sb;
    cout<< "ERROR!!!!!!!!!!!!!" << endl;
    return state_aux_boiler::noop;
}



#endif // BOILER_H
