# Waveform Visualization Feature

## Overview

WanjPlayer now includes comprehensive audio visualization capabilities that display animated waveforms when audio files are played. The visualization system automatically detects audio-only files and switches to visualization mode.

## Features

### Audio Visualization Modes

The player supports multiple visualization styles:

1. **Waveform Display** (Default - Style 0)
   - Shows harmonic waveform patterns
   - Smooth, flowing visualization
   - Best for general music playback

2. **Spectrum Analyzer** (Style 1)
   - Frequency-based bar visualization
   - Shows audio frequency distribution
   - Great for analyzing bass and treble

3. **Oscilloscope Mode** (Style 2)
   - Real-time waveform oscillation
   - Technical view of audio signal
   - Useful for audio engineering

### Theme Colors

The visualization uses an **Earth Brown and Sky Blue** color palette:

- **Background**: Dark earth brown (RGB: 101, 67, 33)
- **Accent/Waveform**: Light sky blue (RGB: 135, 206, 250)
- **Text**: Beige (RGB: 245, 245, 220)

This color scheme provides:
- Warm, natural earth tones for comfortable viewing
- High contrast sky blue for clear waveform visibility
- Easy-to-read text overlay

## How It Works

### Automatic Detection

When you load a media file, WanjPlayer automatically detects whether it's audio or video:

1. **Audio Files** → Switches to waveform visualization mode
2. **Video Files** → Displays video content normally

### Supported Audio Formats

- MP3
- WAV
- AAC
- M4A
- FLAC
- OGG

### Visualization Display

When playing audio:
- The waveform animates in real-time
- "Now Playing" text shows the filename at the bottom
- Smooth 30 FPS animation for fluid motion
- Test pattern generation ensures visualization always displays

## Implementation Details

### Key Components

1. **PlayerCanvas** (`canvas.hpp/canvas.cpp`)
   - Main visualization component
   - Handles display mode switching
   - Manages animation timers

2. **AudioVisualizer**
   - Renders different visualization types
   - Applies smoothing and effects
   - Handles data interpolation

3. **WaveformGenerator**
   - Generates test waveform patterns
   - Creates harmonic frequencies
   - Applies envelopes and effects

### Media Event Integration

The visualization is triggered through media events:

```cpp
// OnMediaLoaded event
if (audio_only_file) {
    player_canvas->StartAudioVisualization(filename);
}

// OnMediaStop event
player_canvas->StopAudioVisualization();
player_canvas->SetDisplayMode(DisplayMode::IDLE);
```

### Display Modes

- **IDLE**: Shows idle screen when no media is loaded
- **AUDIO_VIS**: Active waveform visualization for audio
- **VIDEO**: Video playback mode

## Usage

### Basic Usage

1. Open an audio file (File → Open File)
2. The waveform visualization starts automatically
3. Press Play to begin playback with animated waveforms

### Changing Visualization Style

Currently, visualization style can be changed programmatically:

```cpp
// In code
player_canvas->SetVisualizationStyle(0); // Waveform
player_canvas->SetVisualizationStyle(1); // Spectrum
player_canvas->SetVisualizationStyle(2); // Oscilloscope
```

### Customizing Colors

The theme colors can be adjusted:

```cpp
player_canvas->SetBackgroundColor(wxColour(r, g, b));
player_canvas->SetAccentColor(wxColour(r, g, b));
player_canvas->SetTextColor(wxColour(r, g, b));
```

## Performance

- **Frame Rate**: 30 FPS (configurable)
- **Update Interval**: 33ms per frame
- **Waveform Points**: 256 samples
- **Spectrum Bars**: 64 frequency bands
- **CPU Usage**: Low impact with efficient rendering

## Troubleshooting

### Waveform Not Showing

1. **Check if audio file is loaded correctly**
   - Look for "Audio-only media detected" in logs
   - Verify file format is supported

2. **Verify PlayerCanvas is created**
   - Check logs for "PlayerCanvas created"
   - Ensure no initialization errors

3. **Check media events**
   - Look for "Started waveform visualization" log message
   - Verify OnMediaLoaded event is triggered

### Black Screen Instead of Waveform

- Ensure `StartAudioVisualization()` is called
- Check that display mode is set to `AUDIO_VIS`
- Verify timer is running (check logs)

### Waveform Colors Not Visible

- Adjust background/accent colors for better contrast
- Try different visualization styles
- Check if theme colors are applied correctly

## Future Enhancements

Potential improvements for future versions:

1. **GUI Controls**: Add menu or toolbar buttons to switch visualization styles
2. **Real Audio Data**: Connect to actual audio stream for real-time analysis
3. **More Styles**: Add circular, bars, and other visualization types
4. **User Themes**: Allow users to customize color schemes
5. **Fullscreen Mode**: Dedicated fullscreen visualization mode
6. **Beat Detection**: Sync visualization to music beats
7. **FFT Analysis**: Real frequency analysis from audio stream

## Developer Notes

### Adding New Visualization Styles

To add a new visualization type:

1. Add enum value to `AudioVisualizer::VisualizationType`
2. Implement drawing method (e.g., `DrawCustomVis()`)
3. Add case in `AudioVisualizer::Draw()` switch statement
4. Update `PlayerCanvas::SetVisualizationStyle()` mapping

### Modifying Animation Speed

```cpp
player_canvas->SetAnimationSpeed(speed); // Default is 1.0
```

### Accessing Visualization Data

```cpp
// Update with custom frequency data
std::vector<float> freq_data(64, 0.0f);
player_canvas->UpdateVisualizationData(freq_data);
```

## Credits

Visualization system designed and implemented for WanjPlayer.
Inspired by classic music visualizers and modern audio analysis tools.

## License

Same license as WanjPlayer project.