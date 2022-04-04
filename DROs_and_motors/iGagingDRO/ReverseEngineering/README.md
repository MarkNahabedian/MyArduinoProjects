# How Does the DRO Work?

This file and its containing directory host my observations about the
iGaging EZ-VIEW DRO.


## Links to others' observations:

https://www.yuriystoys.com/2015/12/working-with-igaging-absolute-dro-scales.html

https://wiki.hive13.org/images/2/24/21_Bit_Protocol_Scales.pdf


## My Observations

My observations were made using a **Digilent Analog Discovery 2** instrument.

I'm studying an **iGaging EZ-VIEW DRO**.  This DRO has a liner scale
with a slider that reads its position.  The slider has a cable
connected to a display.  The display provides power to the sliding
read head.  My goal is to replace the display with a single board
computer.


### Electrical

The cable from the read head has a UDB Micro B connector. Though USB
is not used to communicate between the display and read head, I will
use the USB names for each pin of the connector to refer to them.

| **USB Pin** | **DRO Use** |
| 5V          | 3V at 20uA  }
| D-          | clock from display  |
| D+          | data from read head |
| ID          | unused      |
| GND         | GND         |

Current measurements were made using an old Micronta multimeter.

