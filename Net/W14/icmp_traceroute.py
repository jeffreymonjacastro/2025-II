from socket import *
import os
import struct
import time
import select
from sys import getsizeof

ICMP_ECHO_REQUEST = 8
MAX_HOPS = 30
TIMEOUT = 2.0
TRIES = 2

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


def build_packet():
    """Build the ICMP echo request packet."""

    my_checksum = 0
    my_id = os.getpid() & 0xFFFF 
    # Header: Type=8, Code=0, Checksum=0, ID, Sequence=1
    header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, my_checksum, my_id, 1)
    data = struct.pack("d", time.time())
    
    my_checksum = checksum(header + data)
    
    if os.name == 'posix':
        my_checksum = htons(my_checksum) & 0xFFFF
    else:
        my_checksum = htons(my_checksum)
        
    header = struct.pack("bbHHh", ICMP_ECHO_REQUEST, 0, my_checksum, my_id, 1)
    return header + data


def get_route(hostname):
    """Perform a traceroute to the specified hostname."""

    try:
        dest_addr = gethostbyname(hostname)
        print(f"Traceroute to {hostname} ({dest_addr}), {MAX_HOPS} hops max:\n")
    except gaierror as e:
        print(f"Unable to resolve hostname {hostname}: {e}")
        return
    
    for ttl in range(1, MAX_HOPS + 1):
        for _ in range(TRIES):
            with socket(AF_INET, SOCK_RAW, getprotobyname("icmp")) as my_socket:
                my_socket.setsockopt(IPPROTO_IP, IP_TTL, struct.pack("I", ttl))
                my_socket.settimeout(TIMEOUT)
                
                try:
                    packet = build_packet()
                    
                    my_socket.sendto(packet, (hostname, 0))
                    
                    ready = select.select([my_socket], [], [], TIMEOUT)
                    
                    # Timeout 
                    if ready[0] == []: 
                        print(f"{ttl} * * * Request timed out.")
                        continue
                        
                    time_received = time.time()
                    rec_packet, addr = my_socket.recvfrom(1024)

                    # Debug: Check packet length
                    if len(rec_packet) < 28:
                        print(f"{ttl} {addr[0]} (Packet too short: {len(rec_packet)} bytes)")
                        break

                    icmp_header = rec_packet[20:28]

                    types, code, checksum_recv, id_recv, sequence_recv = struct.unpack(
                        "bbHHh", icmp_header
                    )

                    # ICMP Type 11: Time Exceeded (from an intermediate router)
                    if types == 11:
                        if len(rec_packet) >= 64:
                            time_sent = struct.unpack("d", rec_packet[56:64])[0]
                            rtt = (time_received - time_sent) * 1000
                            print(f"{ttl} {addr[0]} rtt={rtt:.2f} ms (Time Exceeded)")
                        else:
                            print(f"{ttl} {addr[0]} (Time Exceeded - incomplete packet) {len(rec_packet)} bytes")
                        break 
                        
                    # ICMP Type 3: Destination Unreachable
                    elif types == 3: 
                        if len(rec_packet) >= 64:
                            time_sent = struct.unpack("d", rec_packet[56:64])[0]
                            rtt = (time_received - time_sent) * 1000
                            print(f"{ttl} {addr[0]} rtt={rtt:.2f} ms (Destination unreachable)")
                        else:
                            print(f"{ttl} {addr[0]} (Destination unreachable - incomplete packet)")
                        break 
                        
                    # ICMP Type 0: Echo Reply (from the final destination)
                    elif types == 0: 
                        if len(rec_packet) >= 36:  # 28 + 8 bytes for timestamp
                            time_sent = struct.unpack("d", rec_packet[28:36])[0]
                            rtt = (time_received - time_sent) * 1000
                            print(f"{ttl} {addr[0]} rtt={rtt:.2f} ms (Reached destination)")
                        else:
                            print(f"{ttl} {addr[0]} (Reached destination - incomplete packet)")
                        return 

                    else:
                        print(f"{ttl} {addr[0]} Unexpected ICMP type {types}, packet length: {len(rec_packet)} bytes")
                        break 
                        
                except timeout:
                    print(f"{ttl} * * * Request timed out.")
                    continue
                except Exception as e:
                    print(f"{ttl} Error: {e}")
                    break

if __name__ == '__main__':
    get_route("www.google.com")
    get_route("www.elcomercio.pe")
    get_route("www.mppt-dashboard-iot.vercel.app")