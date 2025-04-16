import ctypes
import socket
import threading
import time

class UdpPacket(ctypes.Structure):
    _fields_ = [("buffer", ctypes.c_char * 1024)]

lib = ctypes.CDLL('./libudpbridge.so')
lib.receive_and_respond_udp.argtypes = [ctypes.c_int]
lib.receive_and_respond_udp.restype = UdpPacket

def send_udp_message():
    time.sleep(1)
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(b"Hello from Python (UDP)", ('127.0.0.1', 9999))

    s.settimeout(3)
    try:
        data, _ = s.recvfrom(1024)
        print("Python received:", data.decode())
    except socket.timeout:
        print("No reply from C.")
    finally:
        s.close()

threading.Thread(target=send_udp_message).start()

packet = lib.receive_and_respond_udp(9999)

print("C received (via Python FFI):", packet.buffer.decode())
