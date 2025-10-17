#include "preferences.hpp"
#include "utils.hpp"
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

// IDs for controls
enum {
    ID_ClearLogs = wxID_HIGHEST + 1,
    ID_ClearPerfData,
    ID_CleanupTemp,
    ID_BrowseLogFile
};

PreferencesDialog::PreferencesDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Preferences", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    config = wxConfig::Get();

    notebook = new wxNotebook(this, wxID_ANY);

    CreateGeneralPage(notebook);
    CreateLoggingPage(notebook);
    CreatePerformancePage(notebook);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

    wxStdDialogButtonSizer* button_sizer = new wxStdDialogButtonSizer();
    button_sizer->AddButton(new wxButton(this, wxID_OK));
    button_sizer->AddButton(new wxButton(this, wxID_CANCEL));
    button_sizer->AddButton(new wxButton(this, wxID_APPLY));
    button_sizer->Realize();

    main_sizer->Add(button_sizer, 0, wxALIGN_RIGHT | wxALL, 5);

    SetSizerAndFit(main_sizer);
    CenterOnParent();

    LoadSettings();

    Bind(wxEVT_BUTTON, &PreferencesDialog::OnOk, this, wxID_OK);
    Bind(wxEVT_BUTTON, &PreferencesDialog::OnCancel, this, wxID_CANCEL);
    Bind(wxEVT_BUTTON, &PreferencesDialog::OnApply, this, wxID_APPLY);
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::CreateGeneralPage(wxNotebook* parent_notebook) {
    wxPanel* page = new wxPanel(parent_notebook);
    parent_notebook->AddPage(page, "General");

    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

    // Window Behavior
    wxStaticBoxSizer* window_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "Window Behavior");
    remember_geometry_checkbox = new wxCheckBox(window_sizer->GetStaticBox(), wxID_ANY, "Remember window size and position on exit");
    window_sizer->Add(remember_geometry_checkbox, 0, wxALL, 5);
    top_sizer->Add(window_sizer, 0, wxEXPAND | wxALL, 5);

    // Appearance
    wxStaticBoxSizer* appearance_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "Appearance");
    wxArrayString themes;
    themes.Add("System");
    themes.Add("Dark");
    themes.Add("Light");
    theme_choice = new wxChoice(appearance_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, themes);
    appearance_sizer->Add(new wxStaticText(appearance_sizer->GetStaticBox(), wxID_ANY, "Theme (requires restart):"), 0, wxALL, 5);
    appearance_sizer->Add(theme_choice, 0, wxEXPAND | wxALL, 5);

    appearance_sizer->Add(new wxStaticText(appearance_sizer->GetStaticBox(), wxID_ANY, "Window Transparency:"), 0, wxALL, 5);
    transparency_slider = new wxSlider(appearance_sizer->GetStaticBox(), wxID_ANY, 255, 0, 255);
    appearance_sizer->Add(transparency_slider, 0, wxEXPAND | wxALL, 5);
    top_sizer->Add(appearance_sizer, 0, wxEXPAND | wxALL, 5);

    page->SetSizer(top_sizer);
}

void PreferencesDialog::CreateLoggingPage(wxNotebook* parent_notebook)
{
    wxPanel* page = new wxPanel(parent_notebook);
    parent_notebook->AddPage(page, "Logging");

    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

    // Log Level
    wxStaticBoxSizer* log_level_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "Logging Level");
    wxArrayString levels;
    levels.Add("Debug"); levels.Add("Info"); levels.Add("Warning"); levels.Add("Error"); levels.Add("Critical");
    log_level_choice = new wxChoice(log_level_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, levels);
    log_level_sizer->Add(log_level_choice, 0, wxEXPAND | wxALL, 5);
    top_sizer->Add(log_level_sizer, 0, wxEXPAND | wxALL, 5);

    // File Logging
    wxStaticBoxSizer* file_log_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "File Logging");
    log_to_file_checkbox = new wxCheckBox(file_log_sizer->GetStaticBox(), wxID_ANY, "Enable file logging");
    file_log_sizer->Add(log_to_file_checkbox, 0, wxALL, 5);

    wxBoxSizer* path_sizer = new wxBoxSizer(wxHORIZONTAL);
    log_file_path_ctrl = new wxTextCtrl(file_log_sizer->GetStaticBox(), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    path_sizer->Add(log_file_path_ctrl, 1, wxEXPAND | wxALL, 5);
    wxButton* browse_button = new wxButton(file_log_sizer->GetStaticBox(), ID_BrowseLogFile, "...");
    path_sizer->Add(browse_button, 0, wxALL, 5);
    file_log_sizer->Add(path_sizer, 0, wxEXPAND);

    wxBoxSizer* rotation_sizer = new wxBoxSizer(wxHORIZONTAL);
    rotation_sizer->Add(new wxStaticText(file_log_sizer->GetStaticBox(), wxID_ANY, "Max file size (MB):"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    log_file_size_spin = new wxSpinCtrl(file_log_sizer->GetStaticBox(), wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 100, 10);
    rotation_sizer->Add(log_file_size_spin, 0, wxALL, 5);
    rotation_sizer->AddSpacer(20);
    rotation_sizer->Add(new wxStaticText(file_log_sizer->GetStaticBox(), wxID_ANY, "Files to keep:"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
    log_file_count_spin = new wxSpinCtrl(file_log_sizer->GetStaticBox(), wxID_ANY, "", wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 20, 5);
    rotation_sizer->Add(log_file_count_spin, 0, wxALL, 5);
    file_log_sizer->Add(rotation_sizer, 0, wxEXPAND);

    wxButton* clear_logs_button = new wxButton(file_log_sizer->GetStaticBox(), ID_ClearLogs, "Clear Log Files");
    file_log_sizer->Add(clear_logs_button, 0, wxALIGN_RIGHT | wxALL, 5);
    top_sizer->Add(file_log_sizer, 0, wxEXPAND | wxALL, 5);

    page->SetSizer(top_sizer);

    Bind(wxEVT_BUTTON, &PreferencesDialog::OnClearLogs, this, ID_ClearLogs);
    Bind(wxEVT_BUTTON, &PreferencesDialog::OnBrowseLogFile, this, ID_BrowseLogFile);
}

void PreferencesDialog::CreatePerformancePage(wxNotebook* parent_notebook) {
    wxPanel* page = new wxPanel(parent_notebook);
    parent_notebook->AddPage(page, "Performance");

    wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

    // Profiling
    wxStaticBoxSizer* profiling_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "Profiling");
    performance_profiling_checkbox = new wxCheckBox(profiling_sizer->GetStaticBox(), wxID_ANY, "Enable performance profiling");
    profiling_sizer->Add(performance_profiling_checkbox, 0, wxALL, 5);
    top_sizer->Add(profiling_sizer, 0, wxEXPAND | wxALL, 5);

    // Caching
    wxStaticBoxSizer* cache_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "Caching");
    cache_sizer->Add(new wxStaticText(cache_sizer->GetStaticBox(), wxID_ANY, "Max cache size (MB):"), 0, wxALL, 5);
    cache_size_spin = new wxSpinCtrl(cache_sizer->GetStaticBox(), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10, 1000, 100);
    cache_sizer->Add(cache_size_spin, 0, wxEXPAND | wxALL, 5);
    top_sizer->Add(cache_sizer, 0, wxEXPAND | wxALL, 5);

    // Actions
    wxStaticBoxSizer* actions_sizer = new wxStaticBoxSizer(wxVERTICAL, page, "Actions");
    wxButton* clear_perf_button = new wxButton(actions_sizer->GetStaticBox(), ID_ClearPerfData, "Clear Performance Data");
    actions_sizer->Add(clear_perf_button, 0, wxALL, 5);
    wxButton* cleanup_temp_button = new wxButton(actions_sizer->GetStaticBox(), ID_CleanupTemp, "Clear Temporary Files");
    actions_sizer->Add(cleanup_temp_button, 0, wxALL, 5);
    top_sizer->Add(actions_sizer, 0, wxEXPAND | wxALL, 5);

    page->SetSizer(top_sizer);

    Bind(wxEVT_BUTTON, &PreferencesDialog::OnClearPerfData, this, ID_ClearPerfData);
    Bind(wxEVT_BUTTON, &PreferencesDialog::OnCleanupTempFiles, this, ID_CleanupTemp);
}


void PreferencesDialog::LoadSettings()
{
    config->SetPath("/General");
    remember_geometry_checkbox->SetValue(config->Read("RememberGeometry", true));
    theme_choice->SetSelection(config->Read("Theme", 0L)); // 0=System
    transparency_slider->SetValue(config->Read("Transparency", 255L));

    config->SetPath("/Logging");
    log_level_choice->SetSelection(config->Read("LogLevel", (long)utils::LogUtils::LogLevel::INFO));
    log_to_file_checkbox->SetValue(config->Read("FileLoggingEnabled", false));
    log_file_path_ctrl->SetValue(config->Read("LogFilePath", utils::LogUtils::GetDefaultLogFileName()));
    log_file_size_spin->SetValue(config->Read("MaxLogSizeMB", 10L));
    log_file_count_spin->SetValue(config->Read("MaxLogFiles", 5L));

    config->SetPath("/Performance");
    performance_profiling_checkbox->SetValue(config->Read("ProfilingEnabled", true));
    cache_size_spin->SetValue(config->Read("MaxCacheSizeMB", 100L));
}

void PreferencesDialog::ApplySettings()
{
    // General
    GetParent()->SetTransparent(transparency_slider->GetValue());

    // Logging
    long logLevel = log_level_choice->GetSelection();
    utils::LogUtils::SetLogLevel(static_cast<utils::LogUtils::LogLevel>(logLevel));

    bool logToFile = log_to_file_checkbox->GetValue();
    wxString logPath = log_file_path_ctrl->GetValue();
    utils::LogUtils::EnableFileOutput(logToFile, logPath);

    utils::LogUtils::SetMaxLogFileSize(log_file_size_spin->GetValue() * 1024 * 1024);
    utils::LogUtils::SetMaxLogFiles(log_file_count_spin->GetValue());

    // Performance
    utils::PerformanceUtils::EnableProfiling(performance_profiling_checkbox->GetValue());
    utils::PerformanceUtils::SetMaxCacheSize(cache_size_spin->GetValue() * 1024 * 1024);
}

void PreferencesDialog::SaveSettings()
{
    config->SetPath("/General");
    config->Write("RememberGeometry", remember_geometry_checkbox->GetValue());
    config->Write("Theme", (long)theme_choice->GetSelection());
    config->Write("Transparency", (long)transparency_slider->GetValue());

    config->SetPath("/Logging");
    config->Write("LogLevel", (long)log_level_choice->GetSelection());
    config->Write("FileLoggingEnabled", log_to_file_checkbox->GetValue());
    config->Write("LogFilePath", log_file_path_ctrl->GetValue());
    config->Write("MaxLogSizeMB", (long)log_file_size_spin->GetValue());
    config->Write("MaxLogFiles", (long)log_file_count_spin->GetValue());

    config->SetPath("/Performance");
    config->Write("ProfilingEnabled", performance_profiling_checkbox->GetValue());
    config->Write("MaxCacheSizeMB", (long)cache_size_spin->GetValue());

    config->Flush();
}

void PreferencesDialog::OnOk(wxCommandEvent& event)
{
    ApplySettings();
    SaveSettings();
    EndModal(wxID_OK);
}

void PreferencesDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void PreferencesDialog::OnApply(wxCommandEvent& event)
{
    ApplySettings();
    SaveSettings();
}

void PreferencesDialog::OnClearLogs(wxCommandEvent& event)
{
    utils::LogUtils::ClearLogFile();
    wxMessageBox("Log file has been cleared.", "Logs Cleared", wxOK | wxICON_INFORMATION, this);
}

void PreferencesDialog::OnClearPerfData(wxCommandEvent& event)
{
    utils::PerformanceUtils::ClearPerformanceData();
    wxMessageBox("Performance data has been cleared.", "Performance Data Cleared", wxOK | wxICON_INFORMATION, this);
}

void PreferencesDialog::OnCleanupTempFiles(wxCommandEvent& event)
{
    if (utils::FileUtils::CleanupTempFiles()) {
        wxMessageBox("Temporary files have been cleaned up.", "Cleanup Successful", wxOK | wxICON_INFORMATION, this);
    } else {
        wxMessageBox("Could not delete all temporary files.", "Cleanup Warning", wxOK | wxICON_WARNING, this);
    }
}

void PreferencesDialog::OnBrowseLogFile(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, "Set Log File Path", "", "",
                                "Log files (*.log)|*.log", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    log_file_path_ctrl->SetValue(saveFileDialog.GetPath());
}
