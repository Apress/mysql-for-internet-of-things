import mysql.connector;
from random import random, randint

def read_sensor():
  return 500 + randint(0,1500)

strInsert = "INSERT INTO pond VALUES (null, {0}, '{1}')"
cnx = mysql.connector.connect(user="root", password="SECRET", database="test")
cur = cnx.cursor()
# Calculate enumerated value for sensor.
water_level = read_sensor()
if water_level > 1500:
  state = 'CLEAN'
elif water_level > 1150:
  state = 'LOW'
elif water_level > 700:
  state = 'NORMAL'
else:
  state = 'HIGH'
cur.execute(strInsert.format(water_level, state))
cnx.commit()
cur.close()
cnx.close()
