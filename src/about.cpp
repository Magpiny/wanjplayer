#include "about.hpp"

About::About(wxFrame* parent) : _parent(parent) {};

void About::create_about_page() {
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetName("WanjPlayer");
  aboutInfo.SetVersion("0.1.0");  // Replace with your actual version macro
  aboutInfo.SetDescription(_("Simple Media Player"));
  aboutInfo.SetCopyright("(C) 2025 Samuel Wanjare");
  aboutInfo.SetWebSite("https://www.magpiny.dev");
  aboutInfo.AddDeveloper("Samuel Wanjare");
  aboutInfo.AddDocWriter("WSammy");
  aboutInfo.AddTranslator("Magpiny BO");
  aboutInfo.AddArtist("Wanjman");

  wxAboutBox(aboutInfo);
};

About::~About() {};
