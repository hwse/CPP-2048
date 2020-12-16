#include <wx/wx.h>

class GameFrame : public wxFrame 
{
public:
    GameFrame(const wxString& title) 
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250,250))
    {
        Centre();
    }
};


class GameApp : public wxApp
{
    virtual bool OnInit() {
        GameFrame *frame = new GameFrame("2048");
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(GameApp);

