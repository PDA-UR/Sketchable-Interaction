

#ifndef SITEST_IOMANAGER_H
#define SITEST_IOMANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

int iomanager_read_file_to_buffer(char* buffer, int* size, const char* filepath);

#ifdef __cplusplus
}
#endif

#endif //SITEST_IOMANAGER_H
