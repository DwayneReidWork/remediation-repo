from funcs import add, divide, port_type
import pytest

<<<<<<< HEAD
@pytest.mark.parametrize("a, b, output", [(1,2,3),
                                          (-1,-2,-3),
                                          (-1,2,1),
                                          (999,999,1998)])
=======
@pytest.mark.parametrize("a, b, output", [(1,2,3), (-1,-2,-3), (-1,2,1), (999,999,1998)])
>>>>>>> Mem_Mgmt_and_File_IO
def test_add_function(a, b, output):
    """
    Test the add function
    """
    assert add(a, b) == output

<<<<<<< HEAD
@pytest.mark.parametrize("a, b, output", [(4,2,2),
                                          (-10,-2,5),
                                          (1,0,ZeroDivisionError),
                                          (1000,-2,-500)])
=======
@pytest.mark.parametrize("a, b, output", [(4,2,2), (-10,-2,5), (1,0,ZeroDivisionError), (1000,-2,-500)])
>>>>>>> Mem_Mgmt_and_File_IO
def test_divide_function(a, b, output):
    """
    Test the division function
    """
    assert divide(a, b) == output

<<<<<<< HEAD
@pytest.mark.parametrize("port_string, output", [("8080", 8080)
                                                  ("1234", 1234)])
=======
@pytest.mark.parametrize("port_string, output", [("8080", 8080), ("1234", 1234)])
>>>>>>> Mem_Mgmt_and_File_IO
def test_port_type_function(port_string, output):
    #Test port_type function with valid params
    assert port_type(port_string) == output

<<<<<<< HEAD
@pytest.mark.parametrize("port_string, output", [("-1", ValueError)
                                                 ("67000", ValueError)])
=======
@pytest.mark.parametrize("port_string, output", [("-1", ValueError),("67000", ValueError)])
>>>>>>> Mem_Mgmt_and_File_IO
def test_port_type_function_invalid_port(port_string, output):
    #Test port_type function with invalid params
    with pytest.raises(ValueError):
        assert port_type(port_string) == output