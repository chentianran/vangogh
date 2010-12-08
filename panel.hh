#ifndef _PANEL_HH_
#define _PANEL_HH_

#include <vector>
#include <wx/wx.h>

class World;

class CtrlPanel : public wxPanel
{
protected:

    wxSlider* s_tension;
    wxSlider* s_repel;

    void on_scroll_tension (wxScrollEvent&);

public:

    World& world;

    CtrlPanel (World& w, wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    ~CtrlPanel();
};

#endif
