#include "explosion.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

#include "Tools/awesome_node_toolbox.h"
#include "Tools/console_logger.h"
#include "hero.h"
#include "rock.h"

using namespace godot;

void Explosion::_ready() {
    // This function is executed both in-game and in the Godot Editor,
    // hence the need for this condition to interrupt executions from the editor.
    if (Engine::get_singleton()->is_editor_hint())
        return;

    // Dynamically instantiating child nodes
    ant::load_packed_scene_as_new_child<Sprite2D>(this, EXPLOSION_SPRITE2D_PATH);
    m_ptrTimer = ant::add_new_child<Timer>(this);
    m_ptrArea2D = ant::load_packed_scene_as_new_child<Area2D>(this, EXPLOSION_COLLISIONSHAPE2D_PATH);

    m_ptrTimer->connect("timeout", Callable(this, "destroy"), Node::ConnectFlags::CONNECT_ONE_SHOT);
    m_ptrTimer->start(EXPLOSION_DURATION);

    m_ptrArea2D->set_physics_process(true);
    m_ptrArea2D->set_collision_mask(EXPLOSION_COLLISION_LAYER);
    m_ptrArea2D->set_monitoring(true);
    m_ptrArea2D->connect("body_entered", Callable(this, "hit_body"), Node::ConnectFlags::CONNECT_PERSIST);

    // Workaround for a bug that is currently present in the Godot engine
    // Area3D and Area2D don't detect static bodies if "Monitorable" is disabled (while it should)
    // https://github.com/godotengine/godot/issues/17238
    m_ptrArea2D->set_monitorable(true);
}

void Explosion::hit_body(Node2D* ptrBody) {
    Rock* ptrRock = Object::cast_to<Rock>(ptrBody);
    if (ptrRock) {
        ptrRock->destroy();
        return;
    }

    Hero* ptrHero = Object::cast_to<Hero>(ptrBody);
    if (ptrHero) {
        ptrHero->receive_hit();
        return;
    }
}

void Explosion::destroy() {
    Node* ptrParentNode = get_parent();
    if (ptrParentNode)
        ptrParentNode->remove_child(this);

    queue_free();
}

// ===== Register methods (explicitly exposing them to Godot) =====
void Explosion::_bind_methods() {
    ClassDB::bind_method(D_METHOD("hit_body", "ptrBody"), &Explosion::hit_body);
    ClassDB::bind_method(D_METHOD("destroy"), &Explosion::destroy);
}
