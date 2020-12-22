#include <wx/wx.h>
#include <math.h>

#include "game.h"

class GameFrame : public wxFrame 
{
private:
    wxButton *cells[ROW_COUNT][COLUMN_COUNT];
    GameState gameState; 
public:
    GameFrame(const wxString& title) 
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250,250)), gameState()
    {
        Centre();
        wxGridSizer *grid = new wxGridSizer(COLUMN_COUNT, ROW_COUNT, 0, 0);
        Bind(wxEVT_CHAR_HOOK, &GameFrame::OnKeyUp, this);

        for (size_t row = 0; row < ROW_COUNT; row++) {
            for (size_t column = 0; column < COLUMN_COUNT; column++) {
                wxButton *button = new wxButton(this, 10000 + (row * ROW_COUNT + column), "0");
                cells[row][column] = button;
                grid->Add(button, 1, wxEXPAND | wxALL);
            }
        }
        this->SetSizer(grid);

        gameState.spawn();
        gameState.spawn();

        updateButtons();
    }

    void updateButtons() {
        for (int row = 0; row < ROW_COUNT; row++) {
            for (int column = 0; column < COLUMN_COUNT; column++) {
                Cell cell = gameState.get({row, column});
                wxString label;
                label << cell;
                wxButton *button = cells[row][column]; 
                button->SetLabel(label);


                unsigned char c = static_cast<char>(log2(cell));
                unsigned char yellow = c > 255 ? 0 : 255 - 20 * c;
                wxColour color = cell ? wxColour{255, yellow, 0} : *wxWHITE;
                button->SetBackgroundColour(color);
            }
        }
    }

    void OnKeyUp(wxKeyEvent& event) {

        Direction direction;
        switch (event.GetKeyCode()) {
            case WXK_LEFT:
                direction = Direction::LEFT; break;
            case WXK_UP:
                direction = Direction::UP; break;
            case WXK_RIGHT: 	
                direction = Direction::RIGHT; break;
            case WXK_DOWN:
                direction = Direction::DOWN; break; 
            default: 
                return;
        }
        bool changed = gameState.shift(direction);
        if (changed) {
            gameState.spawn();
        }
        updateButtons();
        event.Skip();
    }

};

class GameApp : public wxApp
{
    virtual bool OnInit() {
        GameFrame *frame = new GameFrame("2048");
        frame->Connect(wxEVT_KEY_UP, wxKeyEventHandler(GameFrame::OnKeyUp), NULL, this);
        
        SetTopWindow(frame);
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(GameApp);

