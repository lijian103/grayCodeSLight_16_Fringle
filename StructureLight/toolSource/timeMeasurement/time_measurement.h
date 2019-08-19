/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/2
 */

#ifndef TIME_MEASUREMENT_HPP
#define TIME_MEASUREMENT_HPP

/** @brief  Contains all mytool classes, functions, etc. */
namespace  mytool
{

    class TimeMeasurement
    {
    public:
        TimeMeasurement();
        ~TimeMeasurement();
        void UpdateStartTime();
        double GetTimeConsumption();
        void PrintTimeConsumption();

    private:
        double start_time;
        double end_time;
        double tick_freq;
    };

}

#endif //TIME_MEASUREMENT

