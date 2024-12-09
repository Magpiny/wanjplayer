#include "license_dlgbx.hpp"
//
LicenseDialog::LicenseDialog(wxWindow* parent, const wxString& title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(650, 450))
{
    license_text_ctrl = new wxStaticText(this, wxID_ANY, wxEmptyString);
    wxBoxSizer* text_sizer = new wxBoxSizer(wxVERTICAL);
    text_sizer->Add(license_text_ctrl, 1, wxEXPAND | wxALL, 10);
    SetSizer(text_sizer);

    CenterOnParent();
};

void LicenseDialog::load_license(const wxString& file_path)
{
    wxTextFile file(file_path);
    if (file.Open()) {
        wxString content;
        for (size_t i = 0; i < file.GetLineCount(); i++) {
            content += file.GetLine(i) + "\n";
        }
        license_text_ctrl->SetLabel(content);
        license_text_ctrl->Wrap(600);
        file.Close();
    } else {
        wxMessageBox("Failed to load LICENSE", "ERROR!", wxOK | wxICON_ERROR, this);
    }
};

LicenseDialog::~LicenseDialog() { };
