from enum import IntEnum, auto
import cmd2
import ipaddress
import socket
import struct
from netcomm import recv_expected, send_all


class PacketType(IntEnum):
    PING = 1
    MUL = auto()
    ECHO = auto()
    SHOW = auto()

class Packet:
    HEADER_FORMAT = "!BI"

    def __init__(self, p_type: PacketType, payload=None):
        self.p_type = p_type
        self.payload = b""

        # Allow flexible input for payload
        if payload is None:
            self.payload = b""
        elif isinstance(payload, int):
            self.payload = struct.pack("!i", payload)
        elif isinstance(payload, str):
            self.payload = payload.encode("utf-8")
        elif isinstance(payload, bytes):
            self.payload = payload
        else:
            raise TypeError(f"Unsupported payload type: {type(payload)}")
        
    @property
    def packet_size(self) -> int:
        """Return size of payload only."""
        return len(self.payload)

    def pack(self) -> bytes:
        """Serialize the packet into bytes."""
        header = struct.pack(self.HEADER_FORMAT, self.p_type, self.packet_size)
        return header + self.payload

    # @classmethod
    # def unpack(cls, data: bytes):
    #     """Deserialize a packet from bytes."""
    #     header_size = struct.calcsize(cls.HEADER_FORMAT)
    #     p_type, payload_len = struct.unpack(cls.HEADER_FORMAT, data[:header_size])
    #     payload = data[header_size:header_size + payload_len]
    #     return cls(PacketType(p_type), payload)


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

    str_parser = cmd2.Cmd2ArgumentParser()
    str_parser.add_argument('string', action='store', nargs='?', type=str, help="String you want echo'd back.")
    @cmd2.with_argparser(str_parser)
    def do_echo(self, args):
        if (self.sock is None):
            print("Not connected.")
        echo_packet = self._make_packet(PacketType.ECHO, args.string)
        
        print(f"Echo Packet: {echo_packet}")
        send_all(self.sock, echo_packet)
        type, expected_len = self._check_packet_type(self.sock)

        if (type != PacketType.ECHO):
            print("Invalid reply revcd")
            return
        response = recv_expected(self.sock, expected_len)
        
        if response:
            decoded_response = response.decode('utf-8')
            print(f"Reply: {decoded_response}")
        else:
            print(f"Error: Failed to receive valid response data. {response}")


    def _make_packet(self, PacketType, data):
        match PacketType:
            case PacketType.PING:
                print("Making Ping Packet")
            case PacketType.MUL:
                print("Making Mul Packet")
            case PacketType.ECHO:
                print("Making Echo Packet")
                response_packet = Packet(p_type=PacketType.ECHO, payload=data).pack()
            case PacketType.SHOW:
                print("Making Show Packet")
            case _:
                print("Invalid something recvd")
            
        return response_packet

    def _check_packet_type(self, args):
        header_format = '!BI'
        header_size = struct.calcsize(header_format)
        packet_header = recv_expected(self.sock, header_size)
        print(f"P_Header: {packet_header}")
        if len(packet_header) != header_size:
            raise ValueError("Incomplete header received")
        
        raw_type, expected_len = struct.unpack(header_format, packet_header)
        expected_len = expected_len
        try:
            packet_type = PacketType(raw_type)
        except ValueError:
            raise ValueError(f"Unknown packet type: {raw_type}")

        return packet_type, expected_len