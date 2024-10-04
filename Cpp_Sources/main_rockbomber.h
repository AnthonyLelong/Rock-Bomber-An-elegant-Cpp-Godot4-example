#ifndef MAIN_H
#define MAIN_H

#include <vector>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/camera2d.hpp>

#include "hero.h"
#include "rock.h"
#include "hud.h"

#define MAIN_TIME_LIMIT 60
#define MAIN_NB_ROCKS 8

#define MAIN_PATH_INITIAL_WORLD_NODE "res://Scenes/World/InitialWorld.tscn"
#define MAIN_PATH_WATER_HOLE_NODE "res://Scenes/Other/WaterHole.tscn"

using namespace godot;

class Main_RockBomber : public Node2D { // Exceptionnaly using "_" in class name for visibility purpose
	GDCLASS(Main_RockBomber, Node2D)

public:
	// Required by Godot ======
	static void _bind_methods();
	void _ready();

private:
	// Pointers
	Hero* m_ptrHero;
	Camera2D* m_ptrCamera;
	Hud* m_ptrHud;

	void instanciate_rocks_at_random_positions();

public:
	// Registered/Exposed to Godot
	void spawn_bomb(Vector2 globalPosition);
	void trigger_victory_if_relevant();
	void trigger_defeat();
};

#endif
