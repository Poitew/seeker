#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include "../include/utils.h"
namespace fs = std::filesystem;

int main() {
  using namespace ftxui;

  int previous_selection {0};
  int local_selection {0};
  int next_selection {0};
  fs::path current_path = fs::current_path();
  bool next_dir_valid = false;
  
  std::vector<std::string> previous_dir {};
  std::vector<std::string> local_dir {};
  std::vector<std::string> next_dir {};

  load_vector(local_dir, current_path);
  
  if (current_path.has_parent_path()) {
    load_vector(previous_dir, current_path.parent_path());
  }

  
  auto previous_menu  {Menu(&previous_dir, &previous_selection)};
  auto local_menu     {Menu(&local_dir, &local_selection)};
  auto next_menu      {Menu(&next_dir, &next_selection)};

  
  Component container = Container::Horizontal({previous_menu, local_menu, next_menu});
  container->SetActiveChild(local_menu);

  
  auto renderer = Renderer(container, [&] {
    return window(
		  text("  Current path: " + current_path.string() + "  "),
		  hbox({
		      previous_menu->Render() | frame | flex | size(HEIGHT, EQUAL, 20),
		      local_menu   ->Render() | frame | flex | size(HEIGHT, EQUAL, 20),
		      next_menu    ->Render() | frame | flex | size(HEIGHT, EQUAL, 20),
		    })   
		  );
  });

  
  auto app = CatchEvent(renderer, [&](Event e) {

    if (e == Event::ArrowUp || e == Event::ArrowDown) {
      local_menu->OnEvent(e);

      if (!local_dir.empty()) {
	const std::string& name = local_dir.at(local_selection);
	fs::path selected_path = current_path / name;

	if (fs::is_directory(selected_path)) {
	  static fs::path last_loaded_path;
	  if (selected_path != last_loaded_path || !next_dir_valid) {
	    load_vector(next_dir, selected_path);
	    next_dir_valid = true;
	    last_loaded_path = selected_path;
	  }
	}
	else {
	  if (!next_dir.empty()) {
	    next_dir.clear();
	    next_dir_valid = false;
	  }
	}
      }
      else {
	if (!next_dir.empty()) {
	  next_dir.clear();
	  next_dir_valid = false;
	}
      }

      return true;
    }



    if (e == Event::ArrowLeft) {
      current_path = current_path.parent_path();

      local_dir.clear();
      load_vector(local_dir, current_path);

      previous_dir.clear();
      if (current_path.has_parent_path()) {
        load_vector(previous_dir, current_path.parent_path());
      }

      next_dir.clear();

      if (!local_dir.empty()) {
	fs::path first_entry {current_path / local_dir.at(0)};
	if(fs::is_directory(first_entry)){
	  load_vector(next_dir, first_entry);
	  next_dir_valid = true;
	}
      }

      // If we're at root, do nothing
      if (!current_path.has_parent_path() || current_path == current_path.parent_path()) {
	previous_dir.clear();
	next_dir.clear();
	return true;
      }

      next_dir_valid = false;
      container->SetActiveChild(local_menu);
      return true;
    }

    if (e == Event::ArrowRight) {
      if (!local_dir.empty()) {
        const std::string name = local_dir.at(local_selection);
        fs::path selected_path = current_path / name;

	// We could get permission denied error
	try{
	  if (fs::is_directory(selected_path) && !fs::is_empty(selected_path)) {
	    current_path = selected_path;

	    local_dir.clear();
	    load_vector(local_dir, current_path);

	    previous_dir.clear();
	    if (current_path.has_parent_path()) {
	      load_vector(previous_dir, current_path.parent_path());
	    }
	  
	    next_dir.clear();

	    if (!local_dir.empty()) {
	      fs::path first_entry {current_path / local_dir.at(0)};
	      if(fs::is_directory(first_entry)){
		load_vector(next_dir, first_entry);
		next_dir_valid = true;
	      }
	    }
	  }
	}
	catch(std::exception& e){
	  std::cerr << "Could not open directory. " << e.what() << "\n";
	}
      }

      next_dir_valid = false;
      container->SetActiveChild(local_menu);
      return true;
    }

    return false;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(app);
}
