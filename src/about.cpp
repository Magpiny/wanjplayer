#include "about.hpp"

gui::About::About() {};

void
gui::About::create_about_page()
{
  wxAboutDialogInfo aboutInfo;
  aboutInfo.SetName("WanjPlayer");
  aboutInfo.SetVersion("0.1.0"); // Replace with your actual version macro
  aboutInfo.SetDescription(_("Simple Media Player"));
  aboutInfo.SetCopyright("(C) 2025 Samuel Wanjare");
  aboutInfo.SetWebSite("https://www.magpiny.dev");
  aboutInfo.AddDeveloper("Samuel Wanjare");
  aboutInfo.AddDocWriter("WSammy");
  aboutInfo.AddTranslator("Magpiny Bo");
  aboutInfo.AddArtist("Wanjman");

  wxAboutBox(aboutInfo);
};

gui::About::~About() {};
