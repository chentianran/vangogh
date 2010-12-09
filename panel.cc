#include <wx/event.h>

#include <world.hh>
#include <panel.hh>

#include <iostream>

CtrlSlider::CtrlSlider (float& v, float min, float max, wxWindow* parent) :
    wxSlider (parent, -1, (int)(1000.0 * (v-min) / (max-min)), 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_LABELS | wxSL_HORIZONTAL),
    _val (v), _min (min), _max (max)
{
    Connect (wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(CtrlSlider::on_scroll));
}

void CtrlSlider::on_scroll (wxScrollEvent& ev)
{
    _val = _min + ((float)ev.GetPosition()) * (_max - _min) / 1000.0 ;
}
    
CtrlPanel::CtrlPanel (World& w, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
    wxPanel (parent, id, pos, size),
    world(w)
{
    wxSizer* sizer = new wxGridSizer (5,2,0);

    sizer->Add (new CtrlSlider (w.tension_const, 0.0,  200.0,  this), 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    sizer->Add (new CtrlSlider (w.coulomb_const, 10.0, 5000.0, this), 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    sizer->Add (new CtrlSlider (w.force_range,   0.0,  100.0,  this), 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    sizer->Add (new CtrlSlider (w.min_link,      5.0,  15.0,   this), 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    sizer->Add (new CtrlSlider (w.max_link,      10.0, 30.0,   this), 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    sizer->Add (new wxStaticText (this, -1, wxT("Tension")),     0, wxALIGN_CENTER_HORIZONTAL);
    sizer->Add (new wxStaticText (this, -1, wxT("Repelling")),   0, wxALIGN_CENTER_HORIZONTAL);
    sizer->Add (new wxStaticText (this, -1, wxT("Force range")), 0, wxALIGN_CENTER_HORIZONTAL);
    sizer->Add (new wxStaticText (this, -1, wxT("Min. link")),   0, wxALIGN_CENTER_HORIZONTAL);
    sizer->Add (new wxStaticText (this, -1, wxT("Max. link")),   0, wxALIGN_CENTER_HORIZONTAL);

    SetSizer (sizer);
}

