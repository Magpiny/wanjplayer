#include "sidebar.hpp"
#include "menubar.hpp"
#include "utils.hpp"
#include <wx/splitter.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/config.h>

namespace gui {

// Sidebar implementation
Sidebar::Sidebar(wxFrame* parent)
    : parent_frame(parent)
    , main_splitter(nullptr)
    , playlist_pane(nullptr)
    , video_canvas_pane(nullptr)
    , toggle_playlist_btn(nullptr)
    , media_book(nullptr)
    , playlist(nullptr)
    , media_controls(nullptr)
    , status_bar(nullptr)
    , media_ctrl(nullptr)
    , player_canvas(nullptr)
    , default_playlist_width(DEFAULT_PLAYLIST_WIDTH)
    , playlist_visible(true)
    , minimum_pane_size(MINIMUM_PANE_SIZE)
    , sash_gravity(DEFAULT_SASH_GRAVITY)
{
    InitializeTheme();
    LoadLayoutSettings();
}

Sidebar::~Sidebar()
{
    SaveLayoutSettings();
    
    // Components are managed by wxWidgets parent-child relationship
    // No manual deletion needed for GUI components
}

void Sidebar::InitializeTheme()
{
    LoadSystemTheme();
    UpdateFonts();
}

void Sidebar::LoadSystemTheme()
{
    background_color = GetDefaultBackgroundColor();
    text_color = GetDefaultTextColor();
    accent_color = GetDefaultAccentColor();
}

wxColour Sidebar::GetDefaultBackgroundColor()
{
    return utils::GuiUtils::GetSystemColor(wxSYS_COLOUR_WINDOW);
}

wxColour Sidebar::GetDefaultTextColor()
{
    return utils::GuiUtils::GetSystemColor(wxSYS_COLOUR_WINDOWTEXT);
}

wxColour Sidebar::GetDefaultAccentColor()
{
    return utils::GuiUtils::GetSystemColor(wxSYS_COLOUR_HIGHLIGHT);
}

void Sidebar::UpdateFonts()
{
    default_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    title_font = utils::GuiUtils::GetScaledFont(default_font, 1.1);
    title_font.SetWeight(wxFONTWEIGHT_BOLD);
}

void Sidebar::CreateMainLayout()
{
    utils::LogUtils::LogInfo("Creating main sidebar layout");
    
    // Create the main splitter window
    utils::LogUtils::LogInfo("Creating splitter window");
    SetupSplitterWindow();
    utils::LogUtils::LogInfo("Splitter window created");
    
    // Create the playlist pane (left side)
    utils::LogUtils::LogInfo("Creating playlist pane");
    CreatePlaylistPane();
    utils::LogUtils::LogInfo("Playlist pane created");
    
    // Create the video pane (right side)
    utils::LogUtils::LogInfo("Creating video pane");
    CreateVideoPane();
    utils::LogUtils::LogInfo("Video pane created");
    
    // Setup the splitter with both panes
    utils::LogUtils::LogInfo("Splitting panes");
    main_splitter->SplitVertically(playlist_pane, video_canvas_pane, default_playlist_width);
    main_splitter->SetMinimumPaneSize(minimum_pane_size);
    main_splitter->SetSashGravity(sash_gravity);
    utils::LogUtils::LogInfo("Panes split");
    
    // Apply modern styling
    utils::LogUtils::LogInfo("Applying modern styling");
    ApplyModernStyling();
    utils::LogUtils::LogInfo("Modern styling applied");
    
    // Setup event bindings
    utils::LogUtils::LogInfo("Setting up event bindings");
    SetupEventBindings();
    utils::LogUtils::LogInfo("Event bindings set up");
    
    // Layout the splitter on the parent frame
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(main_splitter, 1, wxEXPAND);
    parent_frame->SetSizer(main_sizer);
    
    utils::LogUtils::LogInfo("Main sidebar layout created successfully");
}

void Sidebar::SetupSplitterWindow()
{
    main_splitter = new wxSplitterWindow(parent_frame, wxID_ANY);
    if (!main_splitter) {
        utils::LogUtils::LogError("Failed to create splitter window");
        return;
    }
    
    // Configure splitter properties
    main_splitter->SetSashGravity(sash_gravity);
    main_splitter->SetMinimumPaneSize(minimum_pane_size);
    
    ApplyThemeToWindow(main_splitter);
}

void Sidebar::CreatePlaylistPane()
{
    playlist_pane = new wxPanel(main_splitter, wxID_ANY);
    if (!playlist_pane) {
        utils::LogUtils::LogError("Failed to create playlist pane");
        return;
    }
    
    // Create the playlist component
    playlist = new gui::player::Playlist(playlist_pane, wxID_ANY);
    
    // Create toggle button
    CreateToggleButton();
    
    // Setup the sizer for playlist pane
    SetupPlaylistSizer();
    
    // Apply styling
    ApplyPanelStyling(playlist_pane);
    
    utils::LogUtils::LogInfo("Playlist pane created");
}

void Sidebar::CreateToggleButton()
{
    toggle_playlist_btn = new wxButton(playlist_pane, wxID_ANY, "Hide Playlist", 
                                      wxDefaultPosition, wxSize(-1, TOGGLE_BUTTON_HEIGHT));
    
    if (toggle_playlist_btn) {
        ApplyButtonStyling(toggle_playlist_btn);
    }
}

void Sidebar::SetupPlaylistSizer()
{
    wxBoxSizer* playlist_sizer = new wxBoxSizer(wxVERTICAL);
    
    if (toggle_playlist_btn) {
        playlist_sizer->Add(toggle_playlist_btn, 0, wxEXPAND | wxALL, PANEL_BORDER);
    }
    
    if (playlist) {
        playlist_sizer->Add(playlist, 1, wxEXPAND | wxALL, PANEL_BORDER);
    }
    
    playlist_pane->SetSizer(playlist_sizer);
}

void Sidebar::CreateVideoPane()
{
    video_canvas_pane = new wxPanel(main_splitter, ID_MEDIA_CANVAS);
    if (!video_canvas_pane) {
        utils::LogUtils::LogError("Failed to create video canvas pane");
        return;
    }

    media_book = new wxSimplebook(video_canvas_pane, wxID_ANY);

    // Create PlayerCanvas for audio/video visualization
    player_canvas = new gui::PlayerCanvas(media_book, wxID_ANY);
    if (!player_canvas) {
        utils::LogUtils::LogError("Failed to create player canvas");
        return;
    }
    
    // Create media control
    media_ctrl = new wxMediaCtrl(media_book,
                                ID_MEDIA_CTRL,
                                wxEmptyString,
                                wxDefaultPosition,
                                wxDefaultSize,
                                wxMC_NO_AUTORESIZE);
    
    if (!media_ctrl) {
        utils::LogUtils::LogError("Failed to create media control");
        return;
    }

    media_book->AddPage(media_ctrl, "Video");
    media_book->AddPage(player_canvas, "Audio");
    
    // Apply earth brown and sky blue theme colors
    player_canvas->SetBackgroundColor(wxColour(101, 67, 33));  // Dark earth brown
    player_canvas->SetAccentColor(wxColour(135, 206, 250));    // Light sky blue
    player_canvas->SetTextColor(wxColour(245, 245, 220));      // Beige text for readability
    
    // Create media controls
    CreateMediaControls();
    
    // Setup the sizer for video pane
    SetupVideoSizer();
    
    // Apply styling
    ApplyPanelStyling(video_canvas_pane);
    
    utils::LogUtils::LogInfo("Video pane created");
}

void Sidebar::CreateMediaControls()
{
    media_controls = new gui::player::MediaControls(video_canvas_pane, media_ctrl);
    
    if (!media_controls) {
        utils::LogUtils::LogError("Failed to create media controls");
        return;
    }
    
    ApplyThemeToWindow(media_controls);
}

void Sidebar::SetupVideoSizer()
{
    wxBoxSizer* video_sizer = new wxBoxSizer(wxVERTICAL);
    
    if (media_book) {
        video_sizer->Add(media_book, 1, wxEXPAND);
    }
    
    if (media_controls) {
        video_sizer->Add(media_controls, 0, wxEXPAND);
    }
    
    video_canvas_pane->SetSizer(video_sizer);
}

void Sidebar::ApplyModernStyling()
{
    // Apply modern styling to all components
    if (main_splitter) {
        main_splitter->SetBackgroundColour(background_color);
    }
    
    if (playlist_pane) {
        ApplyPanelStyling(playlist_pane);
    }
    
    if (video_canvas_pane) {
        ApplyPanelStyling(video_canvas_pane);
    }
    
    if (toggle_playlist_btn) {
        ApplyButtonStyling(toggle_playlist_btn);
    }
    
    if (playlist) {
        ApplyThemeToWindow(playlist);
    }
}

void Sidebar::ApplyThemeToWindow(wxWindow* window)
{
    if (!window) return;
    
    window->SetBackgroundColour(background_color);
    window->SetForegroundColour(text_color);
    window->SetFont(default_font);
    window->Refresh();
}

void Sidebar::ApplyPanelStyling(wxPanel* panel)
{
    if (!panel) return;
    
    panel->SetBackgroundColour(background_color);
    panel->SetForegroundColour(text_color);
    panel->SetFont(default_font);
    
    // Add subtle border effect
    panel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void Sidebar::ApplyButtonStyling(wxButton* button)
{
    if (!button) return;
    
    button->SetBackgroundColour(utils::GuiUtils::BlendColors(background_color, accent_color, 0.1));
    button->SetForegroundColour(text_color);
    button->SetFont(default_font);
}

void Sidebar::SetCustomColors(const wxColour& bg, const wxColour& text, const wxColour& accent)
{
    background_color = bg;
    text_color = text;
    accent_color = accent;
    
    ApplyModernStyling();
}

void Sidebar::SetupEventBindings()
{
    if (!parent_frame) return;
    
    // Bind toggle playlist button
    if (toggle_playlist_btn) {
        toggle_playlist_btn->Bind(wxEVT_BUTTON, &Sidebar::OnTogglePlaylist, this);
    }
    
    // Bind splitter events
    if (main_splitter) {
        parent_frame->Bind(wxEVT_SPLITTER_SASH_POS_CHANGED, 
                          &Sidebar::OnSplitterSashPosChanged, this,
                          main_splitter->GetId());
    }
    
    // Connect to parent's menu toggle
    if (parent_frame) {
        parent_frame->Bind(wxEVT_MENU, 
            [this](wxCommandEvent& event) {
                this->TogglePlaylistVisibility();
                this->UpdateLayout();
                event.Skip();
            }, ID_TOGGLE_PLAYLIST);
    }
}

void Sidebar::ConnectComponents()
{
    // Connect playlist and media controls
    if (media_controls && playlist) {
        media_controls->SetPlaylist(playlist);
        playlist->SetMediaCtrl(media_ctrl);
    }
    
    // Connect status bar to media controls
    if (media_controls && status_bar) {
        media_controls->SetStatusBar(status_bar);
    }
    
    utils::LogUtils::LogInfo("Components connected successfully");
}

void Sidebar::TogglePlaylistVisibility()
{
    if (playlist_visible) {
        HidePlaylist();
    } else {
        ShowPlaylist();
    }
}

void Sidebar::ShowPlaylist()
{
    if (!main_splitter || !playlist_pane || !video_canvas_pane) return;
    
    if (!playlist_visible) {
        main_splitter->SplitVertically(playlist_pane, video_canvas_pane, default_playlist_width);
        playlist_visible = true;
        
        if (toggle_playlist_btn) {
            toggle_playlist_btn->SetLabel("Hide Playlist");
        }
        
        // Force layout update
        main_splitter->UpdateSize();
        if (parent_frame) {
            parent_frame->Layout();
        }
        
        utils::LogUtils::LogInfo("Playlist shown");
    }
}

void Sidebar::HidePlaylist()
{
    if (!main_splitter || !video_canvas_pane) return;
    
    if (playlist_visible) {
        // Save current playlist width before hiding
        default_playlist_width = main_splitter->GetSashPosition();
        
        main_splitter->Unsplit(playlist_pane);
        playlist_visible = false;
        
        if (toggle_playlist_btn) {
            toggle_playlist_btn->SetLabel("Show Playlist");
        }
        
        // Force layout update
        main_splitter->UpdateSize();
        if (parent_frame) {
            parent_frame->Layout();
        }
        
        utils::LogUtils::LogInfo("Playlist hidden");
    }
}

void Sidebar::ResizePlaylistPane(int width)
{
    if (!main_splitter || !playlist_visible) return;
    
    if (width >= minimum_pane_size) {
        main_splitter->SetSashPosition(width);
        default_playlist_width = width;
    }
}

void Sidebar::UpdateLayout()
{
    if (main_splitter) {
        main_splitter->UpdateSize();
    }
    
    if (parent_frame) {
        parent_frame->Layout();
    }
}

void Sidebar::OnTogglePlaylist(wxCommandEvent& event)
{
    wxLogMessage("Sidebar: Toggle playlist button clicked");
    TogglePlaylistVisibility();
    UpdateLayout();
    
    // Update button label based on current state
    if (toggle_playlist_btn) {
        if (playlist_visible) {
            toggle_playlist_btn->SetLabel("Hide Playlist");
        } else {
            toggle_playlist_btn->SetLabel("Show Playlist");
        }
    }
    
    event.Skip();
}

void Sidebar::OnSplitterSashPosChanged(wxSplitterEvent& event)
{
    if (playlist_visible) {
        default_playlist_width = event.GetSashPosition();
    }
    event.Skip();
}

void Sidebar::OnPlaylistPaneSize(wxSizeEvent& event)
{
    UpdateLayout();
    event.Skip();
}

void Sidebar::AnimatePlaylistToggle(bool show)
{
    // Simple animation implementation
    // Could be enhanced with more sophisticated transitions
    if (show) {
        ShowPlaylist();
    } else {
        HidePlaylist();
    }
    UpdateLayout();
}

void Sidebar::SetTransitionEffect(bool enable)
{
    // Placeholder for transition effects
    // Could be implemented with timer-based animations
    utils::LogUtils::LogInfo(wxString::Format("Transition effects %s", 
                                             enable ? "enabled" : "disabled"));
}

void Sidebar::SetupAccessibility()
{
    // Setup accessibility labels and descriptions
    if (toggle_playlist_btn) {
        toggle_playlist_btn->SetToolTip("Toggle playlist visibility");
    }
    
    if (playlist) {
        playlist->SetToolTip("Media playlist - Double-click to play, Delete to remove");
    }
    
    if (main_splitter) {
        main_splitter->SetToolTip("Drag to resize playlist panel");
    }
}

void Sidebar::UpdateAccessibilityLabels()
{
    if (toggle_playlist_btn) {
        wxString tooltip = playlist_visible ? "Hide playlist panel" : "Show playlist panel";
        toggle_playlist_btn->SetToolTip(tooltip);
    }
}

void Sidebar::SaveLayoutSettings()
{
    wxConfig config("WanjPlayer");
    
    config.Write("Layout/PlaylistWidth", default_playlist_width);
    config.Write("Layout/PlaylistVisible", playlist_visible);
    config.Write("Layout/MinimumPaneSize", minimum_pane_size);
    
    utils::LogUtils::LogDebug("Layout settings saved");
}

void Sidebar::LoadLayoutSettings()
{
    wxConfig config("WanjPlayer");
    
    config.Read("Layout/PlaylistWidth", &default_playlist_width, DEFAULT_PLAYLIST_WIDTH);
    config.Read("Layout/PlaylistVisible", &playlist_visible, true);
    config.Read("Layout/MinimumPaneSize", &minimum_pane_size, MINIMUM_PANE_SIZE);
    
    utils::LogUtils::LogDebug("Layout settings loaded");
}

// SidebarFactory implementation
Sidebar* SidebarFactory::CreateSidebar(wxFrame* parent, ThemeType theme)
{
    Sidebar* sidebar = new Sidebar(parent);
    
    if (sidebar) {
        SidebarTheme theme_config = GetTheme(theme);
        ApplyTheme(sidebar, theme_config);
        
        utils::LogUtils::LogInfo("Sidebar created with theme");
    }
    
    return sidebar;
}

SidebarTheme SidebarFactory::GetTheme(ThemeType theme)
{
    SidebarTheme config;
    
    switch (theme) {
        case ThemeType::DARK:
            config.background_color = wxColour(45, 45, 48);
            config.text_color = wxColour(255, 255, 255);
            config.accent_color = wxColour(0, 120, 215);
            config.button_color = wxColour(62, 62, 66);
            config.button_hover_color = wxColour(82, 82, 86);
            break;
            
        case ThemeType::LIGHT:
            config.background_color = wxColour(255, 255, 255);
            config.text_color = wxColour(0, 0, 0);
            config.accent_color = wxColour(0, 120, 215);
            config.button_color = wxColour(225, 225, 225);
            config.button_hover_color = wxColour(200, 200, 200);
            break;
            
        case ThemeType::HIGH_CONTRAST:
            config.background_color = wxColour(0, 0, 0);
            config.text_color = wxColour(255, 255, 255);
            config.accent_color = wxColour(255, 255, 0);
            config.button_color = wxColour(128, 128, 128);
            config.button_hover_color = wxColour(192, 192, 192);
            break;
            
        case ThemeType::MODERN:
            config.background_color = wxColour(32, 32, 32);
            config.text_color = wxColour(220, 220, 220);
            config.accent_color = wxColour(0, 150, 136);
            config.button_color = wxColour(55, 55, 55);
            config.button_hover_color = wxColour(75, 75, 75);
            break;
            
        default: // DEFAULT
            config.background_color = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
            config.text_color = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
            config.accent_color = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
            config.button_color = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
            config.button_hover_color = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT);
            break;
    }
    
    config.default_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    config.title_font = utils::GuiUtils::GetScaledFont(config.default_font, 1.1);
    config.border_width = 1;
    config.use_gradients = (theme == ThemeType::MODERN);
    config.use_rounded_corners = (theme == ThemeType::MODERN);
    
    return config;
}

void SidebarFactory::ApplyTheme(Sidebar* sidebar, const SidebarTheme& theme)
{
    if (!sidebar) return;
    
    sidebar->SetCustomColors(theme.background_color, theme.text_color, theme.accent_color);
    
    utils::LogUtils::LogInfo("Theme applied to sidebar");
}

}