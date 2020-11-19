Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does the system draw (instantaneous measurement) when a single LED is on with the GPIO pin set to StrongAlternateStrong?**

**Ans:**   Instantaneous measurement with single LED on when set to StrongAlternateStrong is around **4.89mA**

![Image of screenshot](https://github.com/CU-ECEN-5823/ecen5823-assignments-ruchaborwankar/blob/master/questions/strongalternatestrong%20setting%201%20led.JPG)

**2. How much current does the system draw (instantaneous measurement) when a single LED is on with the GPIO pin set to WeakAlternateWeak?**

**Ans:** Instantaneous measurement with single LED on when set to WeakAlternateWeak is around **4.67mA** 

![Image of screenshot](https://github.com/CU-ECEN-5823/ecen5823-assignments-ruchaborwankar/blob/master/questions/weak%20led%201%20on.JPG)

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

**Ans:** There is only difference of 0.22mA which is not much.
According to The AEM accuracy,it is capable of measuring currents in the range of 0.1 µA to 95 mA. The AEM is able to detect changes in the current consumption as small as 100 nA.Since here the range is above current value range we can see almost the same current consumption for both weak and strong drive.

**4. With the WeakAlternateWeak drive strength setting, what is the average current for 1 complete on-off cycle for 1 LED with an on-off duty cycle of 50% (approximately 1 sec on, 1 sec off)?**

**Ans:**  The average current for 1 complete on-off cycle for 1 LED with 50% duty cycle is **4.56mA**.
![Image of screenshot](https://github.com/CU-ECEN-5823/ecen5823-assignments-ruchaborwankar/blob/master/questions/50%25duty%20cycle%20single%20led.JPG)

**5. With the WeakAlternateWeak drive strength setting, what is the average current for 1 complete on-off cycle for 2 LEDs (both on at the time same and both off at the same time) with an on-off duty cycle of 50% (approximately 1 sec on, 1 sec off)?**

**Ans:**  The average current for 1 complete on-off cycle for two LED with 50% duty cycle is **4.89mA**.
![Image of screenshot](https://github.com/CU-ECEN-5823/ecen5823-assignments-ruchaborwankar/blob/master/questions/50%25duty%20cycle%202%20leds.JPG)
