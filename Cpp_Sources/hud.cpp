#include "hud.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>

#include "Tools/awesome_node_toolbox.h"

using namespace godot;

void Hud::_ready() {
    // This function is executed both in-game and in the Godot Editor,
    // hence the need for this condition to interrupt executions from the editor.
    if (Engine::get_singleton()->is_editor_hint())
        return;

    // Dynamically instantiating child nodes
    m_ptrCountdownLabel = ant::add_new_child<Label>(this);
    m_ptrCenterLabel = ant::add_new_child<Label>(this);
    m_ptrTimer = ant::add_new_child<Timer>(this);

    m_ptrCountdownLabel->set_horizontal_alignment(godot::HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    m_ptrCountdownLabel->set_vertical_alignment(godot::VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    m_ptrCountdownLabel->set_anchors_preset(Control::LayoutPreset::PRESET_CENTER);
    m_ptrCountdownLabel->set("theme_override_font_sizes/font_size", 45);
    m_ptrCountdownLabel->set_global_position(Vector2(-300, 300));
    m_ptrCountdownLabel->set_z_index(1000);
    m_ptrCountdownLabel->show();

    m_ptrCenterLabel->set_horizontal_alignment(godot::HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
    m_ptrCenterLabel->set_vertical_alignment(godot::VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
    m_ptrCenterLabel->set_anchors_preset(Control::LayoutPreset::PRESET_CENTER);
    m_ptrCenterLabel->set_h_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
    m_ptrCenterLabel->set_v_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
    m_ptrCenterLabel->set("theme_override_font_sizes/font_size", 100);
    m_ptrCenterLabel->set_z_index(1000);
    m_ptrCenterLabel->hide();

    m_ptrTimer->connect("timeout", Callable(this, "emit_timeout"), Node::ConnectFlags::CONNECT_ONE_SHOT);
}

void Hud::_process(float delta) {
    // This function is executed both in-game and in the Godot Editor,
    // hence the need for this condition to interrupt executions from the editor.
    if (Engine::get_singleton()->is_editor_hint())
        return;
    if (Engine::get_singleton()->is_editor_hint())
        return;

    const float remainingTime = m_ptrTimer->get_time_left();
    m_ptrCountdownLabel->set_text(String::num(remainingTime, 3));
}

void Hud::display_victory_text() {
    m_ptrCenterLabel->show();
    m_ptrCountdownLabel->hide();
    m_ptrCenterLabel->set_text("You win!");
}

void Hud::display_defeat_text() {
    m_ptrCenterLabel->show();
    m_ptrCountdownLabel->hide();
    m_ptrCenterLabel->set_text("You lose!");
}

void Hud::start_countdown(const int duration) {
    m_ptrTimer->start(duration);
}

void Hud::emit_timeout() {
    emit_signal("timeout");
}

// ===== Register methods (explicitly exposing them to Godot) =====
void Hud::_bind_methods() {
    ADD_SIGNAL(MethodInfo("timeout"));
    ClassDB::bind_method(D_METHOD("emit_timeout"), &Hud::emit_timeout);
}