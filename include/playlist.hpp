#ifndef PLAYLIST__HPP
#define PLAYLIST__HPP

#include "wanjplayer.hpp"
#include "widgets.hpp"

namespace gui {
class Playlist : public wxListBox
{
public:
  Playlist(wxWindow* parent, wxWindowID id);

  void add_item(const wxString& path);
  void clear_items();
  wxString get_item(size_t index) const;
  size_t get_count() const;
  bool is_empty() const;

private:
  std::vector<wxString> items;

  void OnItemSelected(wxCommandEvent& event);
  void OnKeyDown(wxKeyEvent& event);
  void OnDoubleClick(wxMouseEvent& event);

  void play_item(const wxString& path);
};
}

#endif // PLAYLIST_HPP
