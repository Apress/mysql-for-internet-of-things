# Demonstration of a simple round robin read scaling slave selector
#
# Use this class in your own application to choose the next slave
# in the topology.
#
# Note: you must have a database and table setup on the master as
#       follows:
#
# CREATE DATABASE read_scaling;
# CREATE TABLE read_scaling.current_slave(server_id INT, host CHAR(30), port INT);
#
# Note: You must also have a replication topology of a master and at least
#       two slaves for the demo output to be meaningful. In fact, running this
#       on a master with no slaves will result in an error.
#
from __future__ import print_function
import mysql.connector as mysql
import operator
import sys
import time

LOCK_TABLE = "LOCK TABLES read_scaling.current_slave WRITE"
SET_SERVER = "UPDATE read_scaling.current_slave SET server_id={0},host='{1}',port={2}"
UNLOCK_TABLE = "UNLOCK TABLES"
GET_CURRENT_SERVER = "SELECT server_id FROM read_scaling.current_slave"

master = {
  'user': 'root',
  'password': 'root',
  'host': 'localhost',
  'port': 13001,
}

class select_read_server(object):
    def __init__(self):
        self.servers = []
        self.cur_server_id = -1;
    
    # Get the list of servers from the master and save the host,
    # port, and server id.
    def _get_server_list(self):
        conn = mysql.connect(**master)
        cur = conn.cursor()
        cur.execute("SHOW SLAVE HOSTS")
        # Save only the id, host, and port
        for row in cur.fetchall():
            server = {
                'id': row[0],
                'host': row[1],
                'port': row[2],
            }
            self.servers.append(server)
        # order the servers by server_id
        cur.close()
        conn.close()
        self.servers.sort(key=operator.itemgetter('id'))
        

    # Set the server in the database
    def _set_server(self, id, host, port):
        self.cur_server_id = id
        conn = mysql.connect(**master)
        cur = conn.cursor()
        cur.execute(LOCK_TABLE)
        query = SET_SERVER.format(id, host, port)
        cur.execute(query)
        print(">", query)
        cur.execute(UNLOCK_TABLE)
        cur.close()
        conn.close()
        
    # Get the current server id from the database
    def _get_current_server_id(self):
        # if first time, skip
        if self.cur_server_id == -1: return
        conn = mysql.connect(**master)
        cur = conn.cursor()
        cur.execute(GET_CURRENT_SERVER)
        self.cur_server_id = cur.fetchall()[0][0]
        cur.close()
        conn.close()
        
      

    # Get the next server in the list based on server id.
    def get_next_server(self):
        self.servers = []
        self._get_server_list()   # update the server list
        if not self.servers:
          raise RuntimeError("You must have slaves connected to use this code.")
        self._get_current_server_id()  # get current server in the database
        for server in self.servers:
            if server["id"] > self.cur_server_id:
                # store the current server_id
                self._set_server(server["id"], server["host"], server["port"])
                return server
        # if we get here, we've looped through all rows so choose first one
        server = self.servers[0]
        self._set_server(server["id"], server["host"], server["port"])
        # return the current server information
        return server
      
                
# instantiate the class - only need this once.
read_server_selector = select_read_server();

# demonstrate how to retrieve the next server and round robin selection
for i in range(0,10):
    print(i, "next read server =", read_server_selector.get_next_server())
    sys.stdout.flush()
    time.sleep(5)
