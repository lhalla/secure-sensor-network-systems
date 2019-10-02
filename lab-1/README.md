# Lab 1 - Introduction

## Installing ContikiOS on Fedora  30
I followed the instructions for the installation of the Contiki-NG Docker image available [here](https://github.com/contiki-ng/contiki-ng/wiki/Docker "Contiki-NG Docker installation guide"). This image contains the necessary source code for ContikiOS applications as well as the Cooja simulator.

## Laboratory tasks
[ss-task-1]: ./screenshots/lab_1_task_1.png
[ss-task-2]: ./screenshots/lab_1_task_2.png
[ss-task-3]: ./screenshots/lab_1_task_3.png

All of the following tasks are combined in the [lab-1.c](./lab-1.c)-file. The documentation used for the **timers** is available [here](https://github.com/contiki-ng/contiki-ng/wiki/Documentation:-Timers) and for the **energest** module [here](https://github.com/contiki-ng/contiki-ng/wiki/Documentation:-Energest).

### Task 1
The first task was to implement an **etimer** that periodically prints my name and the current interval. **Etimer** schedules events that are posted to the local process once the set amount of time has passed. The length of the interval increases linearly between 5 and 95 seconds in steps of 6. Once it reaches the maximum duration, the duration is reset to the minimum.

The program for this task is built over the **hello-world** example available in Contiki. The `E_TIMER` constant must be defined in the source code on line 36. This defines the necessary duration constants as well as causes the `timer` variable to be declared as an **etimer**. The pattern I used for the prints was `"lhalla - etimer - %d"`, where `%d` would be replaced with the current value of the variable used to store the timer duration. The program prints it out first, then sets the timer and finally, after waiting for the expiration event, increases the duration for the next iteration. A screenshot of this program being run is shown in figure 1.

![Task 1 program running in Cooja][ss-task-1]
**Figure 1**. Task 1 program running in Cooja.

### Task 2
The second task was to, similarly to the first task, implement a timer but this time a **ctimer**. **Ctimer** uses a callback function to handle timer expirations and this function will be used here to produce periodic prints akin to those of the first task. This time the intervals vary between 10 and 155 seconds with a step of 11.

To keep both of the implementations in the same file, the aforementioned `E_TIMER` constant was added to the source code during this task. This allowed me to separate the constant definitions and to define the callback function used with the **ctimer** only when the said timer was used.

Initially there was some confusion due to the vagueness of the examples included in the **ctimer** section of the **timer** documentation. This was mainly related to the position of the callback function within the source code. This problem was solved once I found [this](https://gitlab.irisa.fr/fedesismo/contiki-sniffer-injector/blob/22e5e268458214b93869126d504d338cc1f79714/examples/ipv6/rpl-udp-bckp/udp-client.c) example through Google.

This time the `E_TIMER` constant's definition has to be commented out. The functionality of the program is similar to that of the first task with the exception of the duration variable incrementation being moved to the callback function. A screenshot of this program being run is shown in figure 2.

![Task 2 program running in Cooja][ss-task-2]
**Figure 2**. Task 2 program running in Cooja.

### Task 3
The third, and final, task of this lab was to implement **energest** into the program of the second task and print the energy consumption statistics at the intervals defined in task 2. **Energest** provides functionality that enables the estimation of the node's energy consumption by measuring the amount of time e.g. the radio is active.

At first this task appeared to be a simple addition of the required prints found in the **energest** example application. However, after adding the prints to the callback function, Cooja was unable to compile the file. It complained that the `energest_get_total_time`-function was undefined. I had at that point set the definition for the `ENERGEST_CONF_ON` constant at the beginning of the file. 

After going through the source code of the **energest** library found [here](https://github.com/contiki-ng/contiki-ng/blob/develop/os/sys/energest.h) and [here](https://github.com/contiki-ng/contiki-ng/blob/develop/os/sys/energest.c), I realised that I had to declare the constant outside of the application for it to also apply to the .c-file. I then noticed that the **energest** example application had a project-conf.h -file defined under the same directory. I copied this configuration file and then Cooja managed to compile the code. A screenshot of the finished program being run is shown in figure 3.

![Task 3 program running in Cooja][ss-task-3]
**Figure 3**. Task 3 program running in Cooja.
