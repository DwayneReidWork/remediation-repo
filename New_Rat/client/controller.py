from enum import IntEnum, auto
import cmd2
import ipaddress
import socket
import struct
from .netcomm import recv_expected, send_all

HEADER_FORMAT = "<BI"

class PacketType(IntEnum):
    PING = 1
    MUL = auto()
    ECHO = auto()
    SHOW = auto()


class Client():
    def __init__(self,client_socket: socket.socket, address:tuple[str,int]):
        super().__init__(daemon=True)
        self.client_socket = client_socket
        self.address = address

def ip_type(ip_str: str) -> ipaddress.IPv4Address:
    try:
        addr = ipaddress.IPv4Address(ip_str)
        return addr
    except:
        raise Exception(f'A valid host IPv4 is required; found: {ip_str}')
    
def port_type(port_str: str) -> int:
    port = int(port_str)
    if port < 0 or port > 65535:
        raise Exception('Port must be in range [1,65535]')
    return port

class CmdController(cmd2.Cmd):
    
    def __init__(self):
        super().__init__()
        self.sock = None
        self.debug = True

    '''Parser using cmd2 for dealing with connections'''
    connection_parser = cmd2.Cmd2ArgumentParser()
    connection_parser.add_argument('host', action='store', type=ip_type, help="Host you want to connect to")
    connection_parser.add_argument('port', action='store', type=port_type, help="Port you want to connect to")
    @cmd2.with_argparser(connection_parser)
    def do_connect(self, args):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((str(args.host), (args.port)))
        response = recv_expected(self.sock, 17)
        print(f"Response: {response}")