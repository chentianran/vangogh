#include <wx/event.h>

#include <world.hh>
#include <panel.hh>

CtrlPanel::CtrlPanel (World& w, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
    wxPanel (parent, id, pos, size),
    world(w)
{
    s_tension = new wxSlider (this, -1, w.tension_const, 0,  1000);
    s_repel   = new wxSlider (this, -1, w.coulomb_const, 10, 10000);

    wxFlexGridSizer* sizer = new wxFlexGridSizer (1, 2, 3, 3);

    sizer->Add (new wxStaticText (this, -1, wxT("Tension")));
    sizer->Add (s_tension, 1, wxEXPAND);

    sizer->Add (new wxStaticText (this, -1, wxT("Repelling")));
    sizer->Add (s_repel, 1, wxEXPAND);

    sizer->AddGrowableCol (1,1);

    SetSizer (sizer);

    s_tension->Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(CtrlPanel::on_scroll_tension));
}

CtrlPanel::~CtrlPanel()
{

}

void CtrlPanel::on_scroll_tension (wxScrollEvent&)
{

}
