#include <iostream>

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <wx/thread.h>
//#include <wx/msgqueue.h>

#include <world.hh>
#include <panel.hh>
#include <canvas.hh>

 
using namespace std;

struct Op
{
    int i1;
    int i2;
    int dir;
};

//typedef wxMessageQueue<Op> OpQueue;

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
            Node* n1 = world.anchor (op.i1);
            Node* n2 = world.anchor (op.i2);
            world.rotate (n1, n2, static_cast<float>(op.dir));

            while (world.evolve()) {
            }
        }
    }
};

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

    //World::instance().rotate (a3, a4, 1.0f);
    frame = new MainFrame (World::instance());
    frame->Show();

    thread = new WorkerThread (World::instance());
    thread->Create();
    thread->Run();

    thread->op.i1 = 2;
    thread->op.i2 = 3;
    thread->op.dir = -1;
    thread->ops_queue.Post();

    World::instance().lock.Unlock();

    Start (20);

    return true;
} 
 
