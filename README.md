# f20-final-project
Gesture Controlled Smart Light Switch

## Project Overview Wiki Page
[Project Overview](https://github.com/CU-ECEN-5823/f20-final-project/wiki/Project-Overview)
A gesture controlled smart light switch will be implemented on the Silicon Labs Blue Gecko Board. The Server gecko will be interfaced with a APD 9960 gesture sensor to detect gestures and a beefcake relay to switch a light on and off. The smart light switch will be able to detect four gestures swipe up, swipe down, swipe left and swipe right. Swipe up gesture will turn the relay switch ON and a swipe down gesture will turn the relay switch OFF. The state of the relay switch as well as the gesture detected will be displayed on the Server Gecko LCD Display. Two Custom GATT Services Gesture and Switch Status hold the characteristic values of the gesture detected and switch status respectively.  The client gecko will be able wirelessly connect and bond to the server gecko over a secure Bluetooth Low Energy (BLE) connection, discover the two services and its associated characteristics and enable notifications in order to get updated values of gesture and relay state. 

## Project Schedule Wiki Page
[Project Schedule](https://github.com/CU-ECEN-5823/f20-final-project/wiki/Project-Schedule)

## Team Members
Update team members names

Chirayu Thakur

Rucha Borwankar


