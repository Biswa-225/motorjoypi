import serial
import evdev
from evdev import InputDevice, categorize, ecodes

# Adjust serial port and speed
arduino = serial.Serial('/dev/ttyUSB0', 9600)

# Find joystick
devices = [InputDevice(path) for path in evdev.list_devices()]
joystick = None
for device in devices:
    if 'Joystick' in device.name or 'Gamepad' in device.name:
        joystick = InputDevice(device.path)
        break

if not joystick:
    print("❌ Joystick not found!")
    exit()

print(f"✅ Connected to joystick: {joystick.name}")

for event in joystick.read_loop():
    if event.type == ecodes.EV_ABS:
        absevent = categorize(event)
        if absevent.event.code == ecodes.ABS_Y:
            if absevent.event.value < 100:
                arduino.write(b'F')
            elif absevent.event.value > 150:
                arduino.write(b'B')
            else:
                arduino.write(b'S')
