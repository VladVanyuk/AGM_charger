/*
******************FUNCTIONS TO CTRL SHIFT REGISTER 74HC595BT*********************
*/
#include "includes.h"

#define SOC_MIN_VOLTAGE 11.63
#define SOC_MAX_VOLTAGE 12.89

#define CONVERT_COEF 8.19
#define VOLTAGE_MAX 3.3
#define ANALOG_READ_MAX 1023

#define CONVERT_TO_VOLTAAGE(a) CONVERT_COEF *a *(VOLTAGE_MAX / ANALOG_READ_MAX)

void stop_charging()
{
    digitalWrite(REL_MAX_PIN, LOW);
    digitalWrite(REL_MID_PIN, LOW);
    digitalWrite(REL_LOW_PIN, LOW);
    print_stop_flag_indicator();
    flags.CSR = false;
    STAGE_FLAGS.stop_flag = true;
}

/////////////////////////////////////////////////////////////////
void first_stage()
{
    digitalWrite(REL_MAX_PIN, HIGH);
    digitalWrite(REL_MID_PIN, LOW);
    digitalWrite(REL_LOW_PIN, LOW);
    print_stage1();
    STAGE_FLAGS.flag_stage_1 = true;
}
/////////////////////////////////////////////////////////////////

void second_stage()
{
    digitalWrite(REL_MAX_PIN, LOW);
    digitalWrite(REL_MID_PIN, HIGH);
    digitalWrite(REL_LOW_PIN, LOW);
    print_stage2();
    STAGE_FLAGS.flag_stage_2 = true;
}
/////////////////////////////////////////////////////////////////

void third_stage()
{
    digitalWrite(REL_MAX_PIN, LOW);
    digitalWrite(REL_MID_PIN, LOW);
    digitalWrite(REL_LOW_PIN, HIGH);
    print_stage3();
    STAGE_FLAGS.flag_stage_3 = true;
}
/////////////////////////////////////////////////////////////////
void set_stage(current_stage_t cur_stage)
{
    current_stage = cur_stage;
    switch (current_stage)
    {
    case STOP:
    {
        stop_charging();
        break;
    }
    case STAGE_1:
    {
        first_stage();
        break;
    }
    case STAGE_2:
    {
        second_stage();
        break;
    }
    case STAGE_3:
    {
        third_stage();
        break;
    }

    default:
        return;
    }
}

current_stage_t get_stage()
{
    current_stage_t stage;

    if (CCV_voltage < 12.40)
    {
        stage = STAGE_1;
    }
    else if (CCV_voltage >= 12.40 && CCV_voltage <= 12.70)
    {
        stage = STAGE_2;
    }
    else if (CCV_voltage > 12.70 && CCV_voltage < 14.30)
    {
        stage = STAGE_3;
    }
    else if (CCV_voltage >= 14.30)
    {
        stage = STOP;
    }

    return stage;
}

/////////////////////////////////////////////////////////////////
uint8_t get_charge()
{
    uint8_t step;

    if (SOC_voltage < SOC_MIN_VOLTAGE)
    {
        step = 0;
    }
    else if (SOC_voltage >= SOC_MAX_VOLTAGE)
    {
        step = 10;
    }
    else
    {
        step = map(SOC_voltage, SOC_MIN_VOLTAGE, SOC_MAX_VOLTAGE, 0, 10);
    }

    return step;
}

//******************OLED DISPLAY FUNCTIONS***************************************

void print_bat_value()
{
    display.fillRect(0, 15, 100, 30, BLACK);
    display.setTextSize(3);
    display.setCursor(70, 20);
    display.println("%");
    display.setCursor(10, 20);
    display.println(CHARGE);
}

void bat_value()
{
    CHARGE = map(get_charge(), 0, 10, 0, 100);
    print_bat_value();
}

float calc_voltage()
{
    int a0 = analogRead(ANALOG_PIN); // read current battery voltage @ SOC0
    delay(200);
    int a1 = analogRead(ANALOG_PIN);    // read current battery voltage @ SOC1
    float v0 = CONVERT_TO_VOLTAAGE(a0); // convert reading to voltage....x8, SOC =source from open circuit
    float v1 = CONVERT_TO_VOLTAAGE(a1);
    float voltage = (v0 + v1) / 2;
    return voltage;
}

void calc_CCV_voltage()
{
    CCV_voltage = calc_voltage();
}

void calc_SOC_voltage()
{
    SOC_voltage = calc_voltage();
}

void stage_handler()
{
    if (current_stage != get_stage())
    {
        set_stage(stage);
    }
}

void charge_handler()
{
    if (SOC_voltage >= 12.60 && flags.AC == true)
    {
        stop_charging();
        charged();
        analogWrite(CHARGE_PUMPIN_PIN, 0); // charge pump
    }

    if (SOC_voltage < 12.60) // 12.89 = 100% bat
    {
        STAGE_FLAGS.stop_flag = false;

        if (flags.AC == true)
        {
            flags.CSR = true;
            charging();
            analogWrite(CHARGE_PUMPIN_PIN, 127); // charge pump
            bat_value();

            if (currentTime - previousTime_2 >= EVENT_INTERVAL_FIVE_SEC)
            {
                calc_CCV_voltage();
                display_CCV_voltage();
                previousTime_2 = currentTime;
            }

            stage_handler();
        }
    }
}
