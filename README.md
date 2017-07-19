# BreathCounter
Arduino code to count the breathing rate from the analog domain.

* The code needs to be compiled to check for errors before downloading
it to the boards. 
* The LED should blink after uploading/breassing the reset button on the chip. 
This indicates the waiting stage before data sampling
* The data will be collected in the next minute when the blinking stops. The end of 
data collection will be indicated when one of trhe external LEDs lights up
* Green LED indicates that the breathing rate is in a good range and
the Red LED is for fast breathing
* In arduino choose *Tools->Board-> Teensy3.6* (or a board of your choice).
* In arduino choose *Tools->Port->COMX* (COMX is the serial port corrsponding to the USB
and it is whichever port that disappears whenever you disconnect the arduino board.



