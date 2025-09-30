import argparse
import socket
import struct

DOS_HEADER = "<HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHL"
FILE_HEADER = "<HHLLLHH"
OPTIONAL_HEADER = "<HBBLLLLQLLHHHHHHLLLLHHQQQQLL"
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

            machine_type, num_sections, *_ = struct.unpack(FILE_HEADER, f.read(struct.calcsize(FILE_HEADER)))
            print(f"Machine Type: 0x{machine_type:X}")
            print(f"Number of Sections: {num_sections}")

            optional_fields = struct.unpack(OPTIONAL_HEADER, f.read(struct.calcsize(OPTIONAL_HEADER)))

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