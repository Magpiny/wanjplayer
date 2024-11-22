#include "playlist.hpp"

gui::Playlist::Playlist(wxWindow* parent, wxWindowID id)
  : wxListBox(parent,
              id,
              wxDefaultPosition,
              wxDefaultSize,
              0,
              nullptr,
              wxLB_MULTIPLE)
{
  Bind(wxEVT_LISTBOX, &Playlist::OnItemSelected, this);
  Bind(wxEVT_KEY_DOWN, &Playlist::OnKeyDown, this);
  Bind(wxEVT_LEFT_DCLICK, &Playlist::OnDoubleClick, this);
};

void
gui::Playlist::add_item(const wxString& path)
{
  wxFileName file_name(path);
  items.push_back(path);
  Append(file_name.GetFullName());
};

void
gui::Playlist::clear_items()
{
  items.clear();
  Clear();
};

wxString
gui::Playlist::get_item(size_t index) const
{
  if (index < items.size()) {
    return items[index];
  }
  return "";
};

size_t
gui::Playlist::get_count() const
{
  return items.size();
};

bool
gui::Playlist::is_empty() const
{
  return items.empty();
};

/*void*/
/*gui::Playlist::OnItemSelected(wxCommandEvent& event)*/
/*{*/
/*// Change background color of selected items*/
/*for (size_t i = 0; i < GetCount(); ++i) {*/
/*if (IsSelected(i)) {*/
/*SetItemBackgroundColour(i, *wxBLUE); // Change to desired color*/
/*} else {*/
/*SetItemBackgroundColour(i, *wxWHITE); // Default color*/
/*}*/
/*}*/
/*Refresh();*/
/*};*/

void
gui::Playlist::OnKeyDown(wxKeyEvent& event)
{
  if (event.GetKeyCode() == WXK_SPACE) {
    // Play the selected item
    wxArrayInt selections;
    GetSelections(selections);
    for (int sel : selections) {
      play_item(items[sel]);
    }
  }
  event.Skip();
};

void
gui::Playlist::OnDoubleClick(wxMouseEvent& event)
{
  long index = HitTest(event.GetPosition());
  if (index != wxNOT_FOUND) {
    play_item(items[index]);
  }
  event.Skip();
};

void
gui::Playlist::play_item(const wxString& path)
{
  // Implement the logic to play the selected item
  wxMediaCtrl* media_ctrl = dynamic_cast<wxMediaCtrl*>(
    GetParent()->GetParent()->FindWindow(ID_MEDIA_CANVAS));
  if (media_ctrl) {
    media_ctrl->Load(path);
    media_ctrl->Play();
  }
};
