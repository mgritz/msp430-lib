# Library of example code for the popular MSP430G2553 MCU from Texas Instruments

Those who stagger upon this repo might have gained the experience that handling the peripherals of
the MSP430 "bare metal" can be quite a pain in the ass.

I also made this experience in my student days and hence, I gathered several lines of example code
and drivers for both the MCU's peripherals and external ICs.

# What is this for and why is this here?

First of all, this repo is not a single project. It is composed of multiple projects from the past
few years.

The idea behind this is to be used as a library of examples how to solve certain reoccurring
problems when doing anything with the MSP430. Most of the drivers were developed along the way
of student and private projects during my time at the university. They usually are targeted at a
certain task which lies beyond scope of this library. This means: The code might not be as generalized
as it looks on first glance! *Make sure you check the stuff you get from here to fit your needs!*

Not all of the aspects are under ongoing development and I advise anyone who is planning
to use any of this code in their own project to get familiar to the MSP430 first.
*You are responsible of what the code you are using does and not the person you copied it from!*
However, if you find this code useful and whish to contribute with a bugfix to any of it feel free
to post a merge request.

# What is inside?

Basically, there are some peripheral drivers:
* UART
* I2C
* ADC
* Timer/Counter module and PWM

Using those, I implemented abstraction for other chips I interfaced with the MSP:
* HD44780 compatible parallel LCD display
* MPU6050 inertia measurement unit
* PT100 temperature sensor (using OP AMP and internal ADC)

In addition, there are some more general helper functions I like to reuse whenever I do a project that does
* timed waiting
* byte buffer handling

# What about compatibility?

Yes, Texas can be quite difficult to deal with in this field.
Most of this code was developed with TI Code Composer Studio V6. It is using the proprietary MSP430
library that comes along with CCS. I know that TI likes to change the semantics and synopsis of
functions and the meaning of macros as they develop their libraries. Hence, compatibility with
future releases of Texas' stuff might break.

Do not expect me to actively fix those up! If you recognize something broken, let me know.
