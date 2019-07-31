
#ifndef SI_SIENGINE_HPP
#define SI_SIENGINE_HPP

#include <core/Core.hpp>

#define xlib 1

#ifdef __cplusplus

extern "C"
{

int SI_start(const char* plugin_path);
int SI_pause();
int SI_end();

Core* p_core;
}
#endif


#endif //SI_SIENGINE_HPP
