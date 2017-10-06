# Multiprocess test script to check netfortune server
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

import socket
import json
import os
from multiprocessing import Pool


def worker():
    print('parent process:', os.getppid())
    print('process id:', os.getpid())
    send_data = {}
    send_data['category'] = 'fun'
    send_data['bla'] = 'blub'
    send_j_data = json.dumps(send_data)
    HOST = '127.0.0.1'    # The remote host
    PORT = 13              # The same port as used by the server
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall((len(send_j_data)).to_bytes(2, byteorder='big'))
        s.sendall(send_j_data.encode())


def main():
    print("Starting Processes pool")
    with Pool(processes=5) as pool:
        for i in range(0, 10):
            pool.apply_async(worker, ())
        pool.close()
        pool.join()


if __name__ == "__main__":
    main()
