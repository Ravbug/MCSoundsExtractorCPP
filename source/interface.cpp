///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/


#include "interface.h"
#include "extractor.hpp"
//linux or mac only
#if defined __APPLE__ || defined __linux__
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#endif

using namespace std;

//Constructor for the Frame
mainFrame::mainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	//window height (platform dependent)
	#ifdef __APPLE__
	int height = 250;
	#elif __linux
	int height = 400;
	#elif _WIN32
	int height = 310;
	this->SetBackgroundColour(*wxWHITE);
	#endif
	
	this->SetSizeHints( wxSize( -1,height ), wxSize( -1,height ) );

    //main sizer: Grid Bag Layout
	wxGridBagSizer* gb_main;
	gb_main = new wxGridBagSizer( 0, 0 );
	gb_main->SetFlexibleDirection( wxHORIZONTAL );
	gb_main->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

    //first label
	label_1 = new wxStaticText( this, wxID_ANY, wxT("Minecraft Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	label_1->Wrap( -1 );
	gb_main->Add( label_1, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

    //minecraft directory text field
	txt_mcDir = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    txt_mcDir->SetEditable(false);
	gb_main->Add( txt_mcDir, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

    //choose button for minecraft directory
	btn_chooseMcDir = new wxButton( this, wxID_OPEN, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	gb_main->Add( btn_chooseMcDir, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

    //output folder label
	label_2 = new wxStaticText( this, wxID_ANY, wxT("Output Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	label_2->Wrap( -1 );
	gb_main->Add( label_2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

    //output folder text field
	txt_outDir = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    txt_outDir->SetEditable(false);
	gb_main->Add( txt_outDir, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

    //choose button for output folder
	btn_chooseOutDir = new wxButton( this, wxID_SAVE, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	gb_main->Add( btn_chooseOutDir, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

    //minecraft version label
	label_3 = new wxStaticText( this, wxID_ANY, wxT("Minecraft Version"), wxDefaultPosition, wxDefaultSize, 0 );
	label_3->Wrap( -1 );
	gb_main->Add( label_3, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

    //combo box for selecting minecraft version
	wxArrayString choice_mcVersionChoices;
	choice_mcVersion = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choice_mcVersionChoices, 0 );
	choice_mcVersion->SetSelection( 0 );
	gb_main->Add( choice_mcVersion, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );

    //extract button (which starts the process)
	btn_extract = new wxButton( this, wxID_EXECUTE, wxT("Extract"), wxDefaultPosition, wxDefaultSize, 0 );
	gb_main->Add( btn_extract, wxGBPosition( 5, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );

    //progress bar which displays progress
	progressBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	progressBar->SetValue( 0 );
	gb_main->Add( progressBar, wxGBPosition( 6, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

    //set the first collumn to be resizable
	gb_main->AddGrowableCol( 0 );

	this->SetSizer( gb_main );
	this->Layout();

	this->Centre( wxBOTH );
    
    //menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_App, "&Hello...\tCtrl-H","Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );
    SetMenuBar( menuBar );
    
    //load defaults
    this->SetPlatformSpecificData();
	this->LoadMcVersions(txt_mcDir->GetValue().ToStdString());
}

//default destructor
mainFrame::~mainFrame(){}

//event table which routes clicks and other things to function calls
wxBEGIN_EVENT_TABLE(mainFrame, wxFrame)
EVT_MENU(wxID_EXIT, mainFrame::OnExit)
EVT_MENU(wxID_ABOUT, mainFrame::OnAbout)
EVT_BUTTON(wxID_OPEN,mainFrame::OnOpen)
EVT_BUTTON(wxID_SAVE,mainFrame::OnSave)
wxEND_EVENT_TABLE()

/**
 Called when the exit menu or shortcut is called (cmd+Q, etc)
 Closes the program
 */
void mainFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
/**
 Called on About menu pressed
 */
void mainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Fill in description text","About Minecraft Sounds Extractor", wxOK | wxICON_INFORMATION);
}

/**
 Called when the choose button for Minecraft Directory is clicked
 Gets a path from the user and sets txt_mcDir's contents accordingly
 @param event wxCommandEvent for the button (not used)
 */
void mainFrame::OnOpen(wxCommandEvent & event)
{
	string message = "Choose the folder where Minecraft is installed.";
	string path = GetPathFromDialog(message);
	if (path != ""){
		//load MC versions into combo box here
		if (LoadMcVersions(path)) {
			txt_mcDir->ChangeValue(wxString(path));
		}
	}
}

/**
 Called when the choose button for output directory is clicked
 Gets a path from the user and sets txt_outDir's contents accordingly
 @param event wxCommandEvent for the button (not used)
 */
void mainFrame::OnSave(wxCommandEvent& event){
	string message = "Choose the location to write the sound files.";
	string path = GetPathFromDialog(message);
	if (path != ""){
		txt_outDir->ChangeValue(wxString(path));
	}
}

/** Brings up a folder selection dialog with a prompt
 * @param message the prompt for the user
 * @return path selected, or an empty string if nothing chosen
 */
string mainFrame::GetPathFromDialog(string& message)
{
	//present the dialog
	wxDirDialog dlg(NULL, message, "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_CANCEL) {
		return "";
	}
	//get the path and return the standard string version
	wxString path = dlg.GetPath();
	return path.ToStdString();
}

/**
 * Sets platform-specific load and save path defaults
 */
void mainFrame::SetPlatformSpecificData(){
	//the paths which will be determined
	string homedir;
	string mcDir;
	string outDir;
#if defined __APPLE__ || defined __linux__
	struct passwd *pw = getpwuid(getuid());
	homedir = pw->pw_dir;
	outDir = "/Desktop";
#endif
#ifdef __APPLE__
	mcDir = "/Library/Application Support/minecraft";
#elif __linux__
	mcDir = "/.minecraft";
#elif _WIN32
	homedir = getenv("HOMEPATH");
	mcDir = "\\AppData\\Roaming\\.minecraft";
	outDir = "\\Desktop";
#endif
	txt_mcDir->ChangeValue(wxString(homedir + mcDir));
	txt_outDir->ChangeValue(wxString(homedir + outDir));
}

/**
 * Loads the minecraft version names into the combo box in the UI
 * @param path file path to the directory to load minecraft versions
 * @return true if successfully loaded versions, false if not
 */
bool mainFrame::LoadMcVersions(string path){

	vector<string> versions = extractor::LoadMcVersions(path);

	//if nothing returned, messagebox and return false
	if (versions.size() == 0) {
		wxMessageBox("No Minecraft versions found inside " + path + "\nSelect another folder.","No Minecraft versions found.", wxOK | wxICON_STOP);
		return false;
	}
	
	choice_mcVersion->Clear();

	//load into the popup
	for (string& version : versions) {
		choice_mcVersion->AppendString(version);
	}
	choice_mcVersion->SetSelection(0);
	return true;
}
