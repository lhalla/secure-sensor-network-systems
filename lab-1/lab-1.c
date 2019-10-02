/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>

#include "contiki.h"

//#define E_TIMER

#ifdef E_TIMER
#define MIN_ITV 5
#define MAX_ITV 95
#define ITV_STP 6

static struct etimer timer;
#else
#include "sys/energest.h"

#define MIN_ITV 10
#define MAX_ITV 155
#define ITV_STP 11

static struct ctimer timer;
#endif

static int cur = MIN_ITV;

PROCESS(lab_1_process, "ssns lab 1 process");
AUTOSTART_PROCESSES(&lab_1_process);
/*---------------------------------------------------------------------------*/
#ifndef E_TIMER
static inline unsigned long to_seconds(uint64_t time) {
  return (unsigned long)(time / ENERGEST_SECOND);
}
/*---------------------------------------------------------------------------*/
static void callback(void * ptr) {
  printf("lhalla - ctimer - %d\n", cur);

  ctimer_reset(&timer);

#if ENERGEST_CONF_ON == 1

  /*
   * Update all energest times. Should always be called before energest
   * times are read.
   */
  energest_flush();

  printf("\nEnergest:\n");
  printf(" CPU          %4lus LPM      %4lus DEEP LPM %4lus  Total time %lus\n",
         to_seconds(energest_type_time(ENERGEST_TYPE_CPU)),
         to_seconds(energest_type_time(ENERGEST_TYPE_LPM)),
         to_seconds(energest_type_time(ENERGEST_TYPE_DEEP_LPM)),
         to_seconds(ENERGEST_GET_TOTAL_TIME()));
  printf(" Radio LISTEN %4lus TRANSMIT %4lus OFF      %4lus\n\n",
         to_seconds(energest_type_time(ENERGEST_TYPE_LISTEN)),
         to_seconds(energest_type_time(ENERGEST_TYPE_TRANSMIT)),
         to_seconds(ENERGEST_GET_TOTAL_TIME()
                    - energest_type_time(ENERGEST_TYPE_TRANSMIT)
                    - energest_type_time(ENERGEST_TYPE_LISTEN)));

#endif
  
  cur = cur >= MAX_ITV - ITV_STP ? MIN_ITV : cur + ITV_STP;
  ctimer_set(&timer, CLOCK_SECOND * cur, callback, NULL);
}
#endif
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(lab_1_process, ev, data)
{
  PROCESS_BEGIN();

#ifdef E_TIMER
  while(1) {
    printf("lhalla - etimer - %d\n", cur);
    etimer_set(&timer, CLOCK_SECOND * cur);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    cur = cur >= MAX_ITV - ITV_STP ? MIN_ITV : cur + ITV_STP;
  }
#else
  ctimer_set(&timer, CLOCK_SECOND * cur, callback, NULL);
#endif

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
