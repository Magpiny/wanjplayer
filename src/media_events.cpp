
// Media events

#include "media_ctrls.hpp"
#include "playlist.hpp"
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

    media_length = media_ctrl->Length();
    current_position = media_ctrl->Tell();

    player_statusbar->create_statusbar(
      "hello world", tm->time_convert(media_length), file_name.GetName());

  } else {
    wxLogError("Failed to load media");
  }
};

void
PlayerFrame::OnFilesOpen(wxCommandEvent& event)
{
  wxFileDialog open_file_dialog(
    this,
    _("Open files"),
    "",
    "",
    "Media files "
    "(*.mp3;*.mp4;*.mkv;*.avi;*.wav;*.aac;*.m4a;*.flac;*.ogg;*.3gp)|*.avi;*."
    "mp3;*."
    "mp4;*.mkv;*.wav;*.aac;*.m4a;*.flac;*.ogg;*.3gp",

    wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);

  if (open_file_dialog.ShowModal() == wxID_CANCEL)
    return;

  wxArrayString paths;
  open_file_dialog.GetPaths(paths);

  // Add files to the playlist
  for (const wxString& path : paths) {
    playlist->add_item(path);

    playlist->play_next_item_in_queue();

    // update the status bar
    file_name.FileName(path);
  }

  // Bind event to play next item when current one finishes
  Bind(wxEVT_MEDIA_FINISHED,
       &PlayerFrame::OnMediaFinished,
       this,
       ID_MEDIA_FINISHED);
  playlist->play_next_item_in_queue();
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
PlayerFrame::OnMediaLoaded(wxMediaEvent& event)
{
  // playlist->play_next_item_in_queue();
}

void
PlayerFrame::OnMediaStop(wxMediaEvent& event)
{
  // Show the static bitmap when media stops playing
  media_ctrl->Refresh();
}

void
PlayerFrame::OnMediaFinished(wxMediaEvent& event)
{
  playlist->play_next_item_in_queue();

  event.Skip();
}

void
PlayerFrame::OnMediaPlay(wxMediaEvent& event)
{
  // Display media file name and the playback rate
}

void
PlayerFrame::OnMediaPause(wxMediaEvent& event)
{
  player_ctrls->Show();
}

// Button events

void
gui::player::MediaControls::OnPlay(wxCommandEvent& event)
{
  _pmedia_ctrl->Play();
};

void
gui::player::MediaControls::OnStop(wxCommandEvent& event)
{
  _pmedia_ctrl->Stop();
};

void
gui::player::MediaControls::OnPause(wxCommandEvent& event)
{
  _pmedia_ctrl->Pause();
};

void
gui::player::MediaControls::OnVolumeChange(wxCommandEvent& event)
{
  double media_volume = slider_volume->GetValue() / 100.0;
  _pmedia_ctrl->SetVolume(media_volume);
};

void
gui::player::MediaControls::OnNext(wxCommandEvent& event) {};

void
gui::player::MediaControls::OnPrevious(wxCommandEvent& event) {};
