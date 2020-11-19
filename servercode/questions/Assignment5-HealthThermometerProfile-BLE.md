Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements in the "Default" configuration of the profiler to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See [Shared document](https://docs.google.com/document/d/1Ro9G2Nsr_ZXDhBYJ6YyF9CPivb--6UjhHRmVhDGySag/edit?usp=sharing) for instructions.* 

1. Provide screen shot verifying the Advertising period matches the values required for the assignment.
   <br>Screenshot:  
   **Since,we set gecko_cmd_le_gap_set_advertise_timing(0, 400, 400, 0, 0)) where 400 represents the min and max advertising interval i.e 250ms*1.6=400(value).we get 250 msec properly in the waveform as shown**
   ![advertising_period](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/advertising%20channel%20period.JPG) 

2. What is the average current between advertisements?
 
 Answer:**The average current between advertisements is 188.16uA.**
   <br>Screenshot:  
   ![avg_current_between_advertisements](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/advertising%20channel%20period.JPG)  

3. What is the peak current of an advertisement? 
   
   Answer:**Peak current of advertisement is observed to be 25.44mA approximately**
   <br>Screenshot:  
   ![peak_current_of_advertisement](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/adv%20channels.JPG)  

4. Provide screen shot showing the connection interval settings. Do they match the values you set in your slave(server) code or the master's values?.
   <br>Screenshot:
  
  **They almost match the values.We entered the min and max interval time as 75msec and obtained around 83msec.**
   ![connection_interval](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/connecting%20interval.JPG)  

5. What is the average current between connection intervals?
   
   Answer:**The average current between connection intervals is 689.26nA**
   <br>Screenshot:  
   ![avg_current_between_connection_intervals](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/connecting%20interval.JPG)  

6. If possible, provide screen shot verifying the slave latency matches what was reported when you logged the values from event = gecko_evt_le_connection_parameters_id. 
   <br>Screenshot:  
   
   **Timeout is set to 600 and slave latency for 300 msec according to assignment document.**
   **we get this on reading the gecko_evt_le_connection_paramters_id-**
   ![connection paramters id parameters received](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/log-connection%20timeout%20param.JPG)
   ![slave_latency](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/slave%20latency.JPG)  

7. What is the peak current of a data transmission when the phone is connected and placed next to the Blue Gecko? 
   
   Answer:**The phone when placed near shows around 4.31mA approximately**
   <br>Screenshot:  
   ![peak_current_phone_next_to](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/peak%20near%20the%20from%20phone.JPG)  
   
8. What is the peak current of a data transmission when the phone is connected and placed approximately 20 feet away from the Blue Gecko? 
  
  Answer:**Peak current when data is placed around 20 feet away is 11.30mA approximately.**
   <br>Screenshot:  
   ![peak_current_phone_20ft_away](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/peak%20current%20away.JPG)  
 **App output** 
 
 ![App output](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/app%20output%20(2).jpeg)
 
 **Log output** 
 
 ![log output](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ruchaborwankar/blob/master/questions/assign%205%20screenshots/log%20output.JPG)
