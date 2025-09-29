#include "gui_utils.hpp"
#include <wx/display.h>
#include <wx/config.h>
#include <wx/image.h>
#include <wx/dcmemory.h>
#include <algorithm>
#include <cmath>

namespace utils {

// Color utilities
wxColour GuiUtils::GetSystemColor(wxSystemColour color)
{
    return wxSystemSettings::GetColour(color);
}

wxColour GuiUtils::BlendColors(const wxColour& color1, const wxColour& color2, double ratio)
{
    if (ratio <= 0.0) return color1;
    if (ratio >= 1.0) return color2;
    
    int r = static_cast<int>(color1.Red() * (1.0 - ratio) + color2.Red() * ratio);
    int g = static_cast<int>(color1.Green() * (1.0 - ratio) + color2.Green() * ratio);
    int b = static_cast<int>(color1.Blue() * (1.0 - ratio) + color2.Blue() * ratio);
    
    return ClampColor(r, g, b);
}

wxColour GuiUtils::DarkenColor(const wxColour& color, double factor)
{
    int r = static_cast<int>(color.Red() * factor);
    int g = static_cast<int>(color.Green() * factor);
    int b = static_cast<int>(color.Blue() * factor);
    
    return ClampColor(r, g, b);
}

wxColour GuiUtils::LightenColor(const wxColour& color, double factor)
{
    int r = static_cast<int>(color.Red() + (255 - color.Red()) * (factor - 1.0));
    int g = static_cast<int>(color.Green() + (255 - color.Green()) * (factor - 1.0));
    int b = static_cast<int>(color.Blue() + (255 - color.Blue()) * (factor - 1.0));
    
    return ClampColor(r, g, b);
}

wxColour GuiUtils::GetContrastColor(const wxColour& background)
{
    double luminance = CalculateLuminance(background);
    return luminance > 0.5 ? *wxBLACK : *wxWHITE;
}

// Layout helpers
void GuiUtils::SetMinimumSize(wxWindow* window, int width, int height)
{
    if (window) {
        window->SetMinSize(wxSize(width, height));
    }
}

void GuiUtils::CenterWindow(wxWindow* window, wxWindow* parent)
{
    if (window) {
        if (parent) {
            window->CenterOnParent();
        } else {
            window->Center();
        }
    }
}

void GuiUtils::CenterWindowOnScreen(wxWindow* window)
{
    if (window) {
        window->Center();
    }
}

wxSize GuiUtils::GetOptimalSize(wxWindow* window)
{
    if (!window) {
        return wxSize(400, 300);
    }
    
    wxSize best = window->GetBestSize();
    wxSize min = window->GetMinSize();
    
    return wxSize(std::max(best.GetWidth(), min.GetWidth()),
                  std::max(best.GetHeight(), min.GetHeight()));
}

void GuiUtils::SetWindowAspectRatio(wxWindow* window, double ratio)
{
    if (!window || ratio <= 0.0) {
        return;
    }
    
    wxSize current = window->GetSize();
    int width = current.GetWidth();
    int height = static_cast<int>(width / ratio);
    
    window->SetSize(width, height);
}

// Font utilities
wxFont GuiUtils::GetScaledFont(const wxFont& base_font, double scale)
{
    wxFont scaled_font = base_font;
    int new_size = static_cast<int>(base_font.GetPointSize() * scale);
    scaled_font.SetPointSize(std::max(6, new_size));
    return scaled_font;
}

wxFont GuiUtils::GetMonospaceFont(int point_size)
{
    return wxFont(point_size == -1 ? wxNORMAL_FONT->GetPointSize() : point_size,
                  wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
}

wxFont GuiUtils::GetBoldFont(const wxFont& base_font)
{
    wxFont bold_font = base_font;
    bold_font.SetWeight(wxFONTWEIGHT_BOLD);
    return bold_font;
}

wxFont GuiUtils::GetItalicFont(const wxFont& base_font)
{
    wxFont italic_font = base_font;
    italic_font.SetStyle(wxFONTSTYLE_ITALIC);
    return italic_font;
}

int GuiUtils::GetTextWidth(const wxString& text, const wxFont& font)
{
    wxMemoryDC dc;
    dc.SetFont(font);
    wxSize size = dc.GetTextExtent(text);
    return size.GetWidth();
}

int GuiUtils::GetTextHeight(const wxFont& font)
{
    wxMemoryDC dc;
    dc.SetFont(font);
    wxSize size = dc.GetTextExtent("Ay");
    return size.GetHeight();
}

// Icon and bitmap utilities
wxBitmap GuiUtils::LoadScaledBitmap(const wxString& path, int width, int height)
{
    wxImage image;
    if (!image.LoadFile(path)) {
        return wxNullBitmap;
    }
    
    if (width > 0 && height > 0) {
        image = image.Scale(width, height, wxIMAGE_QUALITY_HIGH);
    }
    
    return wxBitmap(image);
}

wxBitmap GuiUtils::CreateColorBitmap(const wxColour& color, int width, int height)
{
    wxBitmap bitmap(width, height);
    wxMemoryDC dc(bitmap);
    dc.SetBackground(wxBrush(color));
    dc.Clear();
    return bitmap;
}

wxBitmap GuiUtils::ResizeBitmap(const wxBitmap& bitmap, int width, int height)
{
    if (!bitmap.IsOk()) {
        return wxNullBitmap;
    }
    
    wxImage image = bitmap.ConvertToImage();
    image = image.Scale(width, height, wxIMAGE_QUALITY_HIGH);
    return wxBitmap(image);
}

wxIcon GuiUtils::LoadIcon(const wxString& path, int size)
{
    wxIcon icon;
    if (icon.LoadFile(path, wxBITMAP_TYPE_ANY, size, size)) {
        return icon;
    }
    return wxNullIcon;
}

// Theme utilities
bool GuiUtils::IsDarkTheme()
{
    wxColour bg = GetSystemColor(wxSYS_COLOUR_WINDOW);
    return CalculateLuminance(bg) < 0.5;
}

wxColour GuiUtils::GetDefaultBackgroundColor()
{
    return GetSystemColor(wxSYS_COLOUR_WINDOW);
}

wxColour GuiUtils::GetDefaultForegroundColor()
{
    return GetSystemColor(wxSYS_COLOUR_WINDOWTEXT);
}

wxColour GuiUtils::GetDefaultAccentColor()
{
    return GetSystemColor(wxSYS_COLOUR_HIGHLIGHT);
}

// Animation helpers
double GuiUtils::EaseInOut(double t)
{
    t = std::max(0.0, std::min(1.0, t));
    return t * t * (3.0 - 2.0 * t);
}

double GuiUtils::EaseIn(double t)
{
    t = std::max(0.0, std::min(1.0, t));
    return t * t;
}

double GuiUtils::EaseOut(double t)
{
    t = std::max(0.0, std::min(1.0, t));
    return 1.0 - (1.0 - t) * (1.0 - t);
}

wxColour GuiUtils::InterpolateColors(const wxColour& start, const wxColour& end, double progress)
{
    progress = std::max(0.0, std::min(1.0, progress));
    return BlendColors(start, end, progress);
}

// Screen utilities
wxRect GuiUtils::GetScreenRect()
{
    wxDisplay display(wxDisplay::GetFromPoint(wxGetMousePosition()));
    return display.GetGeometry();
}

wxRect GuiUtils::GetWorkAreaRect()
{
    wxDisplay display(wxDisplay::GetFromPoint(wxGetMousePosition()));
    return display.GetClientArea();
}

double GuiUtils::GetDisplayScaleFactor()
{
    return wxDisplay().GetScaleFactor();
}

bool GuiUtils::IsHighDPIDisplay()
{
    return GetDisplayScaleFactor() > 1.0;
}

// Window state utilities
void GuiUtils::SaveWindowGeometry(wxWindow* window, const wxString& key)
{
    if (!window) return;
    
    wxConfig config;
    wxString group = "WindowGeometry/" + key;
    
    config.SetPath("/" + group);
    
    wxPoint pos = window->GetPosition();
    wxSize size = window->GetSize();
    
    config.Write("x", pos.x);
    config.Write("y", pos.y);
    config.Write("width", size.GetWidth());
    config.Write("height", size.GetHeight());
    
    wxFrame* frame = dynamic_cast<wxFrame*>(window);
    if (frame && frame->IsMaximized()) {
        config.Write("maximized", true);
    }
}

void GuiUtils::RestoreWindowGeometry(wxWindow* window, const wxString& key)
{
    if (!window) return;
    
    wxConfig config;
    wxString group = "WindowGeometry/" + key;
    
    config.SetPath("/" + group);
    
    long x, y, width, height;
    bool maximized = false;
    
    if (config.Read("x", &x) && config.Read("y", &y) &&
        config.Read("width", &width) && config.Read("height", &height)) {
        
        wxPoint pos(x, y);
        wxSize size(width, height);
        
        // Ensure window is visible on screen
        if (IsWindowVisibleOnScreen(window)) {
            window->SetSize(pos.x, pos.y, size.GetWidth(), size.GetHeight());
        } else {
            window->SetSize(size.GetWidth(), size.GetHeight());
            CenterWindowOnScreen(window);
        }
        
        config.Read("maximized", &maximized, false);
        if (maximized) {
            wxFrame* frame = dynamic_cast<wxFrame*>(window);
            if (frame) {
                frame->Maximize();
            }
        }
    }
}

bool GuiUtils::IsWindowVisibleOnScreen(wxWindow* window)
{
    if (!window) return false;
    
    wxRect window_rect = window->GetRect();
    wxRect screen_rect = GetScreenRect();
    
    return screen_rect.Intersects(window_rect);
}

// Utility methods
void GuiUtils::FlashWindow(wxWindow* window)
{
    if (window) {
        wxFrame* frame = dynamic_cast<wxFrame*>(window);
        if (frame) {
            frame->RequestUserAttention();
        }
    }
}

void GuiUtils::SetWindowTransparency(wxWindow* window, int alpha)
{
    if (window && alpha >= 0 && alpha <= 255) {
        window->SetTransparent(alpha);
    }
}

wxString GuiUtils::GetWindowTitle(wxWindow* window)
{
    return window ? window->GetLabel() : wxEmptyString;
}

// Private helper methods
wxColour GuiUtils::ClampColor(int r, int g, int b)
{
    r = std::max(0, std::min(255, r));
    g = std::max(0, std::min(255, g));
    b = std::max(0, std::min(255, b));
    return wxColour(r, g, b);
}

double GuiUtils::CalculateLuminance(const wxColour& color)
{
    // Calculate relative luminance using sRGB formula
    double r = color.Red() / 255.0;
    double g = color.Green() / 255.0;
    double b = color.Blue() / 255.0;
    
    // Apply gamma correction
    r = r <= 0.03928 ? r / 12.92 : std::pow((r + 0.055) / 1.055, 2.4);
    g = g <= 0.03928 ? g / 12.92 : std::pow((g + 0.055) / 1.055, 2.4);
    b = b <= 0.03928 ? b / 12.92 : std::pow((b + 0.055) / 1.055, 2.4);
    
    return 0.2126 * r + 0.7152 * g + 0.0722 * b;
}

}