#include "playlist.hpp"

gui::player::Playlist::Playlist(wxWindow* parent, wxWindowID id)
  : wxListBox(parent,
              id,
              wxDefaultPosition,
              wxDefaultSize,
              0,
              nullptr,
              wxLB_MULTIPLE | wxLB_SORT)
{
  Bind(wxEVT_KEY_DOWN, &Playlist::OnKeyDown, this);
  Bind(wxEVT_LISTBOX_DCLICK, &Playlist::OnDoubleClick, this);
};

wxMediaCtrl*
gui::player::Playlist::GetMediaCtrl()
{
  return dynamic_cast<wxMediaCtrl*>(GetParent()->FindWindowById(ID_MEDIA_CTRL));
}

void
gui::player::Playlist::add_item(const wxString& path)
{
  wxFileName file_name(path);
  play_queue.push_back(path);
  Append(file_name.GetFullName());
};

void
gui::player::Playlist::clear_play_queue()
{
  play_queue.clear();
  Clear();
};

wxString
gui::player::Playlist::get_item(size_t index) const
{
  if (index < play_queue.size()) {
    return play_queue[index];
  }
  return "";
};

size_t
gui::player::Playlist::get_count() const
{
  return play_queue.size();
};

bool
gui::player::Playlist::is_empty() const
{
  return play_queue.empty();
};

void
gui::player::Playlist::OnKeyDown(wxKeyEvent& event)
{
  if (event.GetKeyCode() == WXK_SPACE) {
    wxArrayInt selections;
    GetSelections(selections);
    for (int sel : selections) {
      wxString media_item = play_queue[sel];

      wxMediaCtrl* media_ctrl = GetMediaCtrl();
      media_ctrl->Load(media_item);
      media_ctrl->Play();

      play_next_item_in_queue();
    }
  }
  event.Skip();
};

void
gui::player::Playlist::OnDoubleClick(wxCommandEvent& event)
{
  int index = event.GetSelection();
  if (index >= 0) {

    wxString media_item = play_queue[index];
    wxMediaCtrl* media_ctrl = GetMediaCtrl();
    if (media_ctrl->Load(media_item)) {
      media_ctrl->Play();
    } else {
      wxLogError("Failed to load media file: " + media_item);
    }
    play_next_item_in_queue();
  }
  event.Skip();
};

void
gui::player::Playlist::play_next_item_in_queue()
{
  if (!play_queue.empty()) {
    wxString next_item = play_queue.front();

    wxMediaCtrl* media_ctrl = GetMediaCtrl();
    media_ctrl->Load(next_item);
    media_ctrl->Play();
  } else {
    wxLogStatus("No media to play");
  }
};
