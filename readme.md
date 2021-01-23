To upload this code to the teensy, use the Arduino IDE. 
  1. Open TCU.ino in the arduino IDE (download at https://www.arduino.cc/en/software)
  2. Select file>preferences and ensure the sketchbook location is the folder which contains the TCU.ino file.
  3. Plug the teensy into the computer via a micro usb
  4. In the IDE, select tools>board>teensy>teensy 3.5
  5. Select tools>port and choose the correct port (if not done automatically)
  6. Click the upload button on the top left.
  
  Note that the TCU.ino file includes the other files via the header (.h) files. The functionality is primariliy described in the associated .cpp files.
  Each header file in the include list at the top of TCU.ino has an associated .cpp file which creates the implimentation.
