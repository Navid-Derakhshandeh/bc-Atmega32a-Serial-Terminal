# bc-Atmega32a

I have developed a simple BC calculator, inspired by retro-style computers from the 1970s. You can upload the code to an ATmega32A microcontroller using a programmer. After that, connect a USB-to-Serial module to the microcontroller and open the serial terminal application (which I developed using C# with a WinForms interface). Select the appropriate serial port and click the "Connect" button.

Once connected, you can enter BC commands into the terminal box and click the "Send" button. The ATmega32A program, designed like a simple kernel, will switch to calculator mode. For example, if you enter an arithmetic expression like 8*100 and click "Send," the output box will display the result 800 in this case. When you enter the exit command and click "Send," the program will exit calculator mode, just like a basic kernel would.

<img width="447" height="363" alt="Screenshot (7)" src="https://github.com/user-attachments/assets/c0854e14-92a2-4405-8904-33ae1322b64e" />

<img width="447" height="360" alt="Screenshot (8)" src="https://github.com/user-attachments/assets/af2bfbf8-886b-4b7d-adc8-964976bb757f" />

![bc-image](https://github.com/user-attachments/assets/5cac9d39-290e-4329-9ddd-5ac94e400584)
