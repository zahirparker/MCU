LED Fade
===

LED Fade project for R5F102AA 
LED is connected to P3.1 with Anode connected to P3.1 and cathode connected to ground 
This project uses the PWM hardware 

 **Device** : R5F102AA

 **Tools** : e2 studio v3.0.0.22

   KPIT GNU RL78-ELF ToolChain v14.02

Code was generated using Applilet Code generator from within e2 studio

Settings used for Code generation
---------------------------------
  * PIOR0 = 0, PIOR1 = 0, PIOR2 = 0

  * VDD = 5V
  
  * Main system clock (fMAIN) = High-speed Onchip Clock Osc (fIH)
  * Internal Oscillator Freq: 24MHz

  * Interval Timer working on 15KHz 
  * Interval Timer Configured to generate an interrupt every 100 ms
  
  * Timer Array Unit has 8 16 bit timers
  ** Channel 0 to be configured as PWM master
  *** Channel 0 master sets the PWM frequency. 100Hz (or 10ms)
  ** Channel 3 to be configured as PWM slave
  *** Channel 3 slave sets the PWM duty cycle. 50% to begin with.
  ** Channel 3 Output on P3.1 
  
 After generating the code Call the required functions in R_MAIN_UserInit() 
For e.g in this case the interval Timer needs to be started and hence call the R_IT_Start() function 
R_TAU0_Channel0_Start()
Populate the r_it_interrupt function in r_cg_it_user file

Add the code for delay()

```
  void r_it_interrupt(void) 
  { 
      /* Start user code. Do not edit comment generated here */ 
    ticks++; 
    if (ticks >= 5 ) 
    { 
      /* Toggle P3.1 */ 
      P3_bit.no1 = ! P3_bit.no1; 
       
      ticks = 0; 
    } 
      /* End user code. Do not edit comment generated here */ 
  } 
```

Building the project will generate LED_BLINK.mot in the HardwareDebug folder

Flashing 
---------------------------------
  * Done through Serial Port using Renesas Flash Programmer 
  * The microcontroller comes with a boot loader installed 
  * Open Renesas Flash Programmer 
  * Create New Workspace 
  * Select Microcontroller R5F102AA 
  * Select Communication Interface : COM<ID> 
  * Browse for .mot file 
  * Put the device in program mode by pressing the SW3 on board 
  * Hit the start button in Renesas Flash Programmer 
  * Release the SW3 button  



Issues Faced
-------------------------------------
  * WatchDog Timer resetting the device
  ** How to debug if the issue is due to watchdog?

  * Wrong datatype selection
  ** unsigned int vs int 
  
  // Read the Master Timer value
	g_timerVal = TDR00;

  // Compute 10% of the value
	g_timerValInc = g_timerVal / 10;






