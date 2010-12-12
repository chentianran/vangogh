#include <GL/glu.h>
#include <GL/gl.h>
#include <boost/foreach.hpp>

#include <canvas.hh>
#include <world.hh>

void Canvas::mouseMoved(wxMouseEvent& event) {}
void Canvas::mouseDown(wxMouseEvent& event)
{
    Node* a = World::instance().find_anchor (event.GetX(), event.GetY(), 10.0f);
    if (a)
        a->selected = !a->selected;
}

void Canvas::mouseWheelMoved(wxMouseEvent& event) {}
void Canvas::mouseReleased(wxMouseEvent& event) {}
void Canvas::rightClick(wxMouseEvent& event) {}
void Canvas::mouseLeftWindow(wxMouseEvent& event) {}
void Canvas::keyPressed(wxKeyEvent& event) {}
void Canvas::keyReleased(wxKeyEvent& event) {}
 
Canvas::Canvas(wxFrame* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY,  0, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
}
 
Canvas::~Canvas()
{
    delete m_context;
}
 
void Canvas::resized(wxSizeEvent& evt)
{
//	wxGLCanvas::OnSize(evt);
	
    World::instance().max_x = evt.GetSize().GetWidth();
    World::instance().max_y = evt.GetSize().GetHeight();
    Refresh();
}
 
void Canvas::prepareViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
 
void Canvas::render (wxPaintEvent& evt)
{
    if (! IsShown())
        return;
    
    wxGLCanvas::SetCurrent (*m_context);
    wxPaintDC (this); // only to be used in paint events. use wxClientDC to paint outside the paint event
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    prepareViewport(0,0,getWidth(), getHeight());
    glLoadIdentity();
	
    glColor4f (0.9, 0.3, 0, 1);
    BOOST_FOREACH (Chain* chain, World::instance().chains()) {
        glBegin (GL_LINE_STRIP);
        BOOST_FOREACH (Node* node, *chain) {
            glVertex3f (node->x, node->y, 0);
        }
        glEnd();
    }
    
    const float sz = 4;
    BOOST_FOREACH (Node* a, World::instance().anchors()) {
        glBegin (GL_QUADS);
        glVertex3f (a->x-sz, a->y-sz, 0);
        glVertex3f (a->x+sz, a->y-sz, 0);
        glVertex3f (a->x+sz, a->y+sz, 0);
        glVertex3f (a->x-sz, a->y+sz, 0);
        glEnd();

        if (a->selected) {
            const float sz2 = 8;
            glBegin (GL_LINE_LOOP);
            glVertex3f (a->x-sz2, a->y-sz2, 0);
            glVertex3f (a->x+sz2, a->y-sz2, 0);
            glVertex3f (a->x+sz2, a->y+sz2, 0);
            glVertex3f (a->x-sz2, a->y+sz2, 0);
            glEnd();
        }
    }

    glFlush();
    SwapBuffers();
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
 
