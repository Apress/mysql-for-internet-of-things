from random import random
import string

def read_sensor(sensor_num):
  if (sensor_num == 1):
    return 90.125 + random()*10  # sensor #1
  elif (sensor_num == 2):
    return 14.512313 + random()  # sensor #2
  else:
    if (randint(25) >= 5):
      return randint(14)/10.00
    else:
      return -randint(14)/10.00

strData = "Temperature sensor: {0:.2f}, Barometric sensor: {1:.2f}\n"
file_log = open("data_log_python.txt", 'a')
file_log.write(strData.format(read_sensor(1), read_sensor(2)))
file_log.close()
