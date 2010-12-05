#ifndef _WORLD_HH_
#define _WORLD_HH_

#include <list>
#include <cmath>

using std::list;

struct Vect
{
    float x;
    float y;

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

class Node : public Vect
{
public:

    Vect velocity;
    Vect force;
    float mass;
    float charge;

    Node (float x, float y, float m = 1.0f, float c = 1.0f) :
        Vect(x,y), velocity(0.0f,0.0f), force(0.0f,0.0f), mass(m), charge(c) { }

    void evolve (float dt);
};

class Chain : public list<Node*>
{
public:

    static float max_link;
    static float min_link;

public:
    
    float coef;             // Hooke's coefficient

    Chain() : coef(30.0) { };

    Node* add (float x, float y);
    void apply_tension();
};

class World
{
private: 

    World() : friction_threshold(1.0), friction_coef (4.0), coulomb_const(1e3f) {}
    ~World() {} 
    World(const World&);                // intentionally undefined
    World& operator=(const World&);     // intentionally undefined
 
 
public:

    static World& instance();

protected:

    list<Node*>  _nodes;
    list<Chain*> _chains;

public:

    float max_x;
    float max_y;

    float friction_threshold;
    float friction_coef;
    float coulomb_const;        // Coulomb's constant

    void evolve (float dt);
    const list<Node*>&  nodes()  const { return _nodes; }
    const list<Chain*>& chains() const { return _chains; }

    Node* create_node (float x, float y);
    Chain* create_chain();
};

#endif
