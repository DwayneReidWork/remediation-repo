import regex

"""
I want you to regex parse the ip, the username (- is a username where the username is unknown), the timestamp, the method (POST, DELETE, GET, PUT), api endpoint (/api/data, /settings) the status code and the response size.

Then I want to know every 403 forbidden done by frank
"""
pattern = regex.compile(r'(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])\.(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9]) - (.*?) \[(\d\d\/\w*\/\d*:\d\d:\d\d:\d\d) -\d*\] \"(\w+) (\/[^\s]*) HTTP\/[0-9.]+" (\d{3}) (\d+)')

def main():
    data = load_file()
    #user_search(data) #returns all usernames
    #timestamp_search(data)
    http_search(data) #search for get/put/post/delete
    #api_endpoint_search(data)
    #response_size_search(data)
    #frank_search(data) #search for all frank 403 errors

def load_file():
    try:
        with open("sample_clf_log.txt", 'r') as log_file:
            data = log_file.read()
    except FileNotFoundError:
        raise FileNotFoundError
    return data
    


def user_search(data):
    for line in data.strip().split("\n"):
        match = pattern.search(line)
        if match:
            print(match.group(5))


def timestamp_search(data):
    for line in data.strip().split("\n"):
        match = pattern.search(line)
        if match:
            print(match.group(6))

def http_search(data):  
    while True:
        u_input = input(
            "Please enter the number of the request type you'd like to search:\n"
            "1. GET\n2. PUT\n3. POST\n4. DELETE\n\n-> "
        )

        try:
            u_input = int(u_input)
            if u_input not in (1, 2, 3, 4):
                print("Invalid selection. Please choose 1-4.")
                continue
            break
        except ValueError:
            print("Invalid input. Please enter a number between 1 and 4.")
            continue

    for line in data.strip().split("\n"):
        found = pattern.search(line)
        if found:
            http_method = found.group(7)
            match(u_input):
                case 1:
                    if http_method == "GET":
                        print(line)
                    continue
                case 2:
                    if http_method == "PUT":
                        print(line)
                    continue
                case 3:
                    if http_method == "POST":
                        print(line)
                    continue
                case 4:
                    if http_method == "DELETE":
                        print(line)
                    continue
                case _:
                    return "Invalid Selection"
            
def api_endpoint_search(data):
    for line in data.strip().split("\n"):
        match = pattern.search(line)
        if match:
            print(match.group(8))

def response_size_search(data):
    for line in data.strip().split("\n"):
        match = pattern.search(line)
        if match:
            print(match.group(10))  

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