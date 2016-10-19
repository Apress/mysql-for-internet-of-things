import string
from random import random, randint

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
temp_tot = 0;
temp_min = 999;
temp_max = 0;
for i in range(0,20):
  # read sensors
  temp = read_sensor(1)
  baro = read_sensor(2)
  # add to total
  temp_tot = temp_tot + temp
  # find min/max
  if (temp < temp_min):
    temp_min = temp
  if (temp > temp_max):
    temp_max = temp
  print(temp, baro) 
  file_log.write(strData.format(temp, baro))
# display aggregate values
print "Average Temperature:", temp_tot/20.00 
print "Min Temperature:", temp_min 
print "Max Temperature:", temp_max 
file_log.close()
