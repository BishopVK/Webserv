# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    get.py                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/07 00:11:05 by danjimen,is       #+#    #+#              #
#    Updated: 2025/07/07 00:17:15 by danjimen,is      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import threading
import requests
import time

TARGET = "http://localhost:8080/task"
THREADS = 100
REQUESTS_PER_THREAD = 50

def send_requests():
    for _ in range(REQUESTS_PER_THREAD):
        try:
            r = requests.get(TARGET)
            print(f"{r.status_code} - {len(r.content)} bytes")
        except Exception as e:
            print(f"Error: {e}")

start = time.time()
threads = []

for _ in range(THREADS):
    t = threading.Thread(target=send_requests)
    t.start()
    threads.append(t)

for t in threads:
    t.join()

end = time.time()
print(f"\nTotal time: {end - start:.2f}s")
print(f"Total requests: {THREADS * REQUESTS_PER_THREAD}")
