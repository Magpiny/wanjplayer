
// Media events

#include "wanjplayer.hpp"

void
PlayerFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
};

void
PlayerFrame::OnAbout(wxCommandEvent& event)
{
  std::unique_ptr<gui::About> about = std::make_unique<gui::About>();
  about->create_about_page();
};

void
PlayerFrame::OnFileOpen(wxCommandEvent& event)
{
  wxFileDialog open_file_dialog(
    this,
    _("Open Media File"),
    "",
    "",
    "Media files "
    "(*.mp3;*.mp4;*.mkv;*.avi;*.wav;*.aac;*.m4a;*.flac;*.ogg)|*.avi;*.mp3;*."
    "mp4;*.mkv;*.wav;*.aac;*.m4a;*.flac;*.ogg",
    wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (open_file_dialog.ShowModal() == wxID_CANCEL) {
    return;
  }

  // get the file path dir
  wxString file_path = open_file_dialog.GetPath();

  // Check if the media is audio or video
  wxFileName file_name(file_path);

  // Play media
  if (media_ctrl && media_ctrl->Load(file_path)) {

    media_ctrl->Play();
    media_ctrl->ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_VOLUME);

  } else {
    wxLogError("Failed to load media");
  }
}

void
PlayerFrame::OnPreferences(wxCommandEvent& event)
{
  wxMessageBox(
    "Opens App Preferences", "App preferences", wxOK | wxICON_INFORMATION);
};

void
PlayerFrame::OnLicense(wxCommandEvent& event)
{
  gui::LicenseDialog license_dialog =
    gui::LicenseDialog(this, "License Agreement");
  license_dialog.load_license("../assets/LICENSE");
  license_dialog.DoLayoutAdaptation();
  license_dialog.ShowModal();
};

void
PlayerFrame::OnMediaLoaded(wxCommandEvent& event)
{
  canvas_banner->Hide();
  player_ctrls->Show();
  media_ctrl->Show();
}

void
PlayerFrame::OnMediaStop(wxCommandEvent& event)
{
  // Show the static bitmap when media stops playing
  canvas_banner->Show();
  if (default_image->IsOk()) {
    canvas_banner->SetBitmap(wxBitmapBundle::FromImage(*default_image));
  }
  canvas_banner->Refresh();
}

void
PlayerFrame::OnMediaUnload(wxCommandEvent& event)
{
  if (audio_banner->IsOk())
    canvas_banner->SetBitmap(wxBitmapBundle::FromSVGFile(
      "../assets/logo/wanjplayer-256x256.svg", wxSize(200, 200)));

  media_ctrl->Hide();
  player_ctrls->Show();
  canvas_banner->Show();
  canvas_banner->Refresh();
}

void
PlayerFrame::OnMediaPlay(wxCommandEvent& event)
{
  player_ctrls->Hide();
  media_ctrl->Show();
}

void
PlayerFrame::OnMediaPause(wxCommandEvent& event)
{
  player_ctrls->Show();
}
