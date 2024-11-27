#ifndef __PLAYLIST__HPP
#define __PLAYLIST__HPP

#include "wanjplayer.hpp"

namespace gui::player {
class Playlist : public wxListBox
{
public:
  Playlist(wxWindow* parent, wxWindowID id);

  void add_item(const wxString& path);
  void clear_play_queue();
  wxString get_item(size_t index) const;
  size_t get_count() const;
  bool is_empty() const;
  void play_next_item_in_queue();
  wxMediaCtrl* GetMediaCtrl();

private:
  std::vector<wxString> play_queue;

  void OnKeyDown(wxKeyEvent& event);
  void OnDoubleClick(wxCommandEvent& event);

  void play_item(const wxString& path);
};
}

#endif // __PLAYLIST_HPP
