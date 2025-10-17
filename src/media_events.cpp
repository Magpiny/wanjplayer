
// Media events

#include "media_ctrls.hpp"
#include "playlist.hpp"
#include "wanjplayer.hpp"
#include "canvas.hpp"
#include "utils.hpp"
#include "player_ui_control.hpp"

namespace {
bool IsAudioFile(const wxString& path) {
    if (path.IsEmpty()) return false;
    wxFileName filename(path);
    wxString ext = filename.GetExt().Lower();
    return ext == "mp3" || ext == "wav" || ext == "aac" || ext == "m4a" || ext == "flac" || ext == "ogg";
}
} // namespace

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
  wxString extension = file_name.GetExt().Lower();

  // Log the file being loaded
  wxLogMessage("Attempting to load media file: %s", file_path);
  wxLogMessage("File extension: %s", extension);

  // Clear playlist and add this single file
  playlist->ClearPlayQueue();
  playlist->AddItem(file_path);
  
  // Show in status bar
  if (status_bar) {
    wxFileName fname(file_path);
    status_bar->set_system_message("Loading: " + fname.GetName());
  }
  
  // Play the file
  playlist->PlayItemAtIndex(0);
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

  // Clear existing playlist and add new files
  playlist->ClearPlayQueue();
  
  // Add files to the playlist
  for (const wxString& path : paths) {
    playlist->AddItem(path);
  }
  
  // Start playing the first file if any files were added
  if (!paths.IsEmpty()) {
    if (status_bar) {
      status_bar->set_system_message(wxString::Format("Loaded %zu files", paths.GetCount()));
    }
    playlist->PlayItemAtIndex(0);
  }
}

#include "preferences.hpp"

void
PlayerFrame::OnPreferences(wxCommandEvent& event)
{
  PreferencesDialog prefs(this);
  prefs.ShowModal();
};

void
PlayerFrame::OnLicense(wxCommandEvent& event)
{
  gui::LicenseDialog license_dialog =
    gui::LicenseDialog(this, "License Agreement");
  license_dialog.load_license("assets/LICENSE");
  license_dialog.DoLayoutAdaptation();
  license_dialog.ShowModal();
};

void
PlayerFrame::OnMediaLoaded(wxMediaEvent& event)
{
  wxMediaCtrl* media_ctrl = player_ui_control->GetMediaCtrl();
  if (media_ctrl) {
    wxFileOffset length = media_ctrl->Length();
    wxLogMessage("Media duration: %lld ms", length);
    
    // Update media controls with new duration
    if (player_ctrls) {
      player_ctrls->UpdateDuration();
    }
    
    // Update status bar with file information
    if (status_bar && playlist) {
      wxString current_file = playlist->GetCurrentItem();
      wxFileName fname(current_file);
      status_bar->update_file_info(current_file, length);
      status_bar->update_playback_info("Loaded");
      status_bar->set_system_message("Loaded: " + fname.GetName());
    }
    
    // Check if this is a video file
    wxString current_file = playlist->GetCurrentItem();

    if (utils::FileUtils::IsVideoFile(current_file)) {
      wxLogMessage("Video file detected: %s", current_file);
      if (player_ui_control) {
        player_ui_control->ShowVideoCanvas();
      }
    } else {
      wxLogMessage("Audio-only media detected");
      if (player_ui_control) {
        player_ui_control->ShowAudioCanvas();
      }
    }
  }
  event.Skip();
}

void
PlayerFrame::OnMediaStop(wxMediaEvent& event)
{
  wxLogMessage("Media stopped");
  
  // Update status bar
  if (status_bar) {
    status_bar->update_playback_info("Stopped");
    status_bar->set_system_message("Stopped");
  }
  
  if (player_ui_control) {
      player_ui_control->ShowAudioCanvas();
  }
  
    wxMediaCtrl* media_ctrl = player_ui_control->GetMediaCtrl();  if (media_ctrl) {
    media_ctrl->Refresh();
  }
  event.Skip();
}

void
PlayerFrame::OnMediaFinished(wxMediaEvent& event)
{
  wxLogMessage("Media playback finished, playing next item in playlist");
  
  if (status_bar) {
    status_bar->update_playback_info("Finished");
  }
  
  if (playlist && playlist->HasNext()) {
    playlist->PlayNextItem();
    if (status_bar) {
      status_bar->set_system_message("Next track");
    }
  } else {
    if (status_bar) {
      status_bar->set_system_message("End of playlist");
      status_bar->update_playback_info("Stopped");
    }
    if (player_ui_control) {
      player_ui_control->ShowAudioCanvas();
    }
  }
  event.Skip();
}

void
PlayerFrame::OnMediaPlay(wxMediaEvent& event)
{
  wxLogMessage("Media play event triggered");
  
  wxMediaCtrl* media_ctrl = player_ui_control->GetMediaCtrl();
  if (media_ctrl) {
    // Update duration when playback starts
    if (player_ctrls) {
      player_ctrls->UpdateDuration();
    }
    
    // Update status bar and ensure timer-based updates start
    if (status_bar) {
      status_bar->update_playback_info("Playing");
      
      // Initialize status bar with current position (should be 0s at start)
      if (playlist) {
        wxString current_file = playlist->GetCurrentItem();
        if (!current_file.IsEmpty()) {
          wxFileName fname(current_file);
          status_bar->set_system_message("Playing: " + fname.GetName());
          wxFileOffset duration = media_ctrl->Length();
          if (duration > 0) {
            wxString initial_display = "0s / " + utils::TimeFormatter::FormatTime(duration);
            status_bar->set_duration_display(initial_display);
          }
          if (player_ui_control) {
            if (utils::FileUtils::IsVideoFile(current_file)) {
              player_ui_control->ShowVideoCanvas();
            } else {
              player_ui_control->ShowAudioCanvas();
            }
          }
        }
      }
    }
  }
  event.Skip();
}

void
PlayerFrame::OnMediaPause(wxMediaEvent& event)
{
  wxLogMessage("Media paused");
  
  // Update status bar
  if (status_bar) {
    status_bar->update_playback_info("Paused");
    status_bar->set_system_message("Paused");
  }
  
  // Pause audio visualization
  // if (player_canvas && player_canvas->GetDisplayMode() == gui::PlayerCanvas::DisplayMode::AUDIO_VIS) {
  //   player_canvas->PauseAudioVisualization();
  // }
  
  event.Skip();
}



// These button event implementations are now in media_ctrls.cpp
