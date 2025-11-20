from socket import *
import os
import struct
import time
import select
ICMP_ECHO_REQUEST = 8 

def checksum(source_string):
	"""Calculate the checksum of the input string."""

	csum = 0
	count_to = (len(source_string) // 2) * 2
	count = 0

	while count < count_to:
		this_val = source_string[count + 1] * 256 + source_string[count]
		csum = csum + this_val
		csum = csum & 0xFFFFFFFF
		count += 2
		if count_to < len(source_string):
			csum = csum + source_string[-1]
			csum = csum & 0xFFFFFFFF
			csum = (csum >> 16) + (csum & 0xFFFF)
			csum = csum + (csum >> 16)
			answer = ~csum
			answer = answer & 0xFFFF
			answer = answer >> 8 | (answer << 8 & 0xFF00)

	return answer

def receive_one_ping(my_socket, ID, timeout, dest_addr):
	"""Receive the ping response."""
	time_left = timeout

	while True:
		started_select = time.time()
		ready = select.select([my_socket], [], [], time_left)
		select_duration = (time.time() - started_select)
		if ready[0] == []: # Timeout
			return "Request timed out."

		time_received = time.time()
		rec_packet, addr = my_socket.recvfrom(1024)
		#! Extract the ICMP header from the packet
		#! FILL WITH YOUR CODE

	return "Request timed out."


def send_one_ping(my_socket, dest_addr, ID):
	"""Send an ICMP echo request."""

	my_checksum = 0
	header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, my_checksum, ID, 1)
	data = struct.pack("d", time.time())
	my_checksum = checksum(header + data)

	if os.name == 'posix':
		my_checksum = htons(my_checksum) & 0xFFFF
	else:
		my_checksum = htons(my_checksum)

	header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, my_checksum, ID, 1)
	packet = header + data
	my_socket.sendto(packet, (dest_addr, 1))


def do_one_ping(dest_addr, timeout):
	"""Perform a single ping."""

	icmp = getprotobyname("icmp")
	with socket(AF_INET, SOCK_RAW, icmp) as my_socket:
	#! FILL WITH YOUR CODE
		pass

def ping(host, timeout=1):
	"""Ping a host."""

	try:
		dest = gethostbyname(host)
		print(f"Pinging {dest} ({host}) using Python:\n")
		while True:
			delay = do_one_ping(dest, timeout)
			print(delay)
			time.sleep(1)
	except KeyboardInterrupt:
		print("\nPing stopped.")
	except Exception as e:
		print(f"Error: {e}")


#! FILL WITH YOUR CODE
#! Make ping instructions to three different hosts. Provide the necessary screenshots.
