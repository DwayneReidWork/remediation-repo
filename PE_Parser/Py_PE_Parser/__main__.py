import argparse
import socket
import struct


"""
More work with structs like these?
Will this look like a general struc.unpack format and just pass in whatever of these formates depending on recvd packet type?

PING_PACK = "<"BB"
CONNECT-PACK "<BHL"

def custom_unpack(format_str, file_stream):
    if format_str == PING_PACK:
        do stuff
    elif format string == CONNECT_PACK:
        do more stuff
    ...
    
How to from socket?

"""

FILE_HEADER = "<HHLLLHH"
OPTIONAL_HEADER = "<HBBLLLLQLLHHHHHHLLLLHHQQQQLL"
DOS_HEADER = "<HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHL"
NT_HEADER =f"<L{FILE_HEADER}{OPTIONAL_HEADER}"

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-exe", help="Path to the executable you'd like to parse", required=True)
    args = parser.parse_args()

    print(f"filename recieved {args.exe}")
    parsefile(filename=args.exe)

def parsefile(filename): 
    header_size = struct.calcsize(DOS_HEADER)
    try:
        with open(filename, 'rb') as f:
            pe_data = f.read(header_size)
            fields = struct.unpack(DOS_HEADER, pe_data)
            e_magic = socket.ntohs(fields[0])
            e_lfanew = fields[30]

            print(f"Hex E_magic: 0x{e_magic:X}")
            #print(f"E_lfanew: {e_lfanew}")

            #Seek to next starting point based on offset
            f.seek(e_lfanew)
            # Verify Signature
            pe_signature = f.read(4)
            if pe_signature != b'PE\x00\x00':
                raise ValueError(f"Invalid PE signature: {pe_signature}")  

            # Unpack struct set first to fields to variables drain rest
            machine_type, num_sections, *_ = struct.unpack(FILE_HEADER, f.read(struct.calcsize(FILE_HEADER)))
            print(f"Machine Type: 0x{machine_type:X}")
            print(f"Number of Sections: {num_sections}")

            #Unpack struct to variable since the fields we want are deep in there
            optional_fields = struct.unpack(OPTIONAL_HEADER, f.read(struct.calcsize(OPTIONAL_HEADER)))

            #Access fields by index
            print(f"Alignment: {optional_fields[10]}")
            print(f"Image_size: {optional_fields[18]}")
    except FileNotFoundError:
        print(f"Error: File not found at {filename}")
    except struct.error as e:
        print(f"Error unpacking DOS header: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")

    
if __name__ == "__main__":
    main()