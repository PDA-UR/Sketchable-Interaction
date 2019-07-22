
#ifndef SI_SIRUNTIME_HPP
#define SI_SIRUNTIME_HPP

#ifdef __cplusplus

extern "C"
{
int SI_initialize(const char* plugin_path);
int SI_start();
int SI_pause();
int SI_end();
}
#endif


#endif //SI_SIRUNTIME_HPP
