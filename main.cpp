#include "mainwindow.hpp"

#include <gtkmm.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

int main(int argc, char *argv[]) {
  MainWindow *                   window;
  Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create(argc, argv, "PygaMasher");
  Glib::RefPtr<Gtk::Builder> builder =
      Gtk::Builder::create_from_file("PygaGui.glade");

  builder->get_widget_derived("window", window);

  return app->run(*window);
}
