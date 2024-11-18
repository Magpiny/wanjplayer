#ifndef __LICENCE_DIALOG_BOX__HPP
#define __LICENCE_DIALOG_BOX__HPP
#include "widgets.hpp"
#include <cstddef>

namespace gui {
class LicenseDialog : public wxDialog
{
public:
  LicenseDialog(wxWindow* parent, const wxString& title);
  void load_license(const wxString& file_path);
  ~LicenseDialog();

private:
  wxStaticText* license_text_ctrl;
};
}

#endif // !__LICENCE_DIALOG_BOX__HPP
