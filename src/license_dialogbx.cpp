
//
gui::LicenseDialog::LicenseDialog(wxWindow* parent,
                                  wxWindowID id,
                                  const wxString& title)
  : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(850, 750))
{
  license_text_ctrl = new wxStaticText(this, wxID_ANY, wxEmptyString);
  wxBoxSizer* text_sizer = new wxBoxSizer(wxVEERTICAL);
  text_sizer->Add(license_text_ctrl, 1, wxEXPAND | wxALL, 10);
  SetSizer(text_sizer);

  CenterOnParent();
}

void
gui::LicenseDialog::load_license(const wxString& file_path)
{
  wxTextFile file(file_path);
  if (file.Open()) {
    wxString content;
    for (size_t i = 0; i < file.GetLineCount; i++) {
      content += file.GetLine(i) + "\n";
    }
    license_text_ctrl->SetValue(content);
    file.close();
  } else {
    wxMessageBox("Failed to load LICENSE", "ERROR!", wxOK | wxICON_ERROR, this);
  }
}
