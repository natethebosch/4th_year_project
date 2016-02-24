/* 
 * File:   SensorDataPoint.h
 * Author: Nate Bosscher
 *
 * Created on February 24, 2016, 3:18 PM
 */

#ifndef SENSORDATAPOINT_H
#define SENSORDATAPOINT_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        float x;
        float y;
        float value;
    } SensorDataPoint;

#ifdef __cplusplus
}
#endif

#endif /* SENSORDATAPOINT_H */

