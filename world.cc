#include <cmath>
#include <boost/foreach.hpp>

#include <world.hh>

#include <iostream>

using std::max;
using std::min;

Vect operator - (const Vect& v1, const Vect& v2)
{
    return Vect (v1.x - v2.x, v1.y - v2.y);
}

Vect Vect::normalize() const
{
    float n = norm();
    return Vect (x/n, y/n);
}

void Node::evolve (float dt, const World& world)
{
    bool moving = true;

    if (moving) {
        //move (force, dt / (mass*3e1));
        velocity.move (force, dt / mass);
        move (velocity, dt);
        world.clip (*this);
    }

    force.x = 0.0f;
    force.y = 0.0f;

    velocity.x /= 1.09;
    velocity.y /= 1.09;
}

//float Chain::max_link = 20.0;
//float Chain::min_link = 8.0;

Node* Chain::add (float x, float y)
{
    Node* n = World::instance().create_node (x, y);
    push_back (n);
    return n;
}

void Chain::apply_tension (World& world)
{
    const float coef = world.tension_const;             // Hooke's tension constant
    iterator n1 = begin();                              // iterator for the 1st node
    iterator n2 = n1;                                   // iterator for the 2nd node
    if (n1 != end()) {
        for (++n2; n2 != end(); ++n1, ++n2) {
            for ( ; ; ) {
                Vect f = (**n1) - (**n2);                   // direction of the force
                float d = f.norm();
                if (d > world.max_link) {                  // if the two nodes are too far apart
                    float x3 = ((**n1).x+(**n2).x)/2.0;
                    float y3 = ((**n1).y+(**n2).y)/2.0;
                    Node* n3 = world.create_node (x3, y3);  // create a new node
                    insert (n2, n3);                        // add the new node in between the two
                    ++n1;                                   // update iterator
                    break;
                } else if (d < world.min_link) {
                    iterator old_n2 = n2;
                    ++n2;
                    if (end() != n2) {                     // if old n2 is not the last node
                        (**old_n2).removed = true;
                        erase (old_n2);
                        continue;
                    } else {
                        n2 = old_n2;
                        break;
                    }
                } else {                                    // not too far, apply tension
                    (**n1).force.move (f, -coef);           // apply force on the first node
                    (**n2).force.move (f,  coef);           // apply force on the second node
                    break;
                }
            }
        }
    }
}

World::World()
{
    min_link =  8.0;
    max_link = 20.0;

    float r = (min_link + max_link) / 2.0;
    tension_const = 50.0f;
    coulomb_const = 5e3;
    force_range = 30;
}

void World::evolve (float dt)
{
    BOOST_FOREACH (Chain* chain, _chains) {
        chain->apply_tension (*this);
    }

    float C = World::instance().coulomb_const;

#if 0
    BOOST_FOREACH (Chain* c, _chains) {
        BOOST_FOREACH (Node* n, *c) {               // for each node on the chain
            float nc = n->charge;
            BOOST_FOREACH (Node* a, _anchors) {             // apply anchor-to-chain force
                if (n != a) {
                    Vect d = *n - *a;
                    float r = d.norm();                 // distance
                    float f = force (nc, a->charge, r);
                    //if (r < 100.0) {
                        //float f = nc * a->charge * C / (r*r);
                        n->force.move (d,  f);
                        a->force.move (d, -f);
                    //}
                }
            }
            BOOST_FOREACH (Chain* c2, _chains) {            // apply chain-to-chain force
                if (c != c2) {
                    BOOST_FOREACH (Node* n2, *c2) {
                        Vect d = *n - *n2;
                        float r = d.norm();         // distance
                        float f = force (nc, n2->charge, r);
                        //if (r < 100.0) {
                        //    float f = nc * n2->charge * C / (r*r);
                            n ->force.move (d,  f);
                            n2->force.move (d, -f);
                        //}
                    }
                }
            }
        }
    }
#endif

    BOOST_FOREACH (Node* n1, _nodes) {
        BOOST_FOREACH (Node* n2, _nodes) {
            if (n1 != n2) {
                Vect d = *n1 - *n2;
                float r = d.norm();         // distance
                float f = force (n1->charge, n2->charge, r);
                n1->force.move (d,  f);
                n2->force.move (d, -f);
            }
        }
    }

    float wall_charge = 10.0;
    BOOST_FOREACH (Node* n, _nodes) {
        float q = n->charge * wall_charge * C;
        float l = max(0.5f,n->x);
        float r = max(0.5f,max_x - l);
        float t = max(0.5f,n->y);
        float b = max(0.5f,max_y - t);
        n->force.x += q/(l*l) - q/(r*r);
        n->force.y += q/(t*t) - q/(b*b);
    }

    list<Node*>::iterator ni = _nodes.begin();
    while (_nodes.end() != ni) {
        if ((**ni).removed) {
            list<Node*>::iterator old = ni;
            ++ni;
            delete *old;
            _nodes.erase (old);
        } else {
            (**ni).evolve (dt, *this);
            ++ni;
        }
    }
}

Node* World::create_node (float x, float y)
{
    Node* node = new Node (x, y);
    _nodes.push_back (node);
    return node;
}

Node* World::create_anchor (float x, float y)
{
    Node* a = new Node (x, y, INFINITY, 10.0f);
    _nodes.push_back (a);
    _anchors.push_back (a);
    return a;
}

Chain* World::create_chain()
{
    Chain* chain = new Chain;
    _chains.push_back (chain);
    return chain;
}

World& World::instance()
{
    static World world;
    return world;
}

void World::clip (Vect& v) const
{
    v.x = max (v.x, .0f);
    v.y = max (v.y, .0f);
    v.x = min (v.x, max_x);
    v.y = min (v.y, max_y);
}

float World::force (float c1, float c2, float r) const
{
    r = max(r, 0.001f);
    if (r < force_range)
        return c1 * c2 * coulomb_const / (r*r);
    else
        return 0.0f;
}
