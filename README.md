### *Disclaimer*
*I am not a microcontroller developer nor am I a C-plusplus developer.  I am also definitively not a hardware designer.*

# Kitchen timer

A long time ago my lovely sister gave me a kitchen timer which would beep once when there are 10 minutes left and twice when there are 5 minutes left.  Unfortunately time got the better of the timer and it broke but I got hooked on those features.

I have been unable to find a replacement but after dabbling with some [circuits](https://github.com/nurk/8-bit-cpu) a bit I thought I might be able to make my own.

This repository contains the version(s) that I deem worthy of the internet.

You will find a software and hardware folder with each containing the respective versions.

The hardware folder contains a KiCaD project where the symbols and footprints used have been added to a local library.  You will also find a pdf of the [schematic](https://github.com/nurk/kitchen_timer/blob/ce0c0dc44edd9d53a935f777b976885855d0056c/hardware/kitchen_timer_6.3/schematic.pdf), a screenshot of the [pcb](https://github.com/nurk/kitchen_timer/blob/ce0c0dc44edd9d53a935f777b976885855d0056c/hardware/kitchen_timer_6.3/pcb.png) and a screenshot of the [3D rendering](https://github.com/nurk/kitchen_timer/blob/ce0c0dc44edd9d53a935f777b976885855d0056c/hardware/kitchen_timer_6.3/3D.png).

The software folder contains an arduino ide project with the, well, software.

## User guide
I am not going to explain how to use a kitchen timer.  If the main function of the 4 buttons does not speak for themselves I did really bad.  I am only going to explain the extra features.

### Timer / C°
This switch toggles the display to show the timer and the measured temperature.

### Loud / Quiet
I bought cheap buzzers on amazon and they all seem a bit different.  Some are quiet and some are loud.  Because I was tired of changing the inline resistor for the buzzer I added a toggle to switch between two values.  One is louder, the other is quieter.

### Reset long press
Because you can fully turn off this device a way was needed to set the time for when you turn on the device.  The chance of you needing a timer of 0 minutes and 0 seconds is very slim.

After setting a time you can long press the **Reset** button until you hear a second beep and the set time will be stored.  When cycling the power the stored time will be loaded.

### Start/Stop long press
When you long press the **Start/Stop** the letter `A` will appear and you will have entered the setup menu.  After releasing the **Start/Stop** button you will see `A5. 0`.  Clicking the **Start/Stop** button will cycle through the menu items:  
| Displayed        | Meaning         |
| :--------------: | :-------------: |
| `A5. 0`            | 5 minute alarm  |
| `A10.0`            | 10 minute alarm |
| `A15.0`            | 15 minute alarm |
| `A20.0`            | 20 minute alarm |

The value after the `.` is the number of beeps that will sound when there are 'x' minutes left.  You can use the **Minutes** button to increase the number of beeps and the **Seconds** button to decrease the number of beeps.

To exit the setup menu just click the **Reset** button.

## Miscellaneous
Some component sizes might seem big for some.  Especially resistors and capacitors of size 0805.  Keep in mind I am hand soldering this and I am becoming an old fart with bad eyesight.

The microcontroller is an ATmega4808 which has way more flash storage than is needed.  I still chose this one because it is what I have and I am not going to buy a new variant for each project I do.

The battery charge controller I chose only charges at 500mA.  So you will need a battery with at least this capacity (or you have to change the resistor on the programming pin).  I have a 2000mA battery connected and I takes a fair while to charge.

**This is not a cost effective way to get a kitchen timer.  If you need one you are better of going to the store and buying one.**