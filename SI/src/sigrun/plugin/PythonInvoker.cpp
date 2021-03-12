#include "PythonInvoker.hpp"


bool __si_evaluate_syntax_error__(const std::string& error)
{
    return error.find("SyntaxError") != std::string::npos;
}
