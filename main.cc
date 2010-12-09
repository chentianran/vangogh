#include <iostream>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/timer.h>

#include <world.hh>
#include <panel.hh>
#include <canvas.hh>

#include <iostream>
 
class MainFrame : public wxFrame
{
public:

    World&  world;
    Canvas* canvas;

public:

    MainFrame (World& w) :
        wxFrame (0, -1,  wxT("Heegaard van Gogh"), wxDefaultPosition, wxSize(900,700)),
        world (w)
    {
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            
        int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
        
        CtrlPanel* pane = new CtrlPanel (World::instance(), this);
        canvas = new Canvas(this, args);

        sizer->Add (pane,   0, wxEXPAND);
        sizer->Add (canvas, 3, wxEXPAND);
            
        SetSizer(sizer);
        SetAutoLayout(true);
    }
};

class MyApp: public wxApp, public wxTimer
{
    virtual bool OnInit();
    
    MainFrame*  frame;
    //World*   world;

public:
    
    /* override */ void Notify()
    {
        float dt = ((float)GetInterval()) / 1000.0f;
        World::instance().evolve (dt);
        frame->canvas->Refresh();
    }
};
 
IMPLEMENT_APP(MyApp)
 
bool MyApp::OnInit()
{
    Node* a1 = World::instance().create_anchor (150, 300);
    Node* a2 = World::instance().create_anchor (300, 300);
    Node* a3 = World::instance().create_anchor (450, 300);
    Node* a4 = World::instance().create_anchor (600, 300);

    Chain* c1 = World::instance().create_chain();
    c1->add (a2);
    c1->add (a3);

    World::instance().rotate (a3, a4, 1.0f);

    frame = new MainFrame (World::instance());
    frame->Show();

    Start (20);

    return true;
} 
 
