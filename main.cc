#include <iostream>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/timer.h>

#include <world.hh>
#include <panel.hh>
#include <canvas.hh>
 
class MyApp: public wxApp, public wxTimer
{
    virtual bool OnInit();
    
    wxFrame *frame;
    Canvas * canvas;
    //World*   world;

public:
    
    /* override */ void Notify()
    {
        float dt = ((float)GetInterval()) / 1000.0f;
        World::instance().evolve (dt);
        canvas->Refresh();
    }
};
 
IMPLEMENT_APP(MyApp)
 
bool MyApp::OnInit()
{
    Node* a1 = World::instance().create_anchor (200, 200);
    Node* a2 = World::instance().create_anchor (400, 200);
    Node* a3 = World::instance().create_anchor (100, 300);
    Node* a4 = World::instance().create_anchor (300, 300);

    Chain* c1 = World::instance().create_chain();
    c1->add (a1);
    c1->add (a2);

    Chain* c2 = World::instance().create_chain();
    c2->add (a3);
    c2->add (a4);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello GL World"), wxPoint(50,50), wxSize(800,800));
	
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    
    //wxPanel* pane = new wxPanel (frame, -1, wxPoint(0,0), wxSize(200,800));
    CtrlPanel* pane = new CtrlPanel (World::instance(), frame);
    canvas = new Canvas( (wxFrame*) frame, args);
    //canvas->world = world;

    sizer->Add (pane, 1);
    sizer->Add (canvas, 3, wxEXPAND);
	
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
    frame->Show();

    Start (20);

    return true;
} 
 
BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
EVT_MOTION(Canvas::mouseMoved)
EVT_LEFT_DOWN(Canvas::mouseDown)
EVT_LEFT_UP(Canvas::mouseReleased)
EVT_RIGHT_DOWN(Canvas::rightClick)
EVT_LEAVE_WINDOW(Canvas::mouseLeftWindow)
EVT_SIZE(Canvas::resized)
EVT_KEY_DOWN(Canvas::keyPressed)
EVT_KEY_UP(Canvas::keyReleased)
EVT_MOUSEWHEEL(Canvas::mouseWheelMoved)
EVT_PAINT(Canvas::render)
END_EVENT_TABLE()
 
