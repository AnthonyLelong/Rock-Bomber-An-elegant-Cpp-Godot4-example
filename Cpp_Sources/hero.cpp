#include "hero.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

#include "Tools/awesome_node_toolbox.h"
#include "bomb.h"

using namespace godot;

void Hero::_ready() {
	// This function is executed both in-game and in the Godot Editor,
	// hence the need for this condition to interrupt executions from the editor.
	if (Engine::get_singleton()->is_editor_hint())
		return;

	m_ptrInputs = Input::get_singleton();

	// Dynamically loading packed scene created in Godot Editor as child nodes
	ant::load_packed_scene_as_new_child<CollisionShape2D>(this, HERO_COLLISIONSHAPE2D_PATH); // No need to keep pointer for this one
	m_ptrAnimatedSprite = ant::load_packed_scene_as_new_child<AnimatedSprite2D>(this, HERO_ANIMATEDSPRITE2D_PATH);

	set_state(State::Idle);
	update_sprite();
	set_collision_layer(HERO_COLLISION_LAYER);
	m_ptrAnimatedSprite->play();

	// Floating Motion Mode is suitable for top-down games (no notion of floor or ceiling)
	set_motion_mode(CharacterBody2D::MOTION_MODE_FLOATING);

	connect("state_changed", Callable(this, "update_sprite"), Node::ConnectFlags::CONNECT_PERSIST);
}

void Hero::_physics_process(float delta) {
	// This function is executed both in-game and in the Godot Editor,
	// hence the need for this condition to interrupt executions from the editor.
	if (Engine::get_singleton()->is_editor_hint())
		return;

	if ((m_state == State::Hit) || (m_state == State::Waiting))
		return;

	process_bomb_placing();
	process_move();
}

void Hero::process_bomb_placing() {
	const bool spaceInput = m_ptrInputs->is_action_just_pressed("ui_accept");

	if (spaceInput)
		emit_signal("bomb_placed", get_global_position());
}

void Hero::process_move() {

	int leftRightInputs = 1 * m_ptrInputs->is_action_pressed("ui_right") - 1 * m_ptrInputs->is_action_pressed("ui_left");
	int upDownInputs = -1 * m_ptrInputs->is_action_pressed("ui_up") + 1 * m_ptrInputs->is_action_pressed("ui_down");

	if ((leftRightInputs == 0) && (upDownInputs == 0)) {
		set_state(State::Idle);
		return;
	}

	if (m_state == State::Idle)
		set_state(State::Walking);

	Vector2 newDirection(leftRightInputs, upDownInputs);
	Vector2 newVelocity = HERO_MOVEMENT_SPEED * newDirection;
	set_velocity(newVelocity);

	move_and_slide(); // This built-in Godot function uses previously set velocity
}

void Hero::set_state(const State& state) {
	if (m_state == state)
		return;

	m_state = state;
	emit_signal("state_changed");
}

void Hero::update_sprite() {
	switch (m_state) {
	case State::Idle:
	case State::Waiting:
		m_ptrAnimatedSprite->set_animation("Idle");
		break;
	case State::Walking:
		m_ptrAnimatedSprite->set_animation("Walking");
		break;
	case State::Hit:
		m_ptrAnimatedSprite->set_animation("Hit");
		break;
	}
}

void Hero::receive_hit() {
	set_state(State::Hit);
	emit_signal("hit");
}

// ===== Register methods (explicitly exposing them to Godot) =====
void Hero::_bind_methods() {
	ADD_SIGNAL(MethodInfo("state_changed"));
	ADD_SIGNAL(MethodInfo("hit"));
	ADD_SIGNAL(MethodInfo("bomb_placed", PropertyInfo(Variant::VECTOR2, "globalPosition")));
	ClassDB::bind_method(D_METHOD("update_sprite"), &Hero::update_sprite);
	ClassDB::bind_method(D_METHOD("receive_hit"), &Hero::receive_hit);
}
