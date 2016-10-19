import datetime   # date and time library

# We begin by creating the file and writing some data.
log_file = open("log.txt", "a+")
for i in range(0,10):
  log_file.write("%d,%s\n" % (i, datetime.datetime.now()))
log_file.close()

# Now, we open the file and read the contents printing out
# those rows that have values in the first column > 5
log_file = open("log.txt", "r")
rows = log_file.readlines();
for row in rows:
  columns = row.split(",")
  if (int(columns[0]) > 5):
    print ">", row
log_file.close()
