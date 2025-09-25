import regex

"""
I want you to regex parse the ip, the username (- is a username where the username is unknown), the timestamp, the method (POST, DELETE, GET, PUT), api endpoint (/api/data, /settings) the status code and the response size.

Then I want to know every 403 forbidden done by frank
"""
pattern = regex.compile(r'(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]) - (.*?) \[(\d\d\/\w*\/\d*:\d\d:\d\d:\d\d) -\d*\] \"(\w+) (\/[^\s]*) HTTP\/[0-9.]+" (\d{3}) (\d+)')

def main():
    data = load_file()
    #http_search(data)
    frank_search(data)


def load_file():
    try:
        with open("sample_clf_log.txt", 'r') as log_file:
            data = log_file.read()
    except FileNotFoundError:
        raise FileNotFoundError
    return data
    

def http_search(data):
    for line in data.strip().split("\n"):
        match = pattern.search(line)
        if match:
            http_method = match.group(7)
            if http_method == "GET":
                print(line)

def frank_search(data):
    for line in data.strip().split("\n"):
        match = pattern.search(line)
        if match:
            name = match.group(5)
            error_code = match.group(9)          
            if name == "frank":
                if error_code == "403":
                    print(line)

if __name__ == "__main__":
    main()