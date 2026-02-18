#ifndef DRIVER_H
#define DRIVER_H

#include <string>

std::string read_file(const char *file_name);
std::string try_read_file(const int argc, char* argv[]);

#endif //DRIVER_H