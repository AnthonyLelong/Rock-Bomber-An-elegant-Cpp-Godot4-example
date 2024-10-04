#include "bomb.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "Tools/awesome_node_toolbox.h"
#include "explosion.h"

using namespace godot;

void Bomb::_ready() {
    // This function is executed both in-game and in the Godot Editor,
    // hence the need for this condition to interrupt executions from the editor.
    if (Engine::get_singleton()->is_editor_hint())
        return;

    // Dynamically instantiating child nodes
    m_ptrSprite2D = ant::load_packed_scene_as_new_child<Sprite2D>(this, BOMB_SPRITE2D_PATH);
    m_ptrTimer = ant::add_new_child<Timer>(this);

    m_ptrTimer->connect("timeout", Callable(this, "explode"), Node::ConnectFlags::CONNECT_ONE_SHOT);
    m_ptrTimer->start(BOMB_TIME_BEFORE_EXPLOSION);
}

void Bomb::explode() {
    Vector2 globalPosition = get_global_position();
    remove_child(m_ptrSprite2D);
    m_ptrSprite2D->queue_free();

    Explosion* ptrExplosion = memnew(Explosion);
    replace_by(ptrExplosion);
    ptrExplosion->set_global_position(globalPosition);

    queue_free();
}

// ===== Register methods (explicitly exposing them to Godot) =====
void Bomb::_bind_methods() {
    ClassDB::bind_method(D_METHOD("explode"), &Bomb::explode);
}
