#include "../include/utils.h"
#include <iostream>

void load_vector(std::vector<std::string>& v, const fs::path& path) {
  v.clear();
  try {
    for (auto const& dir_entry : fs::directory_iterator{path}) {
      std::string dir_name = dir_entry.path().filename().string();
      if (dir_name.at(0) == '.') continue; // hide dotfiles
      if (dir_name.length() >= 35) dir_name = dir_name.substr(0, 20);
      
      v.push_back(dir_name);
    }
  }
  catch (std::exception& e) {
    std::cerr << "Could not read file " << e.what() << "\n";
    v.clear();
  }
}
