#ifndef _WORLD_HH_
#define _WORLD_HH_

#include <list>
#include <vector>
#include <cmath>

#include <wx/thread.h>

using std::list;
using std::vector;

struct Vect
{
    float x;
    float y;

    Vect () : x(0.0f), y(0.0f) { }
    Vect (float _x, float _y) : x(_x), y(_y) { }

    Vect& operator += (const Vect& v) { x += v.x; y += v.y; }
    Vect& operator -= (const Vect& v) { x -= v.x; y -= v.y; }

    float norm() const { return std::sqrt (x*x + y*y); }

    bool is_zero() const
    {
        return (std::fabs(x) < 1e-7) && (std::fabs(y) < 1e-7);
    }

    void move (const Vect& v, float d)
    {
        x += (v.x * d);
        y += (v.y * d);
    }

    Vect normalize() const;
};

Vect operator - (const Vect& v1, const Vect& v2);
Vect operator + (const Vect& v1, const Vect& v2);

class World;

class Node : public Vect
{
public:

    bool removed;
    bool selected;
    Vect force;
    float mass;
    float charge;

    Node (float x, float y, float m = 100.0f, float c = 1.0f) :
        Vect(x,y), removed (false), selected (false), force(0.0f,0.0f), mass(m), charge(c) { }

    void evolve (const World& world);
};

class Chain : public list<Node*>
{
public:
    
    Node* add (float x, float y);
    void  add (Node* node) { push_back (node); }
    void apply_tension (World& world);
};

class World
{
private: 

    World();
    ~World() {} 
    World(const World&);                // intentionally undefined
    World& operator=(const World&);     // intentionally undefined
 
public:

    static World& instance();

protected:

    list<Node*>     _nodes;
    vector<Node*>   _anchors;
    list<Chain*>    _chains;

    Vect            _axis;
    float           _dir;
    float           _phase1;
    float           _phase2;
    float           _phase;
    Node*           _node1;
    Node*           _node2;

public:

    float min_link;
    float max_link;

    float coulomb_const;        // Coulomb's constant
    float tension_const;        // Hooke's constant for chains
    float force_range;
    float step;

    float max_x;
    float max_y;

    wxMutex lock;

    bool evolve();
    const list<Node*>&  nodes()   const { return _nodes; }
    const vector<Node*>&  anchors() const { return _anchors; }
    const list<Chain*>& chains()  const { return _chains; }

    Node* create_node (float x, float y);
    void rotate (Node* n1, Node* n2, float d);
    Node* create_anchor (float x, float y);
    Node* find_anchor (float x, float y, float range);
    Node* anchor (unsigned int i) const { return _anchors[i]; }
    Chain* create_chain();

    void clip (Vect& v) const;
    float force (float c1, float c2, float r) const;
};

#endif
