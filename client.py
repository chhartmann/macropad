import serial
ser = serial.Serial('/dev/ttyACM0', timeout=1)
ser.write(b'SET1FF0000\n')
ser.write(b'SET2FF0000\n')
ser.write(b'SET3FF0000\n')
ser.write(b'FADE111\n')

while True:
  resp = ser.readline().decode()
  if resp:
    resp = resp.rstrip()
    print(resp)
    if (resp == "BTN_PASTE"):
      ser.write(b'FADE000\n')
      ser.write(b'SET100FF00\n')
      ser.write(b'SET200FF00\n')
      ser.write(b'SET300FF00\n')
    if (resp == "BTN_COPY"):
      ser.write(b'SET1FF0000\n')
      ser.write(b'SET2FF0000\n')
      ser.write(b'SET3FF0000\n')
      ser.write(b'FADE111\n')

ser.close()
