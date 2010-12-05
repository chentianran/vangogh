#include <boost/foreach.hpp>

#include <world.hh>

#include <iostream>

Vect operator - (const Vect& v1, const Vect& v2)
{
    return Vect (v1.x - v2.x, v1.y - v2.y);
}

Vect Vect::normalize() const
{
    float n = norm();
    return Vect (x/n, y/n);
}

void Node::evolve (float dt)
{
    bool moving = true;

    //if (velocity.is_zero()) {
    //    if (force.norm() > World::instance().friction_threshold)
    //        moving = true;
    //} else {
    //    force.move (velocity.normalize(), -World::instance().friction_coef * mass);
    //    moving = true;
    //}

    if (moving) {
        velocity.move (force, dt / mass);
        move (velocity, dt);
    }

    force.x = 0.0f;
    force.y = 0.0f;

    velocity.x /= 1.05;
    velocity.y /= 1.05;
}

Node* Chain::add (float x, float y)
{
    Node* n = World::instance().create_node (x, y);
    push_back (n);
    return n;
}

void Chain::apply_tension()
{
    iterator n1 = begin();                          // iterator for the 1st node
    iterator n2 = n1;                               // iterator for the 2nd node
    if (n1 != end()) {
        for (++n2; n2 != end(); ++n1, ++n2) {
            Vect f = (**n1) - (**n2);               // direction of the force
            (**n1).force.move (f, -coef);           // apply force on the first node
            (**n2).force.move (f,  coef);           // apply force on the second node

        }
    }
}

void World::evolve (float dt)
{
    BOOST_FOREACH (Chain* chain, _chains) {
        chain->apply_tension();
    }

    float C = World::instance().coulomb_const;

    BOOST_FOREACH (Node* n1, _nodes) {
        BOOST_FOREACH (Node* n2, _nodes) {
            if (n1 != n2) {
                Vect d = *n1 - *n2;
                float r = d.norm();         // distance
                float f = n1->charge * n2->charge * C / (r*r);
                n1->force.move (d,  f);
                n2->force.move (d, -f);
            }
        }
    }

    float wall_charge = 10.0;
    BOOST_FOREACH (Node* n, _nodes) {
        float q = n->charge * wall_charge * C;
        float l = n->x;
        float r = max_x - l;
        float t = n->y;
        float b = max_y - t;
        n->force.x += q/(l*l) - q/(r*r);
        n->force.y += q/(t*t) - q/(b*b);
    }

    BOOST_FOREACH (Node* node, _nodes) {
        node->evolve (dt);
    }
}

Node* World::create_node (float x, float y)
{
    Node* node = new Node (x, y);
    _nodes.push_back (node);
    return node;
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
