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
cur.execute("SHOW DATABASES")
rows = cur.fetchall()
for row in rows:
    print row
cur.close()
cnx.close()
