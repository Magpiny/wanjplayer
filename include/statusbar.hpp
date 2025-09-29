#ifndef __STATUSBAR__HPP
#define __STATUSBAR__HPP

#include <wx/wx.h>
#include <wx/mediactrl.h>
#include "utils.hpp"

namespace gui {
class StatusBar
{
public:
  StatusBar(wxFrame* parent);
  void create_statusbar();
  void update_file_info(const wxString& file_path, wxFileOffset duration_ms = 0);
  void update_playback_info(const wxString& status);
  void set_system_message(const wxString& message);
  void set_duration_display(const wxString& duration_str);
  ~StatusBar();

private:
  wxFrame* _parent;
};
}

#endif // !__STATUSBAR__HPP
