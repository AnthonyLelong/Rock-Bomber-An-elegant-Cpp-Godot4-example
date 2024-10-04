#ifndef HERO_H
#define HERO_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>

#define HERO_MOVEMENT_SPEED 200
#define HERO_COLLISION_LAYER 1

#define HERO_ANIMATEDSPRITE2D_PATH "res://Scenes/Hero/Hero_AnimatedSprite2D.tscn"
#define HERO_COLLISIONSHAPE2D_PATH "res://Scenes/Hero/Hero_CollisionShape2D.tscn"

using namespace godot;

class Hero : public CharacterBody2D {
	GDCLASS(Hero, CharacterBody2D)

public:
	// ===== Required by Godot ======
	static void _bind_methods();
	void _ready();
	void _physics_process(float delta);

private:
	// Pointers to child nodes
	Input* m_ptrInputs;
	AnimatedSprite2D* m_ptrAnimatedSprite;

public:
	enum class State
	{
		Idle,
		Walking,
		Hit,
		Waiting
	};

	void set_state(const State& state);

private:
	State m_state;

	void process_bomb_placing();
	void process_move();

public:
	// Explicitely declared/exposed to Godot
	void update_sprite();
	void receive_hit();
};


#endif