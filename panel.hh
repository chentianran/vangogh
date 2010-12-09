#ifndef _PANEL_HH_
#define _PANEL_HH_

#include <vector>
#include <wx/wx.h>

class World;

class CtrlSlider : public wxSlider
{
protected:

    float&  _val;
    float   _min;
    float   _max;

    void on_scroll (wxScrollEvent&);

public:

    CtrlSlider (float& v, float min, float max, wxWindow* parent);
};

    
class CtrlPanel : public wxPanel
{
public:

    World& world;

    CtrlPanel (World& w, wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
};

#endif
