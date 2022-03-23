/*
 * CySatHealthChecks.h
 *
 *  Created on: Mar 10, 2020
 *      Author: Bryan Friestad
 */

#ifndef CYSATHEALTHCHECKS_H_
#define CYSATHEALTHCHECKS_H_

#include <stdint.h>

typedef struct BasicHealthCheck{
    uint8_t OBC_flags;
    uint8_t EPS_flags;
    uint8_t Transceiver_flags;
} BasicHealthCheck_t;

#endif /* CYSATHEALTHCHECKS_H_ */
