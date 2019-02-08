#pragma once
#include <gtkmm/builder.h>
#include <gtkmm/entry.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/window.h>

#include "imagewriter.hpp"
#include "imagereader.hpp"

class MainWindow : public Gtk::Window {
public:
  MainWindow(BaseObjectType *                  cobject,
             const Glib::RefPtr<Gtk::Builder> &builder);
  virtual ~MainWindow();

private:
  void                    onFileSetClicked();
  void                    onStartClicked();
  Gtk::FileChooserButton *openButton;
  Gtk::RadioButton *      mashRadio, *recoverRadio;
  Gtk::Button *           startButton, *setPathButton;
  Gtk::Window *           progressWindow;
  Gtk::Entry *            savePathEntry;
};
