from csv_parser import importCSV

import csv
import pytest

@pytest.fixture
def csv_data():
    """Mock struct data"""
    data = [
    {"pid": 1, "name": "alice", "dob": "1/1/91", "job_title": "accountant"},
    {"pid": 2, "name": "bob", "dob": "2/2/92", "job_title": "bookkeeper"},
    {"pid": 3, "name": "charles", "dob": "3/3/93", "job_title": "chef"},
    {"pid": 4, "name": "dwayne", "dob": "12/5/91", "job_title": "Programmer"},
    {"pid": 5, "name": "Mike", "dob": "12/12/12", "job_title": "Gamer"},
    {"pid": 6, "name": "Not Kevin", "dob": "2/2/22", "job_title": "Homeless"},
    {"pid": 7, "name": "Tyler", "dob": "4/4/24", "job_title": "Clown"}
    ]
    return data

# Fixture to create a temporary CSV file for testing
@pytest.fixture()
def test_file(tmp_path, csv_data):
    """
    tmp_path: pytest built-in fixture providing a temporary directory
    csv_data: the test data provided by the previous fixture
    """
    # Create a path for a temporary CSV file inside the temp directory
    file_path = tmp_path / "test_file.csv"

    # Open the file in write mode
    with open(file_path, "w", newline="") as f:
        # Use DictWriter to write dictionary data into CSV format
        writer = csv.DictWriter(f, fieldnames=csv_data[0].keys())
        writer.writeheader()      # Write CSV header row
        writer.writerows(csv_data)  # Write all rows from the fixture data

    # Return the path to the temporary CSV file for tests to use
    return file_path

@pytest.fixture
def invalid_file():
    """Returns invalid filename for test"""
    return "fake_file.csv"

#Test with valid mock file 
def test_importCSV(test_file):
    """
    test_file: path to the temporary CSV file created by test_file fixture
    """
    result = importCSV(str(test_file), pk="pid")

    # importCSV should return something
    assert result is not None      
    assert len(result) == 7              
    # PID is string 0 index after that is category name for comparison
    assert result['1']['name'] == 'alice'
    assert result['2']['name'] == 'bob'
    assert result['7']['name'] == 'Tyler'

# Import with invalid file name
def test_importCSV_invalid_file(invalid_file):
    result = importCSV(invalid_file)
    assert result == None