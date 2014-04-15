/*
 * clock.h
 *
 *  Created on: Apr 10, 2014
 *      Author: ozi
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <sys/time.h>

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}



#endif /* CLOCK_H_ */
