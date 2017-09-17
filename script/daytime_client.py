# A simpel test script for netfortune server
# Copyright © 2017 Christian Rapp

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Echo client program
import socket
import json

send_data = {}
send_data['category'] = 'fun'
send_data['bla'] = 'blub'
send_j_data = json.dumps(send_data)
print 
HOST = '127.0.0.1'    # The remote host
PORT = 13              # The same port as used by the server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall((len(send_j_data)).to_bytes(2, byteorder='big'))
    s.sendall(send_j_data.encode())
    # data = s.recv(1024)
# print('Received', repr(data))
# j = json.loads(data)
# print(json.dumps(j, indent=4, sort_keys=True))
