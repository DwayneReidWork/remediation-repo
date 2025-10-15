import struct

def main():
    # Step 1: Create a sample binary file
    sample_path = "sample_header.bin"
    make_sample_file(sample_path)

    # Step 2: Read and unpack the file header
    header = read_header(sample_path)

    # Step 3: Print fields nicely
    # TODO: print each field clearly
    print("Magic:", header[0])
    print("Version:", header[1])
    print("Flags:", bin(header[2]))
    print("Record Count:", header[3])

# === Constants ===
HEADER_FORMAT = "<4sBBH"  # little-endian: 4-char string, 2 bytes, 2-byte short
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)

def read_header(file_path: str):
    """
    Reads and unpacks the first 8 bytes of a binary file.

    :param file_path: path to binary file
    :return: tuple (magic, version, flags, record_count)
    """
    with open(file_path, "rb") as f:
        header_bytes = f.read(HEADER_SIZE)

        # TODO: Check file length
        if len(header_bytes) < HEADER_SIZE:
            raise ValueError("File too short for header")

        # TODO: Use struct.unpack to decode header_bytes
        unpacked = struct.unpack(HEADER_FORMAT, header_bytes)

        # TODO: Return unpacked tuple
        return unpacked


def make_sample_file(file_path: str):
    """
    Creates a small binary file with a fake header.
    """
    magic = b"META"
    version = 1
    flags = 0b10101010
    record_count = 42

    # TODO: Use struct.pack to create binary header
    header_bytes = struct.pack(HEADER_FORMAT, magic, version, flags, record_count)

    with open(file_path, "wb") as f:
        # TODO: Write the packed header to file
        f.write(header_bytes)
        pass

if __name__ == "__main__":
    main()