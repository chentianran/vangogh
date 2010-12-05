#ifndef _CANVAS_HH_
#define _CANVAS_HH_
 
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <vector>

class World;
 
class Canvas : public wxGLCanvas
{
    wxGLContext*	m_context;
 
protected:

    void prepareViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

public:

    //World* world;

    Canvas(wxFrame* parent, int* args);
    virtual ~Canvas();

    void resized(wxSizeEvent& evt);

    int getWidth()  { return GetSize().x; }
    int getHeight() { return GetSize().y; }

    void render(wxPaintEvent& evt);

    // events
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);

    DECLARE_EVENT_TABLE()
};
#endif 
