#ifndef __MAIN_LAYOUT__HPP
#define __MAIN_LAYOUT__HPP

#include "wanjplayer.hpp"
#include "playlist.hpp"
#include "media_ctrls.hpp"
#include "statusbar.hpp"
#include "canvas.hpp"
#include "utils.hpp"
#include "player_ui_control.hpp"

#include <wx/simplebook.h>

namespace gui {

class PlayerUIControl; // Forward declaration

class MainLayout
{
private:
    wxFrame* parent_frame;
    wxSplitterWindow* main_splitter;
    wxPanel* playlist_pane;
    wxPanel* video_canvas_pane;
    wxButton* toggle_playlist_btn;
    
    // GUI styling properties
    wxColour background_color;
    wxColour text_color;
    wxColour accent_color;
    wxFont default_font;
    wxFont title_font;
    
    // References to other components
    gui::player::Playlist* playlist;
    gui::player::MediaControls* media_controls;
    gui::StatusBar* status_bar;
    PlayerUIControl* player_ui_control;
    
    // Layout properties
    int default_playlist_width;
    bool playlist_visible;
    int minimum_pane_size;
    double sash_gravity;

public:
    MainLayout(wxFrame* parent);
    ~MainLayout();
    
    // Main GUI creation methods
    void CreateMainLayout();
    void CreatePlaylistPane();
    void CreateVideoPane();
    void CreateMediaControls();
    void SetupSplitterWindow();
    
    // Styling methods
    void InitializeTheme();
    void ApplyThemeToWindow(wxWindow* window);
    void SetCustomColors(const wxColour& bg, const wxColour& text, const wxColour& accent);
    void UpdateFonts();
    void ApplyModernStyling();
    
    // Layout management
    void TogglePlaylistVisibility();
    void ShowPlaylist();
    void HidePlaylist();
    void ResizePlaylistPane(int width);
    void UpdateLayout();
    
    // Component getters
    wxSplitterWindow* GetMainSplitter() const { return main_splitter; }
    wxPanel* GetPlaylistPane() const { return playlist_pane; }
    wxPanel* GetVideoPane() const { return video_canvas_pane; }
    gui::player::Playlist* GetPlaylist() const { return playlist; }
    gui::player::MediaControls* GetMediaControls() const { return media_controls; }
    PlayerUIControl* GetPlayerUIControl() const { return player_ui_control; }
    
    // Component setters
    void SetPlaylist(gui::player::Playlist* pl) { playlist = pl; }
    void SetMediaControls(gui::player::MediaControls* mc) { media_controls = mc; }
    void SetStatusBar(gui::StatusBar* sb) { status_bar = sb; }
    
    // Event handlers
    void OnTogglePlaylist(wxCommandEvent& event);
    void OnSplitterSashPosChanged(wxSplitterEvent& event);
    void OnPlaylistPaneSize(wxSizeEvent& event);
    
    // Utility methods
    void ConnectComponents();
    void SetupEventBindings();
    bool IsPlaylistVisible() const { return playlist_visible; }
    void SaveLayoutSettings();
    void LoadLayoutSettings();
    
    // Animation and effects
    void AnimatePlaylistToggle(bool show);
    void SetTransitionEffect(bool enable);
    
    // Accessibility
    void SetupAccessibility();
    void UpdateAccessibilityLabels();

private:
    // Helper methods
    void CreateToggleButton();
    void SetupPlaylistSizer();
    void SetupVideoSizer();
    void ApplyButtonStyling(wxButton* button);
    void ApplyPanelStyling(wxPanel* panel);
    
    // Theme helpers
    wxColour GetDefaultBackgroundColor();
    wxColour GetDefaultTextColor();
    wxColour GetDefaultAccentColor();
    void LoadSystemTheme();
    
    // Constants
    static constexpr int DEFAULT_PLAYLIST_WIDTH = 250;
    static constexpr int MINIMUM_PANE_SIZE = 50;
    static constexpr double DEFAULT_SASH_GRAVITY = 0.0;
    static constexpr int TOGGLE_BUTTON_HEIGHT = 30;
    static constexpr int PANEL_BORDER = 2;
};

// MainLayout theme configuration
struct MainLayoutTheme {
    wxColour background_color;
    wxColour text_color;
    wxColour accent_color;
    wxColour button_color;
    wxColour button_hover_color;
    wxFont default_font;
    wxFont title_font;
    int border_width;
    bool use_gradients;
    bool use_rounded_corners;
};

// Factory class for creating themed layouts
class MainLayoutFactory {
public:
    enum class ThemeType {
        DEFAULT,
        DARK,
        LIGHT,
        HIGH_CONTRAST,
        MODERN
    };
    
    static MainLayout* CreateMainLayout(wxFrame* parent, ThemeType theme = ThemeType::DEFAULT);
    static MainLayoutTheme GetTheme(ThemeType theme);
    static void ApplyTheme(MainLayout* main_layout, const MainLayoutTheme& theme);
};

}

#endif // __MAIN_LAYOUT__HPP