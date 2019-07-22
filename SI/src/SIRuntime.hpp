
#ifndef SI_SIRUNTIME_HPP
#define SI_SIRUNTIME_HPP

#define xlib 1

#ifdef __cplusplus

extern "C"
{
int SI_start(const char* plugin_path);
int SI_pause();
int SI_end();
}
#endif


#endif //SI_SIRUNTIME_HPP
