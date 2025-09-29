#ifndef __CANVAS_HPP
#define __CANVAS_HPP

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/mediactrl.h>
#include <wx/timer.h>
#include <wx/graphics.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <vector>
#include <memory>

namespace gui {

// Forward declarations
class AudioVisualizer;
class WaveformGenerator;

class PlayerCanvas : public wxPanel
{
public:
    enum class DisplayMode {
        VIDEO,           // Show video content
        AUDIO_VIS,       // Show audio visualization
        IDLE            // Show idle screen
    };
    
    PlayerCanvas(wxWindow* parent, wxWindowID id = wxID_ANY);
    ~PlayerCanvas();
    
    // Main functionality
    void SetMediaCtrl(wxMediaCtrl* media_ctrl);
    void SetDisplayMode(DisplayMode mode);
    DisplayMode GetDisplayMode() const { return current_mode; }
    
    // Audio visualization
    void StartAudioVisualization(const wxString& filename);
    void StopAudioVisualization();
    void UpdateVisualizationData(const std::vector<float>& frequency_data);
    
    // Video display
    void OptimizeVideoDisplay();
    void SetVideoAspectRatio(double ratio);
    void SetVideoScaleMode(int scale_mode); // 0=fit, 1=fill, 2=stretch
    
    // Now playing info
    void SetNowPlayingText(const wxString& text);
    void ShowNowPlayingInfo(bool show);
    bool IsNowPlayingInfoVisible() const { return show_now_playing; }
    
    // Visual customization
    void SetBackgroundColor(const wxColour& color);
    void SetTextColor(const wxColour& color);
    void SetAccentColor(const wxColour& color);
    void SetVisualizationStyle(int style); // 0=waveform, 1=spectrum, 2=oscilloscope
    
    // Animation control
    void StartAnimations();
    void StopAnimations();
    void SetAnimationSpeed(double speed);
    
    // Canvas management
    void Clear();
    void Refresh();
    void Update();
    
protected:
    // Event handlers
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnIdle(wxIdleEvent& event);
    
    // Drawing methods
    void DrawVideoContent(wxGraphicsContext* gc, const wxRect& rect);
    void DrawAudioVisualization(wxGraphicsContext* gc, const wxRect& rect);
    void DrawIdleScreen(wxGraphicsContext* gc, const wxRect& rect);
    void DrawNowPlayingInfo(wxGraphicsContext* gc, const wxRect& rect);
    void DrawBackground(wxGraphicsContext* gc, const wxRect& rect);
    
    // Audio visualization drawing
    void DrawWaveform(wxGraphicsContext* gc, const wxRect& rect);
    void DrawSpectrum(wxGraphicsContext* gc, const wxRect& rect);
    void DrawOscilloscope(wxGraphicsContext* gc, const wxRect& rect);
    void DrawFrequencyBars(wxGraphicsContext* gc, const wxRect& rect);
    
    // Helper methods
    void InitializeGraphics();
    void UpdateCanvasSize();
    void CalculateVideoRect(const wxRect& canvas_rect, wxRect& video_rect);
    void GenerateTestFrequencyData();
    wxRect CalculateCenteredRect(const wxSize& content_size, const wxRect& container);
    
    // Animation helpers
    void UpdateAnimations();
    double GetAnimationProgress() const;
    
private:
    // Core components
    wxMediaCtrl* media_ctrl;
    DisplayMode current_mode;
    
    // Audio visualization
    std::unique_ptr<AudioVisualizer> audio_visualizer;
    std::unique_ptr<WaveformGenerator> waveform_generator;
    std::vector<float> frequency_data;
    std::vector<float> waveform_data;
    int visualization_style;
    
    // Video display
    double video_aspect_ratio;
    int video_scale_mode;
    wxRect video_display_rect;
    
    // Now playing info
    wxString now_playing_text;
    bool show_now_playing;
    wxFont now_playing_font;
    wxRect text_rect;
    
    // Visual properties
    wxColour background_color;
    wxColour text_color;
    wxColour accent_color;
    wxColour secondary_color;
    
    // Animation
    wxTimer animation_timer;
    wxTimer visualization_timer;
    double animation_speed;
    long long animation_start_time;
    bool animations_enabled;
    
    // Graphics
    wxGraphicsRenderer* graphics_renderer;
    wxBitmap buffer_bitmap;
    bool double_buffered;
    
    // Layout
    wxSize canvas_size;
    bool size_changed;
    
    // Performance
    bool enable_smooth_rendering;
    int fps_limit;
    long long last_frame_time;
    
    // Constants
    static const int DEFAULT_FPS = 30;
    static const int VISUALIZATION_UPDATE_MS = 33; // ~30fps
    static const int WAVEFORM_POINTS = 256;
    static const int SPECTRUM_BARS = 64;
    
    DECLARE_EVENT_TABLE()
};

// Audio visualization helper class
class AudioVisualizer
{
public:
    enum class VisualizationType {
        WAVEFORM,
        SPECTRUM,
        OSCILLOSCOPE,
        BARS,
        CIRCLE
    };
    
    AudioVisualizer();
    ~AudioVisualizer();
    
    void SetType(VisualizationType type);
    void SetData(const std::vector<float>& frequency_data, const std::vector<float>& waveform_data);
    void Draw(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color);
    void Update();
    
    // Configuration
    void SetSensitivity(double sensitivity);
    void SetSmoothness(double smoothness);
    void SetBarCount(int count);
    void SetAmplification(double amp);
    
private:
    VisualizationType vis_type;
    std::vector<float> current_freq_data;
    std::vector<float> current_wave_data;
    std::vector<float> smoothed_data;
    
    // Configuration
    double sensitivity;
    double smoothness;
    int bar_count;
    double amplification;
    
    // Animation
    double phase_offset;
    
    // Drawing helpers
    void DrawWaveformVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color);
    void DrawSpectrumVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color);
    void DrawBarsVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color);
    void DrawCircleVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color);
    void SmoothData();
};

// Waveform generator for creating animated waveforms
class WaveformGenerator
{
public:
    WaveformGenerator();
    
    void GenerateWaveform(std::vector<float>& waveform, int points, double frequency, double amplitude, double phase);
    void GenerateTestPattern(std::vector<float>& data, int points);
    void AddNoise(std::vector<float>& data, double noise_level);
    void ApplyEnvelope(std::vector<float>& data, double attack, double decay);
    
private:
    double current_phase;
    double noise_seed;
};

}

#endif // __CANVAS_HPP