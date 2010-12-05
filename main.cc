#include <iostream>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/timer.h>

#include <canvas.hh>
#include <world.hh>
 
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
    Chain* c1 = World::instance().create_chain();
    c1->add (100,100);
    c1->add (400,400);
    c1->add (600,100);

    Chain* c2 = World::instance().create_chain();
    c2->add (100,500);
    c2->add (400,500);
    c2->add (600,500);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello GL World"), wxPoint(50,50), wxSize(800,800));
	
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    
    canvas = new Canvas( (wxFrame*) frame, args);
    //canvas->world = world;

    sizer->Add(canvas, 1, wxEXPAND);
	
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
 
