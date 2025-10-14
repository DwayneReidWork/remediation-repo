from funcs import add, divide, port_type
import pytest

@pytest.mark.parametrize("a, b, output", [(1,2,3),
                                          (-1,-2,-3),
                                          (-1,2,1),
                                          (999,999,1998)])
def test_add_function(a, b, output):
    """
    Test the add function
    """
    assert add(a, b) == output

@pytest.mark.parametrize("a, b, output", [(4,2,2),
                                          (-10,-2,5),
                                          (1,0,ZeroDivisionError),
                                          (1000,-2,-500)])
def test_divide_function(a, b, output):
    """
    Test the division function
    """
    assert divide(a, b) == output

@pytest.mark.parametrize("port_string, output", [("8080", 8080),
                                                  ("1234", 1234)])
def test_port_type_function(port_string, output):
    #Test port_type function with valid params
    assert port_type(port_string) == output

@pytest.mark.parametrize("port_string, output", [("-1", ValueError),
                                                 ("67000", ValueError)])
def test_port_type_function_invalid_port(port_string, output):
    #Test port_type function with invalid params
    with pytest.raises(ValueError):
        assert port_type(port_string) == output