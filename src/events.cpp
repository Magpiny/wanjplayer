#include "about.hpp"
#include "license_dlgbx.hpp"
#include "main_window.hpp"
#include "media_ctrl.hpp"

void MainWindow::OnExit(wxCommandEvent &event) { Close(true); };

void MainWindow::OnAbout(wxCommandEvent &event) {
  /*About* about_dlg = new About(this);*/
  /*about_dlg->create_about_page();*/
};

void MainWindow::OnMouseEnter(wxMouseEvent &event) {
  /*  _media_ctrl->Show();*/
  /*_media_ctrl->Raise();*/
}

void MainWindow::OnMouseLeave(wxMouseEvent &event) { _media_ctrl->Hide(); }

void MediaCtrl::OnPlayPause(wxCommandEvent &event) { /* Implement Me */ }
void MediaCtrl::OnStop(wxCommandEvent &event) { /* Implement Me */ }
void MediaCtrl::OnNext(wxCommandEvent &event) { /* Implement Me */ }
void MediaCtrl::OnPrevious(wxCommandEvent &event) { /* Implement Me */ }
void MediaCtrl::OnVolumeChange(wxCommandEvent &event) { /* Implement Me */
};
void MediaCtrl::OnSeekChange(wxCommandEvent &event) { /* Implement Me */ };