#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/area2d.hpp>

#define EXPLOSION_DURATION 0.5
#define EXPLOSION_COLLISION_LAYER 1

#define EXPLOSION_SPRITE2D_PATH "res://Scenes/Explosion/Explosion_Sprite2D.tscn"
#define EXPLOSION_COLLISIONSHAPE2D_PATH "res://Scenes/Explosion/Explosion_Area2D.tscn"

using namespace godot;

class Explosion : public Node2D {
    GDCLASS(Explosion, Node2D)

public:
    // Required by Godot
    static void _bind_methods();
    void _ready();

private:
    Timer* m_ptrTimer;
    Area2D* m_ptrArea2D;

public:
    // Explicitely declared/exposed to Godot
    void hit_body(Node2D* body);
    void destroy();
};

#endif // EXPLOSION