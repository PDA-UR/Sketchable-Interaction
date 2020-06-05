

#ifndef SI_SIGRUN_PythonGlobalInterpreterLockGuard
#define SI_SIGRUN_PythonGlobalInterpreterLockGuard

#include <sigrun/SIObject.hpp>
#include <Python.h>

class PythonGlobalInterpreterLockGuard : public SIObject
{ SIGRUN

public:
    PythonGlobalInterpreterLockGuard()
    {
        __state__ = PyGILState_Ensure();
    }

    ~PythonGlobalInterpreterLockGuard()
    {
        PyGILState_Release(__state__);
    }

    PythonGlobalInterpreterLockGuard(const PythonGlobalInterpreterLockGuard&) = delete;
    PythonGlobalInterpreterLockGuard& operator =(const PythonGlobalInterpreterLockGuard&) = delete;

private:
    PyGILState_STATE __state__;
};

#endif // SI_SIGRUN_PythonGlobalInterpreterLockGuard