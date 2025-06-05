#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

void load_vector(std::vector<std::string>& v, const fs::path& path);

#endif 
