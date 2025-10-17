#include "player_ui_control.hpp"

namespace gui {

PlayerUIControl::PlayerUIControl(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)
{
    book = new wxSimplebook(this, wxID_ANY);

    media_ctrl = new wxMediaCtrl(book, wxID_ANY);
    audio_canvas = new PlayerCanvas(book, wxID_ANY);

    book->AddPage(media_ctrl, "Video");
    book->AddPage(audio_canvas, "Audio");

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(book, 1, wxEXPAND);
    SetSizer(sizer);
}

void PlayerUIControl::ShowVideoCanvas()
{
    book->SetSelection(0);
}

void PlayerUIControl::ShowAudioCanvas()
{
    book->SetSelection(1);
}

}
