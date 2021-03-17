#include "PythonInvoker.hpp"


bool __si_evaluate_syntax_error__(const std::string& error, const std::string& to_search)
{
    return error.find(to_search) != std::string::npos;
}
