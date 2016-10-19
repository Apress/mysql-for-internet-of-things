import mysql.connector
server = {
  'user': 'root',
  'password': 'root',
  'host': '127.0.0.1',
  'database': 'employees',
  'raise_on_warnings': True,
}
cnx = mysql.connector.connect(**server)
cur = cnx.cursor()
# read rows from a file for inserting into plant_monitor table
f = open("plants_data.txt")
lines = f.readlines()
f.close()
# now insert the data
for line in lines:
    cols = line.strip('\n').split(",") # comma-separated row
    query = "INSERT INTO plant_monitoring.plants (name, location, climate)" \
            " VALUES ('{0}','{1}',{2});".format(cols[0], cols[1], cols[2])
    print query
    cur.execute(query)
cnx.commit()
cur.close()
cnx.close()
