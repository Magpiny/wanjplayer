#ifndef __PREFERENCES_HPP
#define __PREFERENCES_HPP

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/config.h>
#include <wx/slider.h>

class PreferencesDialog : public wxDialog
{
public:
    PreferencesDialog(wxWindow* parent);
    ~PreferencesDialog();

private:
    // Event Handlers
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnClearLogs(wxCommandEvent& event);
    void OnClearPerfData(wxCommandEvent& event);
    void OnCleanupTempFiles(wxCommandEvent& event);
    void OnBrowseLogFile(wxCommandEvent& event);

    // UI Creation
    void CreateGeneralPage(wxNotebook* notebook);
    void CreateLoggingPage(wxNotebook* notebook);
    void CreatePerformancePage(wxNotebook* notebook);

    // Settings Management
    void LoadSettings();
    void ApplySettings();
    void SaveSettings();

    // UI Controls
    wxNotebook* notebook;

    // General Page
    wxCheckBox* remember_geometry_checkbox;
    wxChoice* theme_choice;
    wxSlider* transparency_slider;

    // Logging Page
    wxChoice* log_level_choice;
    wxCheckBox* log_to_file_checkbox;
    wxTextCtrl* log_file_path_ctrl;
    wxSpinCtrl* log_file_size_spin;
    wxSpinCtrl* log_file_count_spin;

    // Performance Page
    wxCheckBox* performance_profiling_checkbox;
    wxSpinCtrl* cache_size_spin;

    wxConfigBase* config;
};

#endif // __PREFERENCES_HPP
