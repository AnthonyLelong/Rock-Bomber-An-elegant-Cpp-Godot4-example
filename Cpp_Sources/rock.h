#ifndef ROCK_H
#define ROCK_H

#include <godot_cpp/classes/static_body2d.hpp>

#define ROCK_COLLISION_LAYER 1

#define ROCK_SPRITE2D_PATH "res://Scenes/Rock/Rock_Sprite2D.tscn"
#define ROCK_COLLISIONSHAPE2D_PATH "res://Scenes/Rock/Rock_CollisionShape2D.tscn"

using namespace godot;

class Rock : public StaticBody2D {
    GDCLASS(Rock, StaticBody2D)

public:
    // Required by Godot
    static void _bind_methods();
    void _ready();

    // Explicitely declared/exposed to Godot 
    void destroy();
};

#endif // ROCK