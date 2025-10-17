#ifndef __PLAYER_UI_CONTROL__HPP
#define __PLAYER_UI_CONTROL__HPP

#include <wx/wx.h>
#include <wx/simplebook.h>
#include <wx/mediactrl.h>
#include "canvas.hpp"

namespace gui {

class PlayerUIControl : public wxPanel
{
public:
    PlayerUIControl(wxWindow* parent, wxWindowID id = wxID_ANY);

    void ShowVideoCanvas();
    void ShowAudioCanvas();
    wxMediaCtrl* GetMediaCtrl() const { return media_ctrl; }

private:
    wxSimplebook* book;
    wxMediaCtrl* media_ctrl;
    PlayerCanvas* audio_canvas;
};

}

#endif // __PLAYER_UI_CONTROL__HPP