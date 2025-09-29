// Another file by Wanjman
#include "media_ctrls.hpp"
#include "playlist.hpp"
#include "player_ui.hpp"
#include "statusbar.hpp"
#include "wanjplayer.hpp"
#include "utils.hpp"

gui::player::MediaControls::MediaControls(wxPanel* panel,
                                          wxMediaCtrl* media_ctrl)
  : wxPanel(panel, wxID_ANY)
  , _pmedia_ctrl(media_ctrl)
  , playlist(nullptr)
  , status_bar(nullptr)
  , media_duration(0)
  , media_position(0)
  , update_timer(nullptr)
{
  if (!_pmedia_ctrl) {
    return;
  }

  SetBackgroundColour(wxColour(3, 4, 96, 100));

  // Create control buttons
  btn_play = new wxButton(this, wxID_ANY, "Play");
  btn_stop = new wxButton(this, wxID_ANY, "Stop");
  btn_pause = new wxButton(this, wxID_ANY, "Pause");
  btn_prev = new wxButton(this, wxID_ANY, "Previous");
  btn_next = new wxButton(this, wxID_ANY, "Next");

  // Create sliders
  slider_volume = new wxSlider(this, wxID_ANY, 35, 0, 100);
  slider_playback_position = new wxSlider(this, wxID_ANY, 0, 0, 100000);

  // Create time display labels
  label_current_time = new wxStaticText(this, wxID_ANY, "00:00", wxDefaultPosition, wxSize(50, -1));
  label_separator = new wxStaticText(this, wxID_ANY, "/");
  label_total_time = new wxStaticText(this, wxID_ANY, "00:00", wxDefaultPosition, wxSize(50, -1));

  // Create timer for updating playback position
  update_timer = new wxTimer(this);
  if (!update_timer) {
    return;
  }

  // Create layout
  wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
  
  // First row: playback position and time
  wxBoxSizer* position_sizer = new wxBoxSizer(wxHORIZONTAL);
  position_sizer->Add(label_current_time, 0, wxALL | wxCENTER, 2);
  position_sizer->Add(slider_playback_position, 1, wxALL | wxEXPAND, 2);
  position_sizer->Add(label_separator, 0, wxALL | wxCENTER, 2);
  position_sizer->Add(label_total_time, 0, wxALL | wxCENTER, 2);
  
  // Second row: control buttons and volume
  wxBoxSizer* controls_sizer = new wxBoxSizer(wxHORIZONTAL);
  controls_sizer->Add(btn_prev, 0, wxALL | wxCENTER, 2);
  controls_sizer->Add(btn_play, 0, wxALL | wxCENTER, 2);
  controls_sizer->Add(btn_pause, 0, wxALL | wxCENTER, 2);
  controls_sizer->Add(btn_stop, 0, wxALL | wxCENTER, 2);
  controls_sizer->Add(btn_next, 0, wxALL | wxCENTER, 2);
  controls_sizer->AddSpacer(20);
  controls_sizer->Add(new wxStaticText(this, wxID_ANY, "Volume:"), 0, wxALL | wxCENTER, 2);
  controls_sizer->Add(slider_volume, 0, wxALL | wxCENTER, 2);

  main_sizer->Add(position_sizer, 0, wxALL | wxEXPAND, 2);
  main_sizer->Add(controls_sizer, 0, wxALL | wxEXPAND, 2);

  // Set sizer for this media_control panel
  SetSizer(main_sizer);
  Layout();

  // Bind button events
  Bind(wxEVT_BUTTON, &MediaControls::OnPlay, this, btn_play->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnStop, this, btn_stop->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnPause, this, btn_pause->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnNext, this, btn_next->GetId());
  Bind(wxEVT_BUTTON, &MediaControls::OnPrevious, this, btn_prev->GetId());
  
  // Bind slider events
  Bind(wxEVT_SLIDER, &MediaControls::OnVolumeChange, this, slider_volume->GetId());
  Bind(wxEVT_SLIDER, &MediaControls::OnPositionSliderChange, this, slider_playback_position->GetId());
  
  // Bind timer event
  Bind(wxEVT_TIMER, &MediaControls::OnUpdateTimer, this, update_timer->GetId());

  // Bind hover events
  Bind(wxEVT_ENTER_WINDOW,
       &MediaControls::OnVideoCanvasHover,
       this,
       ID_MEDIA_CANVAS);
  Bind(wxEVT_LEAVE_WINDOW,
       &MediaControls::OnVideoCanvasLeave,
       this,
       ID_MEDIA_CANVAS);
       
  // Start the update timer (update every second)
  if (update_timer) {
    update_timer->Start(1000);
  }
}

gui::player::MediaControls::~MediaControls()
{
  if (update_timer) {
    update_timer->Stop();
    delete update_timer;
    update_timer = nullptr;
  }
}

void
gui::player::MediaControls::SetPlaylist(Playlist* pl)
{
  playlist = pl;
}

void
gui::player::MediaControls::SetStatusBar(gui::StatusBar* sb)
{
  status_bar = sb;
}

void
gui::player::MediaControls::UpdateDuration()
{
  if (_pmedia_ctrl) {
    media_duration = _pmedia_ctrl->Length();
    if (media_duration > 0) {
      slider_playback_position->SetMax(static_cast<int>(media_duration / 100));
      UpdateTimeDisplay();
    }
  }
}

wxString
gui::player::MediaControls::FormatTime(wxFileOffset milliseconds)
{
  return utils::TimeFormatter::FormatTime(milliseconds);
}

void
gui::player::MediaControls::UpdateTimeDisplay()
{
  label_current_time->SetLabel(utils::TimeFormatter::FormatTime(media_position));
  label_total_time->SetLabel(utils::TimeFormatter::FormatTime(media_duration));
}

void
gui::player::MediaControls::UpdatePositionSlider()
{
  if (_pmedia_ctrl && media_duration > 0) {
    media_position = _pmedia_ctrl->Tell();
    int slider_value = static_cast<int>(media_position / 100);
    slider_playback_position->SetValue(slider_value);
    UpdateTimeDisplay();
  }
}

void
gui::player::MediaControls::OnPlay(wxCommandEvent& event)
{
  if (_pmedia_ctrl) {
    _pmedia_ctrl->Play();
  }
}

void
gui::player::MediaControls::OnStop(wxCommandEvent& event)
{
  if (_pmedia_ctrl) {
    _pmedia_ctrl->Stop();
    media_position = 0;
    slider_playback_position->SetValue(0);
    UpdateTimeDisplay();
  }
}

void
gui::player::MediaControls::OnPause(wxCommandEvent& event)
{
  if (_pmedia_ctrl) {
    _pmedia_ctrl->Pause();
  }
}

void
gui::player::MediaControls::OnVolumeChange(wxCommandEvent& event)
{
  if (_pmedia_ctrl) {
    double media_volume = slider_volume->GetValue() / 100.0;
    _pmedia_ctrl->SetVolume(media_volume);
  }
}

void
gui::player::MediaControls::OnNext(wxCommandEvent& event)
{
  if (playlist && !playlist->IsEmpty() && playlist->HasNext()) {
    playlist->PlayNextItem();
  }
}

void
gui::player::MediaControls::OnPrevious(wxCommandEvent& event)
{
  if (playlist && !playlist->IsEmpty() && playlist->HasPrevious()) {
    playlist->PlayPreviousItem();
  }
}

void
gui::player::MediaControls::OnPositionSliderChange(wxCommandEvent& event)
{
  if (_pmedia_ctrl && media_duration > 0) {
    int slider_value = slider_playback_position->GetValue();
    wxFileOffset new_position = static_cast<wxFileOffset>(slider_value) * 100;
    
    // Seek immediately when slider changes
    if (_pmedia_ctrl->Seek(new_position)) {
      media_position = new_position;
      UpdateTimeDisplay();
    }
  }
}

void
gui::player::MediaControls::OnPositionSliderSeek(wxMouseEvent& event)
{
  // This method is now unused - seeking happens in OnPositionSliderChange
  event.Skip();
}

void
gui::player::MediaControls::OnUpdateTimer(wxTimerEvent& event)
{
  if (_pmedia_ctrl) {
    // Update media duration if it changed
    wxFileOffset current_duration = _pmedia_ctrl->Length();
    if (current_duration != media_duration && current_duration > 0) {
      media_duration = current_duration;
      slider_playback_position->SetMax(static_cast<int>(media_duration / 100));
    }
    
    // Update position slider and time display
    UpdatePositionSlider();
    
    // Update status bar duration counter every second
    if (status_bar && _pmedia_ctrl && media_duration > 0) {
      // Get current position for real-time counter
      wxFileOffset current_pos = _pmedia_ctrl->Tell();
      
      // Create a combined duration string showing current/total time
      wxString duration_str = utils::TimeFormatter::FormatDuration(current_pos, media_duration);
      
      // Update just the duration column with current position
      status_bar->set_duration_display(duration_str);
    } else if (status_bar) {
      // If no media is playing, ensure status bar shows default
      status_bar->set_duration_display("0s");
    }
  }
}

void
gui::player::MediaControls::OnVideoCanvasHover(wxMouseEvent& event)
{
  this->Show();
  this->Refresh();
  event.Skip();
}

void
gui::player::MediaControls::OnVideoCanvasLeave(wxMouseEvent& event)
{
  this->Hide();
  this->Refresh();
  event.Skip();
}
