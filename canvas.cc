#include <GL/glu.h>
#include <GL/gl.h>
#include <boost/foreach.hpp>

#include <canvas.hh>
#include <world.hh>

void Canvas::mouseMoved(wxMouseEvent& event)
{
    if (event.Dragging()) {
        Node* n = World::instance().anchors().front();
        //Vect f (event.GetX(), event.GetY());
        //f -= *n;
        //n->force.move (f, 20.0);
        n->x = event.GetX();
        n->y = event.GetY();
    }
}

void Canvas::mouseDown(wxMouseEvent& event)
{
    //Node* n = World::instance().nodes().front();
    //n->x = event.GetX();
    //n->y = event.GetY();
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
    }
    glEnd();

    glFlush();
    SwapBuffers();
}
