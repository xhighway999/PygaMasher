#include "mainwindow.hpp"

MainWindow::MainWindow(BaseObjectType *                  cobject,
                       const Glib::RefPtr<Gtk::Builder> &builder)
    : Gtk::Window(cobject) {
  builder->get_widget("mashRadio", mashRadio);
  builder->get_widget("recoverRadio", recoverRadio);
  builder->get_widget("fileLoadButton", openButton);
  builder->get_widget("setPathButton", setPathButton);
  builder->get_widget("startButton", startButton);
  builder->get_widget("savePathEntry", savePathEntry);

  startButton->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::onStartClicked));
  setPathButton->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::onFileSetClicked));
}

MainWindow::~MainWindow() {}

void MainWindow::onFileSetClicked() {
  Gtk::FileChooserDialog dialog("Select a Save Path",
                                Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for(*this);
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("Save", Gtk::RESPONSE_OK);

  int result = dialog.run();

  if(result == Gtk::RESPONSE_OK) {
    savePathEntry->set_text(dialog.get_filename());
  }
}

void MainWindow::onStartClicked() {
  std::string openFile = openButton->get_filename();
  std::string saveFile = savePathEntry->get_text();
  bool        mash     = mashRadio->get_active();

  auto mashFunc = [](std::string in, std::string out) {
    ImageWriter writer;
    writer.setOutputFile(out);

    std::ifstream file(in);
    writer.start();
    char data[512];
    do {
      file.read(data, 512);
      writer.writeData(data, file.gcount());
    } while(!file.eof());

    writer.end();
  };
  auto recoverFunc = [](std::string in, std::string out) {
    ImageReader reader;
    reader.setInputFile(in);
    reader.setOutputFile(out);
    reader.start();
  };

  if(mash)
    mashFunc(openFile, saveFile);
  else
    recoverFunc(openFile, saveFile);

  Gtk::MessageDialog dialog("Conversion complete");
  dialog.set_secondary_text("Press OK to close this window");
  dialog.run();
}
