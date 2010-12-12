#include <iostream>
#include <boost/foreach.hpp>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <wx/thread.h>

#include <world.hh>
#include <panel.hh>
#include <canvas.hh>

 
using namespace std;

struct Op
{
    Node*   n1;
    Node*   n2;
    int     dir;
};

class WorkerThread : public wxThread
{
public:

    World&      world;
    wxSemaphore  ops_queue;
    Op          op;

    WorkerThread (World& w) : world(w) { }

    ExitCode Entry()
    {
        cout << "thread start!" << endl;

        while (0 == ops_queue.Wait()) {
            //Node* n1 = world.anchor (op.i1);
            //Node* n2 = world.anchor (op.i2);
            world.rotate (op.n1, op.n2, static_cast<float>(op.dir));

            while (world.evolve()) {
            }
        }
    }
};

    enum {
        BTN_CLOCK = wxID_HIGHEST + 1,
        BTN_COUNT,
        BTN_RESET
    };

class MainFrame : public wxFrame
{
public:

    World&          world;
    WorkerThread&   thread;
    Canvas*         canvas;

public:

    MainFrame (World& w, WorkerThread& t) :
        wxFrame (0, -1,  wxT("Heegaard van Gogh"), wxDefaultPosition, wxSize(900,700)),
        world (w), thread (t)
    {
        wxBoxSizer* sizer = new wxBoxSizer (wxVERTICAL);
        wxBoxSizer* hbox  = new wxBoxSizer (wxHORIZONTAL);
            
        int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
        
        CtrlPanel* pane = new CtrlPanel (World::instance(), this);
        canvas = new Canvas(this, args);

        wxButton* btn_clock = new wxButton (this, BTN_CLOCK, wxT("+"));
        wxButton* btn_count = new wxButton (this, BTN_COUNT, wxT("-"));
        wxButton* btn_reset = new wxButton (this, BTN_RESET, wxT("reset"));

        hbox->Add (btn_clock);
        hbox->Add (btn_count);
        hbox->Add (btn_reset);

        sizer->Add (pane,   0, wxEXPAND);
        sizer->Add (canvas, 3, wxEXPAND);
        sizer->Add (hbox,   0, wxEXPAND);
            
        SetSizer(sizer);
        SetAutoLayout(true);
    }

    void on_clock (wxCommandEvent& event)
    {
        vector<Node*> sel;
        BOOST_FOREACH (Node* a, world.anchors()) {
            if (a->selected)
                sel.push_back (a);
        }
        if (2 == sel.size()) {
            thread.op.n1  = sel [0];
            thread.op.n2  = sel [1];
            thread.op.dir = 1;
            thread.ops_queue.Post();
        }
    }

    void on_count (wxCommandEvent& event)
    {
        vector<Node*> sel;
        BOOST_FOREACH (Node* a, world.anchors()) {
            if (a->selected)
                sel.push_back (a);
        }
        if (2 == sel.size()) {
            thread.op.n1  = sel [0];
            thread.op.n2  = sel [1];
            thread.op.dir = -1;
            thread.ops_queue.Post();
        }
    }

    DECLARE_EVENT_TABLE();
};

BEGIN_EVENT_TABLE (MainFrame, wxFrame)
EVT_BUTTON (BTN_CLOCK, MainFrame::on_clock)
EVT_BUTTON (BTN_COUNT, MainFrame::on_count)
END_EVENT_TABLE()

class MyApp: public wxApp, public wxTimer
{
    virtual bool OnInit();
    
    MainFrame*      frame;
    WorkerThread*   thread;
    //World*   world;

public:
    
    /* override */ void Notify()
    {
        //float dt = ((float)GetInterval()) / 1000.0f;
        //World::instance().evolve (dt);
        //cout << "begin refresh" << endl;
        wxMutexLocker lock (World::instance().lock);
        frame->canvas->Refresh();
        //cout << "end refresh" << endl;
    }
};
 
IMPLEMENT_APP(MyApp)
 
bool MyApp::OnInit()
{
    World::instance().lock.Lock();

    Node* a1 = World::instance().create_anchor (150, 300);
    Node* a2 = World::instance().create_anchor (300, 300);
    Node* a3 = World::instance().create_anchor (450, 300);
    Node* a4 = World::instance().create_anchor (600, 300);

    a3->selected = true;

    Chain* c1 = World::instance().create_chain();
    c1->add (a2);
    c1->add (a3);

    thread = new WorkerThread (World::instance());
    thread->Create();

    //World::instance().rotate (a3, a4, 1.0f);
    frame = new MainFrame (World::instance(), *thread);
    frame->Show();

    thread->Run();

    thread->op.n1 = a3;
    thread->op.n2 = a4;
    thread->op.dir = -1;
    thread->ops_queue.Post();

    World::instance().lock.Unlock();

    Start (20);

    return true;
} 
 
