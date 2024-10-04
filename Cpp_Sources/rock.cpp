#include "rock.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

#include "Tools/awesome_node_toolbox.h"

using namespace godot;

void Rock::_ready() {
    // This function is executed both in-game and in the Godot Editor,
    // hence the need for this condition to interrupt executions from the editor.
    if (Engine::get_singleton()->is_editor_hint())
        return;

    // Dynamically instantiating child nodes
    ant::load_packed_scene_as_new_child<Sprite2D>(this, ROCK_SPRITE2D_PATH);
    ant::load_packed_scene_as_new_child<CollisionShape2D>(this, ROCK_COLLISIONSHAPE2D_PATH);

    set_collision_layer(ROCK_COLLISION_LAYER);
}

void Rock::destroy() {
    Node* ptrParentNode = get_parent();
    if (ptrParentNode)
        ptrParentNode->remove_child(this);

    queue_free();
}

// ===== Register methods (explicitly exposing them to Godot) =====
void Rock::_bind_methods() {
    ClassDB::bind_method(D_METHOD("destroy"), &Rock::destroy);
}
