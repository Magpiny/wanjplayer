#ifndef __GUI_UTILS_HPP
#define __GUI_UTILS_HPP

#include <wx/wx.h>
#include <wx/colour.h>
#include <wx/font.h>
#include <wx/bitmap.h>
#include <wx/settings.h>

namespace utils {

// GUI utilities
class GuiUtils {
public:
    // Color utilities
    static wxColour GetSystemColor(wxSystemColour color);
    static wxColour BlendColors(const wxColour& color1, const wxColour& color2, double ratio);
    static wxColour DarkenColor(const wxColour& color, double factor = 0.8);
    static wxColour LightenColor(const wxColour& color, double factor = 1.2);
    static wxColour GetContrastColor(const wxColour& background);
    
    // Layout helpers
    static void SetMinimumSize(wxWindow* window, int width, int height);
    static void CenterWindow(wxWindow* window, wxWindow* parent = nullptr);
    static void CenterWindowOnScreen(wxWindow* window);
    static wxSize GetOptimalSize(wxWindow* window);
    static void SetWindowAspectRatio(wxWindow* window, double ratio);
    
    // Font utilities
    static wxFont GetScaledFont(const wxFont& base_font, double scale);
    static wxFont GetMonospaceFont(int point_size = -1);
    static wxFont GetBoldFont(const wxFont& base_font);
    static wxFont GetItalicFont(const wxFont& base_font);
    static int GetTextWidth(const wxString& text, const wxFont& font);
    static int GetTextHeight(const wxFont& font);
    
    // Icon and bitmap utilities
    static wxBitmap LoadScaledBitmap(const wxString& path, int width, int height);
    static wxBitmap CreateColorBitmap(const wxColour& color, int width, int height);
    static wxBitmap ResizeBitmap(const wxBitmap& bitmap, int width, int height);
    static wxIcon LoadIcon(const wxString& path, int size = 32);
    
    // Theme utilities
    static bool IsDarkTheme();
    static wxColour GetDefaultBackgroundColor();
    static wxColour GetDefaultForegroundColor();
    static wxColour GetDefaultAccentColor();
    
    // Animation helpers
    static double EaseInOut(double t);
    static double EaseIn(double t);
    static double EaseOut(double t);
    static wxColour InterpolateColors(const wxColour& start, const wxColour& end, double progress);
    
    // Screen utilities
    static wxRect GetScreenRect();
    static wxRect GetWorkAreaRect();
    static double GetDisplayScaleFactor();
    static bool IsHighDPIDisplay();
    
    // Window state utilities
    static void SaveWindowGeometry(wxWindow* window, const wxString& key);
    static void RestoreWindowGeometry(wxWindow* window, const wxString& key);
    static bool IsWindowVisibleOnScreen(wxWindow* window);
    
    // Utility methods
    static void FlashWindow(wxWindow* window);
    static void SetWindowTransparency(wxWindow* window, int alpha);
    static wxString GetWindowTitle(wxWindow* window);
    
private:
    // Helper methods
    static wxColour ClampColor(int r, int g, int b);
    static double CalculateLuminance(const wxColour& color);
};

}

#endif // __GUI_UTILS_HPP