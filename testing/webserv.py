# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    webserv.py                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/07 00:11:05 by danjimen,is       #+#    #+#              #
#    Updated: 2025/07/10 09:07:53 by danjimen,is      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import threading
import requests
import time
import argparse
import sys

def send_get_requests():
	for _ in range(REQUESTS_PER_THREAD):
		try:
			r = requests.get(TARGET)
			print(f"{r.status_code} - {len(r.content)} bytes")
		except Exception as e:
			print(f"Error: {e}")

def send_post_requests():
	for _ in range(REQUESTS_PER_THREAD):
		try:
			data = {"key": "value"}
			r = requests.post(TARGET, data=data)
			print(f"{r.status_code} - {len(r.content)} bytes")
		except Exception as e:
			print(f"Error: {e}")

def send_delete_requests():
	for _ in range(REQUESTS_PER_THREAD):
		try:
			r = requests.delete(TARGET)
			print(f"{r.status_code} - {len(r.content)} bytes")
		except Exception as e:
			print(f"Error: {e}")

def send_requests(method, target, headers, data, validate_text, requests_per_thread):
	for _ in range(requests_per_thread):
		try:
			if method == "GET":
				r = requests.get(target, headers=headers)
			elif method == "POST":
				r = requests.post(target, headers=headers, data=data)
			elif method == "DELETE":
				r = requests.delete(target, headers=headers)
			else:
				print(f"Unsupported method: {method}")
				return

			output = f"{r.status_code} - {len(r.content)} bytes"
			if validate_text and validate_text.encode() not in r.content:
				output += " ❌ VALIDATION FAILED"
			print(output)

		except Exception as e:
			print(f"Error: {e}")

def main():
	parser = argparse.ArgumentParser(description="Multithreaded HTTP tester")
	parser.add_argument("--url", default="http://localhost:8080/", help="Target URL (default: http://localhost:8080/)")
	parser.add_argument("--method", choices=["GET", "POST", "DELETE"], default="GET", help="HTTP method")
	parser.add_argument("--threads", type=int, default=100, help="Number of threads")
	parser.add_argument("--requests", type=int, default=50, help="Requests per thread")
	parser.add_argument("--header", action='append', help="Custom headers. Example: --header 'User-Agent: Test/1.0'")
	parser.add_argument("--data", help="POST data. Example: --data 'key=value&other=value'")
	parser.add_argument("--validate", help="Check if this text is present in response")

	args = parser.parse_args()
	
	# Headers parsing
	headers = {}
	if args.header:
		for h in args.header:
			try:
				key, value = h.split(":", 1)
				headers[key.strip()] = value.strip()
			except ValueError:
				print(f"Invalid header format: {h}")
				sys.exit(1)

	threads = []
	start = time.time()

	for _ in range(args.threads):
		t = threading.Thread(
			target=send_requests,
			args=(args.method, args.url, headers, args.data, args.validate, args.requests)
		)
		t.start()
		threads.append(t)

	for t in threads:
		t.join()

	end = time.time()
	total_requests = args.threads * args.requests

	print(f"\nTotal time: {end - start:.2f}s")
	print(f"Total requests: {total_requests}")
	print(f"Requests/sec: {total_requests / (end - start):.2f}")

if __name__ == "__main__":
	main()