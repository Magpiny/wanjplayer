#include "sidebar.hpp"

SideBar::SideBar(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 650)) {
  SetBackgroundColour(wxColour(0, 0, 0, 50));
  Bind(wxEVT_BUTTON, &SideBar::OnAddTrack, this, btn_add_track->GetId());
  Bind(wxEVT_BUTTON, &SideBar::OnRemoveTrack, this, btn_rmv_track->GetId());
};

void SideBar::create_sidebar() {
  list_ctrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition,
                             wxSize(188, 600), wxLC_REPORT | wxLC_SINGLE_SEL);
  list_ctrl->InsertColumn(0, "Track Name");
  btn_add_track = new wxButton(this, wxID_ANY, "Add Track", wxDefaultPosition,
                               wxSize(80, 25));
  btn_rmv_track =
      new wxButton(this, wxID_ANY, "Remove", wxDefaultPosition, wxSize(80, 25));

  // Layout
  wxBoxSizer* list_ctrl_sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* btns_sizer = new wxBoxSizer(wxHORIZONTAL);

  list_ctrl_sizer->Add(list_ctrl, 1, wxEXPAND | wxALL, 5);
  btns_sizer->Add(btn_add_track, 0, wxALL, 5);
  btns_sizer->Add(btn_rmv_track, 0, wxALL, 5);
  list_ctrl_sizer->Add(btns_sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  SetSizerAndFit(list_ctrl_sizer);
};

void SideBar::OnAddTrack(wxCommandEvent& event) {
  wxLogStatus("Add Button Clicked!");
  event.Skip();
};
void SideBar::OnRemoveTrack(wxCommandEvent& event) {
  wxLogStatus("Remove Track Button Clicked!");
  event.Skip();
};
