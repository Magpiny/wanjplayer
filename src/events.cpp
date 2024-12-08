#include "about.hpp"
#include "license_dlgbx.hpp"
#include "main_window.hpp"
#include "media_ctrl.hpp"

void MainWindow::OnExit(wxCommandEvent &event) { Close(true); };

void MainWindow::OnAbout(wxCommandEvent &event) {
  About *about_dlg = new About(this);
  about_dlg->create_about_page();
};

void MainWindow::OnMouseEnter(wxMouseEvent &event) {
  //_media_ctrl->Show();
  // _media_ctrl->Raise();
  event.Skip();
}

void MainWindow::OnMouseLeave(wxMouseEvent &event) {
  //_media_ctrl->Hide();
  event.Skip();
}

void MainWindow::OnFileOpen(wxCommandEvent &event) {
  wxFileDialog file_dialog(
      this, _("Open Media File"), "", "",
      "Media files "
      "(*.mp3;*.mp4;*.mkv;*.avi;*.wav;*.aac;*.m4a;*.flac;*.ogg)|*.avi;*.mp3;*."
      "mp4;*.mkv;*.wav;*.aac;*.m4a;*.flac;*.ogg",
      wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (file_dialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  // get the file path dir
  wxString file_path = file_dialog.GetPath();

  // Get file name
  wxFileName file_name(file_path);

  // Play media file
  if (_media_pane) {
    _media_pane->Play(file_path);

  } else {
    wxLogError("Pane NOT Ready!");
  }
};

void MediaCtrl::OnPlayPause(wxCommandEvent &event) { /* Implement Me */ };
void MediaCtrl::OnStop(wxCommandEvent &event) { /* Implement Me */ };
void MediaCtrl::OnNext(wxCommandEvent &event) { /* Implement Me */ };
void MediaCtrl::OnPrevious(wxCommandEvent &event) { /* Implement Me */ };
void MediaCtrl::OnVolumeChange(wxCommandEvent &event) { /* Implement Me */
};
void MediaCtrl::OnSeekChange(wxCommandEvent &event) { /* Implement Me */ };
