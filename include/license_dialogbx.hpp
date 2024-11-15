#ifndef __LICENCE_DIALOG_BOX__HPP
#define __LICENCE_DIALOG_BOX__HPP
#include "wx_widgets.hpp"

namespace gui {
class LicenseDialog : public wxDialog
{
public:
  LicenseDialog(wxWindow* parent, wxWindowID id, const wxString& title);
  void load_licence(const wxString& file_path);

private:
  wxStaticText* licence_text_ctrl;
};
}

#endif // !__LICENCE_DIALOG_BOX__HPP
