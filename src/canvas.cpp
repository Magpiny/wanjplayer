#include "../include/canvas.hpp"
#include "utils.hpp"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <wx/filename.h>
#include <cmath>
#include <algorithm>
#include <random>

namespace gui {

// Event table
wxBEGIN_EVENT_TABLE(PlayerCanvas, wxPanel)
    EVT_PAINT(PlayerCanvas::OnPaint)
    EVT_SIZE(PlayerCanvas::OnSize)
    EVT_ERASE_BACKGROUND(PlayerCanvas::OnEraseBackground)
    EVT_TIMER(wxID_ANY, PlayerCanvas::OnTimer)
    EVT_IDLE(PlayerCanvas::OnIdle)
wxEND_EVENT_TABLE()

PlayerCanvas::PlayerCanvas(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
    , current_mode(DisplayMode::IDLE)
    , audio_visualizer(std::make_unique<AudioVisualizer>())
    , waveform_generator(std::make_unique<WaveformGenerator>())
    , visualization_style(0)
    , video_aspect_ratio(16.0 / 9.0)
    , video_scale_mode(0)
    , show_now_playing(false)
    , background_color(*wxBLACK)
    , text_color(*wxWHITE)
    , accent_color(wxColour(0, 150, 136))
    , secondary_color(wxColour(76, 175, 80))
    , animation_timer(this, wxID_ANY)
    , visualization_timer(this, wxID_ANY)
    , animation_speed(1.0)
    , animation_start_time(0)
    , animations_enabled(true)
    , graphics_renderer(nullptr)
    , double_buffered(true)
    , size_changed(true)
    , enable_smooth_rendering(true)
    , fps_limit(DEFAULT_FPS)
    , last_frame_time(0)
{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    InitializeGraphics();

    // Initialize frequency and waveform data
    frequency_data.resize(SPECTRUM_BARS, 0.0f);
    waveform_data.resize(WAVEFORM_POINTS, 0.0f);

    // Setup fonts
    now_playing_font = wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    // Start animation timer
    if (animations_enabled) {
        animation_timer.Start(1000 / fps_limit);
    }

    // Generate initial test data
    GenerateTestFrequencyData();
}

PlayerCanvas::~PlayerCanvas()
{
    StopAnimations();
    StopAudioVisualization();
}


void PlayerCanvas::SetDisplayMode(DisplayMode mode)
{
    if (current_mode != mode) {
        current_mode = mode;

        switch (mode) {
            case DisplayMode::VIDEO:
                StopAudioVisualization();
                ShowNowPlayingInfo(false);
                break;

            case DisplayMode::AUDIO_VIS:
                StartAnimations();
                ShowNowPlayingInfo(true);
                break;

            case DisplayMode::IDLE:
                StopAudioVisualization();
                ShowNowPlayingInfo(false);
                break;
        }

        wxPanel::Refresh();
    }
}

void PlayerCanvas::StartAudioVisualization(const wxString& filename)
{
    SetDisplayMode(DisplayMode::AUDIO_VIS);

    // Extract filename for display
    wxFileName fn(filename);
    SetNowPlayingText("Now Playing: " + fn.GetName());

    // Start visualization timer
    if (!visualization_timer.IsRunning()) {
        visualization_timer.Start(VISUALIZATION_UPDATE_MS);
    }

    // Initialize animation
    animation_start_time = wxGetLocalTimeMillis().GetValue();
}

void PlayerCanvas::StopAudioVisualization()
{
    if (visualization_timer.IsRunning()) {
        visualization_timer.Stop();
    }

    // Clear visualization data
    std::fill(frequency_data.begin(), frequency_data.end(), 0.0f);
    std::fill(waveform_data.begin(), waveform_data.end(), 0.0f);
}

void PlayerCanvas::UpdateVisualizationData(const std::vector<float>& freq_data)
{
    if (current_mode == DisplayMode::AUDIO_VIS) {
        // Update frequency data
        size_t copy_size = std::min(freq_data.size(), frequency_data.size());
        std::copy(freq_data.begin(), freq_data.begin() + copy_size, frequency_data.begin());

        // Update visualizer
        audio_visualizer->SetData(frequency_data, waveform_data);

        wxPanel::Refresh();
    }
}



void PlayerCanvas::SetNowPlayingText(const wxString& text)
{
    now_playing_text = text;
    if (show_now_playing) {
        wxPanel::Refresh();
    }
}

void PlayerCanvas::ShowNowPlayingInfo(bool show)
{
    show_now_playing = show;
    wxPanel::Refresh();
}

void PlayerCanvas::SetBackgroundColor(const wxColour& color)
{
    background_color = color;
    wxPanel::Refresh();
}

void PlayerCanvas::SetTextColor(const wxColour& color)
{
    text_color = color;
    wxPanel::Refresh();
}

void PlayerCanvas::SetAccentColor(const wxColour& color)
{
    accent_color = color;
    wxPanel::Refresh();
}

void PlayerCanvas::SetVisualizationStyle(int style)
{
    visualization_style = style;

    // Update visualizer type
    AudioVisualizer::VisualizationType vis_type;
    switch (style) {
        case 1:
            vis_type = AudioVisualizer::VisualizationType::SPECTRUM;
            break;
        case 2:
            vis_type = AudioVisualizer::VisualizationType::OSCILLOSCOPE;
            break;
        case 3:
            vis_type = AudioVisualizer::VisualizationType::BARS;
            break;
        case 4:
            vis_type = AudioVisualizer::VisualizationType::CIRCLE;
            break;
        default:
            vis_type = AudioVisualizer::VisualizationType::WAVEFORM;
            break;
    }

    audio_visualizer->SetType(vis_type);
    wxPanel::Refresh();
}

void PlayerCanvas::StartAnimations()
{
    animations_enabled = true;
    if (!animation_timer.IsRunning()) {
        animation_timer.Start(1000 / fps_limit);
    }
    animation_start_time = wxGetLocalTimeMillis().GetValue();
}

void PlayerCanvas::StopAnimations()
{
    animations_enabled = false;
    if (animation_timer.IsRunning()) {
        animation_timer.Stop();
    }
}

void PlayerCanvas::SetAnimationSpeed(double speed)
{
    animation_speed = std::max(0.1, std::min(5.0, speed));
}

void PlayerCanvas::Clear()
{
    wxClientDC dc(this);
    dc.SetBackground(wxBrush(background_color));
    dc.Clear();
}

void PlayerCanvas::Refresh()
{
    wxPanel::Refresh();
}

void PlayerCanvas::Update()
{
    wxPanel::Update();
}

void PlayerCanvas::OnPaint(wxPaintEvent& event)
{
    utils::PerformanceTimer timer("OnPaint");
    wxAutoBufferedPaintDC dc(this);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (!gc) {
        return;
    }

    wxRect rect = GetClientRect();

    // Clear background
    DrawBackground(gc, rect);

    // Draw content based on current mode
    switch (current_mode) {
        case DisplayMode::VIDEO:
            DrawVideoContent(gc, rect);
            break;

        case DisplayMode::AUDIO_VIS:
            DrawAudioVisualization(gc, rect);
            break;

        case DisplayMode::IDLE:
            DrawIdleScreen(gc, rect);
            break;
    }

    // Draw now playing info if enabled
    if (show_now_playing) {
        DrawNowPlayingInfo(gc, rect);
    }

    delete gc;
}

void PlayerCanvas::OnSize(wxSizeEvent& event)
{
    canvas_size = event.GetSize();
    size_changed = true;
    UpdateCanvasSize();
    event.Skip();
}

void PlayerCanvas::OnEraseBackground(wxEraseEvent& event)
{
    // Do nothing - we handle all drawing in OnPaint
}

void PlayerCanvas::OnTimer(wxTimerEvent& event)
{
    if (event.GetTimer().GetId() == animation_timer.GetId()) {
        UpdateAnimations();
        if (current_mode == DisplayMode::AUDIO_VIS || current_mode == DisplayMode::IDLE) {
            wxPanel::Refresh();
        }
    } else if (event.GetTimer().GetId() == visualization_timer.GetId()) {
        // Generate test data for visualization
        GenerateTestFrequencyData();
        audio_visualizer->Update();
        wxPanel::Refresh();
    }
}

void PlayerCanvas::OnIdle(wxIdleEvent& event)
{
    // Limit frame rate
    long long current_time = wxGetLocalTimeMillis().GetValue();
    if (current_time - last_frame_time >= (1000 / fps_limit)) {
        last_frame_time = current_time;
        event.RequestMore();
    }
}

void PlayerCanvas::DrawVideoContent(wxGraphicsContext* gc, const wxRect& rect)
{
    // If no media control or not shown, draw placeholder
    gc->SetBrush(wxBrush(wxColour(32, 32, 32)));
    gc->DrawRectangle(rect.x, rect.y, rect.width, rect.height);

    // Draw "No Video" message
    gc->SetFont(now_playing_font, text_color);
    wxString message = "Video Player Ready";
    wxDouble text_width, text_height;
    gc->GetTextExtent(message, &text_width, &text_height);

    double x = rect.x + (rect.width - text_width) / 2.0;
    double y = rect.y + (rect.height - text_height) / 2.0;

    gc->DrawText(message, x, y);
}

void PlayerCanvas::DrawAudioVisualization(wxGraphicsContext* gc, const wxRect& rect)
{
    // Create visualization area (leave space for text)
    wxRect vis_rect = rect;
    if (show_now_playing) {
        vis_rect.height -= 80; // Leave space at bottom for text
    }

    // Draw visualization
    audio_visualizer->Draw(gc, vis_rect, accent_color);

    // Add subtle glow effect with simple gradient
    wxColour glow_color(accent_color.Red(), accent_color.Green(), accent_color.Blue(), 30);
    gc->SetBrush(wxBrush(glow_color));
    gc->DrawRectangle(vis_rect.x, vis_rect.y, vis_rect.width, vis_rect.height);
}

void PlayerCanvas::DrawIdleScreen(wxGraphicsContext* gc, const wxRect& rect)
{
    // Draw subtle pattern or logo
    gc->SetBrush(wxBrush(wxColour(16, 16, 16)));
    gc->DrawRectangle(rect.x, rect.y, rect.width, rect.height);

    // Draw animated circles
    double progress = GetAnimationProgress();
    int num_circles = 5;

    for (int i = 0; i < num_circles; i++) {
        double phase = (progress + i * 0.2) * 2 * M_PI;
        double alpha = (sin(phase) + 1.0) * 0.5 * 50; // 0-50 alpha

        wxColour circle_color(accent_color.Red(), accent_color.Green(), accent_color.Blue(), (int)alpha);
        gc->SetBrush(wxBrush(circle_color));
        gc->SetPen(wxPen(circle_color, 2));

        double radius = 20 + i * 10;
        double center_x = rect.x + rect.width / 2.0;
        double center_y = rect.y + rect.height / 2.0;

        gc->DrawEllipse(center_x - radius, center_y - radius, radius * 2, radius * 2);
    }

    // Draw application name
    gc->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT),
                wxColour(text_color.Red(), text_color.Green(), text_color.Blue(), 128));

    wxString app_name = "WanjPlayer";
    wxDouble text_width, text_height;
    gc->GetTextExtent(app_name, &text_width, &text_height);

    double x = rect.x + (rect.width - text_width) / 2.0;
    double y = rect.y + (rect.height - text_height) / 2.0 + 80;

    gc->DrawText(app_name, x, y);
}

void PlayerCanvas::DrawNowPlayingInfo(wxGraphicsContext* gc, const wxRect& rect)
{
    if (now_playing_text.IsEmpty()) {
        return;
    }

    // Create text area at bottom
    wxRect text_area = rect;
    text_area.y = rect.y + rect.height - 80;
    text_area.height = 80;

    // Draw semi-transparent background
    wxColour bg_color(0, 0, 0, 180);
    gc->SetBrush(wxBrush(bg_color));
    gc->DrawRectangle(text_area.x, text_area.y, text_area.width, text_area.height);

    // Draw text
    gc->SetFont(now_playing_font, text_color);

    wxDouble text_width, text_height;
    gc->GetTextExtent(now_playing_text, &text_width, &text_height);

    double x = text_area.x + (text_area.width - text_width) / 2.0;
    double y = text_area.y + (text_area.height - text_height) / 2.0;

    gc->DrawText(now_playing_text, x, y);

    // Draw decorative line
    gc->SetPen(wxPen(accent_color, 2));
    double line_width = text_width * 0.8;
    double line_x = x + (text_width - line_width) / 2.0;
    double line_y = y + text_height + 5;

    gc->StrokeLine(line_x, line_y, line_x + line_width, line_y);
}

void PlayerCanvas::DrawBackground(wxGraphicsContext* gc, const wxRect& rect)
{
    if (current_mode == DisplayMode::VIDEO) {
        // For video, use pure black background to eliminate borders
        gc->SetBrush(wxBrush(*wxBLACK));
    } else {
        // For audio/idle, use gradient background
        wxGraphicsBrush gradient = gc->CreateLinearGradientBrush(
            rect.x, rect.y, rect.x, rect.y + rect.height,
            background_color,
            wxColour(background_color.Red() + 10, background_color.Green() + 10, background_color.Blue() + 10));
        gc->SetBrush(gradient);
    }

    gc->DrawRectangle(rect.x, rect.y, rect.width, rect.height);
}

void PlayerCanvas::InitializeGraphics()
{
    graphics_renderer = wxGraphicsRenderer::GetDefaultRenderer();
}

void PlayerCanvas::UpdateCanvasSize()
{
    if (size_changed) {
        // Update buffer bitmap if using double buffering
        if (double_buffered) {
            if (canvas_size.x > 0 && canvas_size.y > 0) {
                buffer_bitmap = wxBitmap(canvas_size);
            }
        }
        size_changed = false;
    }
}

void PlayerCanvas::CalculateVideoRect(const wxRect& canvas_rect, wxRect& video_rect)
{
    if (video_scale_mode == 0) { // Fit
        // Maintain aspect ratio, fit within canvas
        double canvas_ratio = (double)canvas_rect.width / canvas_rect.height;

        if (video_aspect_ratio > canvas_ratio) {
            // Video is wider than canvas
            video_rect.width = canvas_rect.width;
            video_rect.height = (int)(canvas_rect.width / video_aspect_ratio);
            video_rect.x = canvas_rect.x;
            video_rect.y = canvas_rect.y + (canvas_rect.height - video_rect.height) / 2;
        } else {
            // Video is taller than canvas
            video_rect.height = canvas_rect.height;
            video_rect.width = (int)(canvas_rect.height * video_aspect_ratio);
            video_rect.x = canvas_rect.x + (canvas_rect.width - video_rect.width) / 2;
            video_rect.y = canvas_rect.y;
        }
    } else if (video_scale_mode == 1) { // Fill
        // Fill entire canvas, may crop video
        video_rect = canvas_rect;
    } else { // Stretch
        // Stretch to fit canvas exactly
        video_rect = canvas_rect;
    }
}

void PlayerCanvas::GenerateTestFrequencyData()
{
    if (current_mode != DisplayMode::AUDIO_VIS) {
        return;
    }

    // Use a simple random generator to create more dynamic-looking test data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Generate random frequency data to simulate a lively audio track
    for (size_t i = 0; i < frequency_data.size(); ++i) {
        // Introduce some randomness and smooth transitions
        float random_val = dis(gen);
        frequency_data[i] = (frequency_data[i] * 0.7f) + (random_val * 0.3f);
    }

    // Generate a more complex waveform than a simple sine wave
    double time = GetAnimationProgress() * animation_speed;
    waveform_generator->GenerateWaveform(waveform_data, WAVEFORM_POINTS, 220.0 + 110.0 * sin(time * 2), 0.6, time);
    waveform_generator->AddNoise(waveform_data, 0.1);
}

wxRect PlayerCanvas::CalculateCenteredRect(const wxSize& content_size, const wxRect& container)
{
    wxRect centered;
    centered.width = content_size.GetWidth();
    centered.height = content_size.GetHeight();
    centered.x = container.x + (container.width - centered.width) / 2;
    centered.y = container.y + (container.height - centered.height) / 2;
    return centered;
}

void PlayerCanvas::UpdateAnimations()
{
    if (animations_enabled && audio_visualizer) {
        audio_visualizer->Update();
    }
}

double PlayerCanvas::GetAnimationProgress() const
{
    if (animation_start_time == 0) {
        return 0.0;
    }

    long long current_time = wxGetLocalTimeMillis().GetValue();
    double elapsed = (current_time - animation_start_time) / 1000.0; // Convert to seconds
    return fmod(elapsed * animation_speed, 1.0);
}

// AudioVisualizer implementation

AudioVisualizer::AudioVisualizer()
    : vis_type(VisualizationType::WAVEFORM)
    , sensitivity(1.0)
    , smoothness(0.8)
    , bar_count(64)
    , amplification(1.5)
    , phase_offset(0.0)
{
    current_freq_data.resize(bar_count, 0.0f);
    current_wave_data.resize(256, 0.0f);
    smoothed_data.resize(bar_count, 0.0f);
}

AudioVisualizer::~AudioVisualizer() = default;

void AudioVisualizer::SetType(VisualizationType type)
{
    vis_type = type;
}

void AudioVisualizer::SetData(const std::vector<float>& frequency_data, const std::vector<float>& waveform_data)
{
    current_freq_data = frequency_data;
    current_wave_data = waveform_data;

    // Resize if needed
    if (current_freq_data.size() != smoothed_data.size()) {
        smoothed_data.resize(current_freq_data.size(), 0.0f);
    }

    SmoothData();
}

void AudioVisualizer::Draw(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color)
{
    switch (vis_type) {
        case VisualizationType::WAVEFORM:
            DrawWaveformVis(gc, rect, color);
            break;
        case VisualizationType::SPECTRUM:
            DrawSpectrumVis(gc, rect, color);
            break;
        case VisualizationType::BARS:
            DrawBarsVis(gc, rect, color);
            break;
        case VisualizationType::CIRCLE:
            DrawCircleVis(gc, rect, color);
            break;
        default:
            DrawWaveformVis(gc, rect, color);
            break;
    }
}

void AudioVisualizer::Update()
{
    phase_offset += 0.05;
    if (phase_offset > 2 * M_PI) {
        phase_offset -= 2 * M_PI;
    }
}

void AudioVisualizer::SetSensitivity(double sens)
{
    sensitivity = std::max(0.1, std::min(5.0, sens));
}

void AudioVisualizer::SetSmoothness(double smooth)
{
    smoothness = std::max(0.0, std::min(1.0, smooth));
}

void AudioVisualizer::SetBarCount(int count)
{
    bar_count = std::max(8, std::min(256, count));
}

void AudioVisualizer::SetAmplification(double amp)
{
    amplification = std::max(0.1, std::min(10.0, amp));
}

void AudioVisualizer::DrawWaveformVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color)
{
    if (current_wave_data.empty()) return;

    gc->SetPen(wxPen(color, 2));

    wxGraphicsPath path = gc->CreatePath();

    double x_step = (double)rect.width / (current_wave_data.size() - 1);
    double center_y = rect.y + rect.height / 2.0;
    double amplitude_scale = rect.height / 4.0 * amplification;

    bool first_point = true;
    for (size_t i = 0; i < current_wave_data.size(); i++) {
        double x = rect.x + i * x_step;
        double y = center_y + current_wave_data[i] * amplitude_scale;

        if (first_point) {
            path.MoveToPoint(x, y);
            first_point = false;
        } else {
            path.AddLineToPoint(x, y);
        }
    }

    gc->StrokePath(path);
}

void AudioVisualizer::DrawSpectrumVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color)
{
    if (smoothed_data.empty()) return;

    double bar_width = (double)rect.width / smoothed_data.size();

    for (size_t i = 0; i < smoothed_data.size(); i++) {
        double x = rect.x + i * bar_width;
        double height = smoothed_data[i] * rect.height * amplification * sensitivity;
        double y = rect.y + rect.height - height;

        // Color gradient based on frequency
        double freq_ratio = (double)i / smoothed_data.size();
        wxColour bar_color(
            (int)(color.Red() * (1.0 - freq_ratio) + 255 * freq_ratio),
            (int)(color.Green() * (1.0 - freq_ratio * 0.5)),
            (int)(color.Blue() * (1.0 + freq_ratio * 0.5))
        );

        gc->SetBrush(wxBrush(bar_color));
        gc->DrawRectangle(x, y, bar_width - 1, height);
    }
}

void AudioVisualizer::DrawBarsVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color)
{
    if (smoothed_data.empty()) return;

    double bar_width = (double)rect.width / smoothed_data.size() * 0.8;
    double bar_spacing = (double)rect.width / smoothed_data.size();

    for (size_t i = 0; i < smoothed_data.size(); i++) {
        double x = rect.x + i * bar_spacing + bar_spacing * 0.1;
        double height = smoothed_data[i] * rect.height * amplification * sensitivity;
        double y = rect.y + rect.height - height;

        // Gradient fill
        wxGraphicsBrush gradient = gc->CreateLinearGradientBrush(
            x, y + height, x, y,
            wxColour(color.Red(), color.Green(), color.Blue(), 100),
            color
        );

        gc->SetBrush(gradient);
        gc->DrawRectangle(x, y, bar_width, height);
    }
}

void AudioVisualizer::DrawCircleVis(wxGraphicsContext* gc, const wxRect& rect, const wxColour& color)
{
    if (smoothed_data.empty()) return;

    double center_x = rect.x + rect.width / 2.0;
    double center_y = rect.y + rect.height / 2.0;
    double max_radius = std::min(rect.width, rect.height) / 3.0;

    gc->SetPen(wxPen(color, 1));

    for (size_t i = 0; i < smoothed_data.size(); i++) {
        double angle = 2 * M_PI * i / smoothed_data.size() + phase_offset;
        double radius = max_radius + smoothed_data[i] * max_radius * amplification * sensitivity;

        double x = center_x + radius * cos(angle);
        double y = center_y + radius * sin(angle);

        gc->DrawEllipse(x - 2, y - 2, 4, 4);
    }
}

void AudioVisualizer::SmoothData()
{
    for (size_t i = 0; i < current_freq_data.size() && i < smoothed_data.size(); i++) {
        smoothed_data[i] = smoothed_data[i] * smoothness + current_freq_data[i] * (1.0 - smoothness);
    }
}

// WaveformGenerator implementation

WaveformGenerator::WaveformGenerator()
    : current_phase(0.0)
    , noise_seed(123456.789)
{
}

void WaveformGenerator::GenerateWaveform(std::vector<float>& waveform, int points, double frequency, double amplitude, double phase)
{
    waveform.resize(points);

    for (int i = 0; i < points; i++) {
        double t = (double)i / points * 2 * M_PI;
        waveform[i] = amplitude * sin(frequency * t + phase);
    }

    // Add some harmonics for interesting waveform
    for (int i = 0; i < points; i++) {
        double t = (double)i / points * 2 * M_PI;
        waveform[i] += amplitude * 0.3 * sin(frequency * 2 * t + phase * 1.5);
        waveform[i] += amplitude * 0.1 * sin(frequency * 3 * t + phase * 2);
    }
}

void WaveformGenerator::GenerateTestPattern(std::vector<float>& data, int points)
{
    data.resize(points);
    std::mt19937 rng(static_cast<unsigned>(noise_seed));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0; i < points; i++) {
        data[i] = dist(rng) * 0.5f; // Random values between 0 and 0.5
    }
}

void WaveformGenerator::AddNoise(std::vector<float>& data, double noise_level)
{
    std::mt19937 rng(static_cast<unsigned>(noise_seed));
    std::uniform_real_distribution<float> dist(-noise_level, noise_level);

    for (float& sample : data) {
        sample += dist(rng);
        sample = std::max(-1.0f, std::min(1.0f, sample));
    }

    noise_seed += 1.0; // Change seed for next call
}

void WaveformGenerator::ApplyEnvelope(std::vector<float>& data, double attack, double decay)
{
    if (data.empty()) return;

    size_t attack_samples = static_cast<size_t>(attack * data.size());
    size_t decay_samples = static_cast<size_t>(decay * data.size());

    // Attack phase
    for (size_t i = 0; i < attack_samples && i < data.size(); i++) {
        double envelope = (double)i / attack_samples;
        data[i] *= envelope;
    }

    // Decay phase
    for (size_t i = data.size() - decay_samples; i < data.size(); i++) {
        double envelope = (double)(data.size() - i) / decay_samples;
        data[i] *= envelope;
    }
}

}
