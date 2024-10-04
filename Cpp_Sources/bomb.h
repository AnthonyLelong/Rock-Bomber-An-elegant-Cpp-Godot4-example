#ifndef BOMB_H
#define BOMB_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/sprite2d.hpp>

#define BOMB_TIME_BEFORE_EXPLOSION 1.5

#define BOMB_SPRITE2D_PATH "res://Scenes/Bomb/Bomb_Sprite2D.tscn"

using namespace godot;

class Bomb : public Node2D {
    GDCLASS(Bomb, Node2D)

public:
    // Required by Godot
    static void _bind_methods();
    void _ready();

private:
    Timer* m_ptrTimer;
    Sprite2D* m_ptrSprite2D;

    // Explicitely declared/exposed to Godot 
    void explode();
};

#endif // BOMB