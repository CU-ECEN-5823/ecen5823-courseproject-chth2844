Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements in the "Default" configuration of the profiler to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See [Shared document](https://docs.google.com/document/d/1Ro9G2Nsr_ZXDhBYJ6YyF9CPivb--6UjhHRmVhDGySag/edit?usp=sharing) for instructions.* 

1. What is the average current per period?
   Answer: 7.11 uA
   <br>Screenshot:  
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/ecen5823-assignment4-chth2844/blob/master/questions/Images%204/q1.PNG)  

2. What is the average current when the Si7021 is Load Power Management OFF?
   Answer: 3.98 uA
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-chth2844/blob/master/questions/Images%204/q2.PNG)  

3. What is the average current when the Si7021 is Load Power Management ON?
   Answer: 148.82 uA
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-chth2844/blob/master/questions/Images%204/q3.PNG)  

4. How long is the Si7021 Load Power Management ON for 1 temperature reading?
   Answer: 120 ms
   <br>Screenshot:  
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/ecen5823-assignment4-chth2844/blob/master/questions/Images%204/q3.PNG)  

5. What is the total operating time of your design for assignment 4 in hours assuming a 1000mAh supply?
   Total Operating Time=  1000mAh/Period Current(mA) = 140647 Hours

6. How has the power consumption performance of your design changed since the previous assignment?
  <br> The power consumption performance of the design has improved considerably since the previous assignment with significantly lower power consumption during both  ON and OFF periods. For Example the period current in assignment 3 was 3.88mA and in assignment 4 the period current was much smaller value, 7.11uA.
  
7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.
   <br> In order to ensure that board is sleeping in EM1 mode during I2C transfer I checked the return value of Sleep_sleep() function in the debugger after i2cwrite() and i2cread() functions were called. The Sleep_sleep() function returns the value of the energy mode it sleeps in.
