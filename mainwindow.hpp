#pragma once
#include "imagereader.hpp"
#include "imagewriter.hpp"
#include "rsdecoder.hpp"
#include "rsencoder.hpp"

#include <gtkmm/builder.h>
#include <gtkmm/entry.h>
#include <gtkmm/filechooserbutton.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/label.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/textview.h>
#include <gtkmm/window.h>

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
  Gtk::Entry *            savePathEntry;
};
