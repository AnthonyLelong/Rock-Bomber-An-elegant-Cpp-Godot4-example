#include "main_rockbomber.h"

#include <vector>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>

#include "Tools/awesome_node_toolbox.h"
#include "Tools/console_logger.h"
#include "bomb.h"

using namespace godot;

void Main_RockBomber::_ready() {
	// This function is executed both in-game and in the Godot Editor,
	// hence the need for this condition to interrupt executions from the editor.
	if (Engine::get_singleton()->is_editor_hint())
		return;

	m_ptrHero = ant::add_new_child<Hero>(this);
	m_ptrCamera = ant::add_new_child<Camera2D>(this);
	m_ptrHud = ant::add_new_child<Hud>(this);

	m_ptrCamera->make_current();

	m_ptrHero->connect("bomb_placed", Callable(this, "spawn_bomb"), Node::ConnectFlags::CONNECT_PERSIST);
	m_ptrHero->connect("hit", Callable(this, "trigger_defeat"), Node::ConnectFlags::CONNECT_ONE_SHOT);
	m_ptrHud->connect("timeout", Callable(this, "trigger_defeat"), Node::ConnectFlags::CONNECT_ONE_SHOT);

	m_ptrHud->start_countdown(MAIN_TIME_LIMIT);

	instanciate_rocks_at_random_positions();
}

void Main_RockBomber::instanciate_rocks_at_random_positions() {
	const Size2 cameraViewportSize = get_viewport_rect().size * m_ptrCamera->get_zoom();
	const Vector2 cameraCenter = m_ptrCamera->get_screen_center_position();
	const int minX = cameraCenter.x - (cameraViewportSize.x / 2);
	const int maxX = cameraCenter.x + (cameraViewportSize.x / 2);
	const int minY = cameraCenter.y - (cameraViewportSize.y / 2);
	const int maxY = cameraCenter.y + (cameraViewportSize.y / 2);

	Ref<RandomNumberGenerator> randomNumberGenerator;
	randomNumberGenerator.instantiate();
	randomNumberGenerator->randomize();
	for (int i; i < MAIN_NB_ROCKS; i++) {
		Vector2 position;
		position.x = randomNumberGenerator->randi_range(minX, maxX);
		position.y = randomNumberGenerator->randi_range(minY, maxY);

		Rock* ptrRock = ant::add_new_child<Rock>(this);
		ptrRock->set_position(position);

		ptrRock->connect("tree_exited", Callable(this, "trigger_victory_if_relevant"), Node::ConnectFlags::CONNECT_PERSIST);
	}
}

void Main_RockBomber::spawn_bomb(Vector2 globalPosition) {
	Bomb* ptrBomb = ant::add_new_child<Bomb>(this);
	ptrBomb->set_global_position(globalPosition);
}

void Main_RockBomber::trigger_victory_if_relevant() {

	std::vector<Rock*> vectorRocks = ant::get_children<Rock>(this);
	ConsoleLogger::logger() << int(vectorRocks.size()) << " rock(s) remaining." << ConsoleLogger::endLine;
	if (!vectorRocks.empty())
		return;

	m_ptrHero->set_state(Hero::State::Waiting);
	m_ptrHud->display_victory_text();
}

void Main_RockBomber::trigger_defeat() {
	m_ptrHero->set_state(Hero::State::Hit);
	m_ptrHud->display_defeat_text();
}

// ===== Register methods (explicitly exposing them to Godot) =====
void Main_RockBomber::_bind_methods() {
	ClassDB::bind_method(D_METHOD("spawn_bomb", "globalPosition"), &Main_RockBomber::spawn_bomb);
	ClassDB::bind_method(D_METHOD("trigger_victory_if_relevant"), &Main_RockBomber::trigger_victory_if_relevant);
	ClassDB::bind_method(D_METHOD("trigger_defeat"), &Main_RockBomber::trigger_defeat);
}
