# Raspberry Pi Code to Read Joystick Inputs and Send to Arduino over Serial

import serial
import pygame
import time

# --- CONFIGURATIONS ---
SERIAL_PORT = '/dev/ttyUSB0'  # Change based on your Arduino USB port
BAUD_RATE = 9600

# Initialize serial communication
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"Connected to {SERIAL_PORT}")
except serial.SerialException:
    print("❌ Could not open serial port. Check the port name and permissions.")
    exit(1)

# Initialize pygame for joystick reading
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("❌ No joystick detected!")
    exit(1)

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"🎮 Joystick connected: {joystick.get_name()}")

try:
    while True:
        pygame.event.pump()  # Process event queue

        # Read joystick axes
        x_axis = joystick.get_axis(0)  # Left-right
        y_axis = joystick.get_axis(1)  # Forward-back

        # Determine direction
        if y_axis < -0.5:
            command = "F"  # Forward
        elif y_axis > 0.5:
            command = "B"  # Backward
        elif x_axis < -0.5:
            command = "L"  # Left
        elif x_axis > 0.5:
            command = "R"  # Right
        else:
            command = "S"  # Stop

        # Send to Arduino
        ser.write(command.encode())
        print(f"Sent: {command}")

        time.sleep(0.1)

except KeyboardInterrupt:
    print("\nExiting...")
finally:
    ser.close()
    pygame.quit()
