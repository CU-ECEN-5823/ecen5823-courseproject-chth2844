Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does a single LED draw when the output drive is set to "Strong" with the original code?**

4.49mA

**2. After commenting out the standard output drive and uncommenting "Weak" drive, how much current does a single LED draw?*
4.49mA

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

There is no difference in the current drawn by the single LED in strong drive and weak drive. The reason for the same is that the forward voltage (Vf) across the 
LED remains the same in both cases. Referencing the mainboard schematic we know that R=3kohm,V=3.3V and after measuring Forward Voltage across LED ,Vf= 1.9V. By applying ohms law 
we get current drawn by the LED,I as I=(V-Vf)/R= 433uA. Also referencing the AEM section of the blue gecko user guide we find out that for currents above 250uA the accuracy of current reading is accurate within 0.1mA.    
**4. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with only LED1 turning on in the main loop?**

4.47 mA
140.26 uW

**5. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with both LED1 and LED0 turning on in the main loop?**

4.5 mA
155.2 uW
