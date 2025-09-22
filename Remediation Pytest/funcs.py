def add(a, b): 
    return a+b

def divide(a, b):
    if (b == 0):
        return ZeroDivisionError
    else:
        return a/b
    
def port_type(port_str: str) -> int:
    port = int(port_str)
    if port < 0 or port > 65535:
        raise ValueError(f"{port} is outside of range [0-65535]")
    return port