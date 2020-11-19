Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements in the "Default" configuration of the profiler to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See [Shared document](https://docs.google.com/document/d/1Ro9G2Nsr_ZXDhBYJ6YyF9CPivb--6UjhHRmVhDGySag/edit?usp=sharing) for instructions.* 

1. What is the average current per period?
   Answer:
   <br>Screenshot:  **2.97sec**
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ruchaborwankar/blob/master/questions/assign%204%20screenshots/full%20period.JPG)  

2. What is the average current when the Si7021 is Load Power Management OFF?
   Answer:
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ruchaborwankar/blob/master/questions/assign%204%20screenshots/off%20period.JPG)  

3. What is the average current when the Si7021 is Load Power Management ON?
   Answer:
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ruchaborwankar/blob/master/questions/assign%204%20screenshots/on%20period.JPG)  

4. How long is the Si7021 Load Power Management ON for 1 temperature reading?
   Answer:
   <br>Screenshot:  
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ruchaborwankar/blob/master/questions/assign%204%20screenshots/on%20period.JPG)  

5. What is the total operating time of your design for assignment 4 in hours assuming a 1000mAh supply?
Ans:
**The operating time for design assuming the current according to full time period was 294uA with a 1000mAh battery will give 3401Hours (1000/0.294)**

6. How has the power consumption performance of your design changed since the previous assignment?

**Ans:The power consumption has changed considerably with addition of sleep as during timerwait condition due to sleep condition it ends up saving more power.**


7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.

**Ans:With the help of SleepBlockBegin and SLeepBlockEnd functionality one can achieve to go into EM1 modes while I2C transfers occur.According to si labs documentation "A "sleep block" will block the MCU from entering a certain energy mode. A "sleep block" can for instance block EM2 entry because an EM1 only peripheral is in use."**


ref:https://docs.silabs.com/mcu/latest/efr32bg1/group-SLEEP
