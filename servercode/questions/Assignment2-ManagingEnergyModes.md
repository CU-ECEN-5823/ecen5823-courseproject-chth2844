Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**Note: All average currents should be taken at a time scale of 200mS/div.**

**1. Fill in the below chart based on currents measured in each energy mode, replacing each TBD with measured values.  Use the [Selecting Ranges](https://www.silabs.com/documents/public/user-guides/ug343-multinode-energy-profiler.pdf) feature of the profiler to select the appropriate measurement range.  Your measurements should be accurate to 10%**

Energy Mode | Period average current (uA) | Average Current with LED off (uA) | Average Current with LED On (uA)
------------| ----------------------------|-----------------------------------|-------------------------
EM0         |           3.22mA(3220uA)    |           3.19mA(3190uA)          |         3.68mA(3680uA)
EM1         |           3.07mA(3070uA)    |           3.53mA(3530uA)          |         3.03mA(3030uA)
EM2         |           46.9uA            |           513.77uA                |         4.91uA
EM3         |           47.2uA            |           522.25uA                |         7.61uA

**2. Fill in the below chart based on time measured period and LED on time measurements for EM2 and EM3 modes, replacing each TBD with measured values.  Use the [Selecting Ranges](https://www.silabs.com/documents/public/user-guides/ug343-multinode-energy-profiler.pdf) feature of the profiler to select the appropriate measurement range.  When measuring time, measure from peak current to peak current.  Your LED on time measurement should be accurate to 1ms and your period measurement should be accurate to within 10ms**

Energy Mode | Period (sec)| LED On Time (ms) |
------------| -----------|-------------------
EM2         |   2.25secs |        178ms
EM3         |   2.22secs |        180ms


**3. ScreenShots**  

***EM0***  
Period average current    
![em0_avg_current_period][em0_avg_current_period]  
Average Current with LED ***off***  
![em0_avg_current_ledoff][em0_avg_current_ledoff]  
Average Current with LED ***on***  
![em0_avg_current_ledon][em0_avg_current_ledon]  

***EM1***  
Period average current    
![em1_avg_current_period][em1_avg_current_period]  
Average Current with LED ***off***  
![em1_avg_current_ledoff][em1_avg_current_ledoff]  
Average Current with LED ***on***  
![em1_avg_current_ledon][em1_avg_current_ledon]  

***EM2***  
Period average current  
![em2_avg_current_period][em2_avg_current_period]  
Average Current with LED ***off***  
![em2_avg_current_ledoff][em2_avg_current_ledoff]  
Average Current with LED ***on***  
![em2_avg_current_ledon][em2_avg_current_ledon]   
LED measurement - Period   
![em2_led_period][em2_led_period]  
LED measurement - LED on time   
![em2_led_ledOnTime][em2_led_ledOnTime]  

***EM3***  
Period average current    
![em3_avg_current_period][em3_avg_current_period]  
Average Current with LED ***off***  
![em3_avg_current_period][em3_avg_current_ledoff]   
Average Current with LED ***on***  
![em3_avg_current_period][em3_avg_current_ledon]   
LED measurement - Period   
![em3_led_period][em3_led_period]  
LED measurement - LED on time   
![em3_led_ledOnTime][em3_led_ledOnTime]  

[em0_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em0/em0%20full%20period.JPG
[em0_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em0/em0%20off%20period.JPG
[em0_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em0/em0%20on%20period.JPG

[em1_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em1/em1%20full%20period.JPG
[em1_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em1/em1%20off%20period.JPG
[em1_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em1/em1%20on.JPG

[em2_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em2/em%20mode%202%20full%20period.JPG
[em2_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em2/em2%20off%20time.JPG
[em2_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em2/em2%20on%20time.JPG
[em2_led_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em2/em%20mode%202%20full%20period.JPG
[em2_led_ledOnTime]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em2/em2%20on%20time.JPG

[em3_avg_current_period]:https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em3/em3%20full.JPG
[em3_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em3/off.JPG
[em3_avg_current_ledon]:https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em3/em3%20on.JPG
[em3_led_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em3/em3%20full.JPG
[em3_led_ledOnTime]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-ruchaborwankar/blob/master/questions/assign%202%20screenshots/em3/em3%20on.JPG
