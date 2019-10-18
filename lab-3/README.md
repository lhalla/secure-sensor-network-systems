# Lab 3 - Logging and IP Protocol Stack – Neighbour and Routing Table

## Errata for lab 1
I forgot to add the source code of the first lab when I turned in the report. The code for that, and this, is available at [https://github.com/lhalla/secure-sensor-network-systems](https://github.com/lhalla/secure-sensor-network-systems).

## Laboratory tasks
[ss-tasks]: ./screenshots/lab_3_tasks.png

In this lab I used contiki-ng to do all of the tasks. I first added the project-conf.h file described in the contiki-ng logging tutoial into the rpl-udp example project. In both cases I used a similar structure for printing: I first look up the head of the table used in the respective task, then loop over the table until all of the entries have been printed. For example, the code I used for printing the neighbours was:

```C
    uip_ds6_nbr_t * nbr_p;
    LOG_INFO("LAB3 :: NBOR LIST BEGINS\n");
    for (nbr_p = uip_ds6_nbr_head(); nbr_p != NULL; nbr_p = uip_ds6_nbr_next(nbr_p)) {
        LOG_INFO("LAB3 :: NBOR IPV6 ");
        LOG_INFO_6ADDR(&nbr_p->ipaddr);
        LOG_INFO_(" LLADDR ");
        LOG_INFO_LLADDR((linkaddr_t *) uip_ds6_nbr_get_ll(nbr_p));
        LOG_INFO_("\n");
    }
    LOG_INFO("LAB3 :: NBOR LIST ENDS\n");
```

Below is a screen capture taken after both tasks had been implemented.

![Tasks running in Cooja][ss-tasks]
**Figure 1**. Lab 3 debug prints.
