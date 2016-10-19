import mysql.connector;

cnx = mysql.connector.connect(user="root", password="secret")
cur = cnx.cursor()
cur.execute("SELECT * FROM test.derived_annotation_test")
old_value = 0;
for row in cur.fetchall():
  if old_value > 0:
    print "{0}, {1}, {2}, {3}, {4}".format(row[1], row[2], row[3], row[4], row[4] - old_value)
  else:  
    print "{0}, {1}, {2}, {3}".format(row[1], row[2], row[3], row[4])
  old_value = row[4]
  