#ifndef HUD_H
#define HUD_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>

using namespace godot;

class Hud : public Control {
    GDCLASS(Hud, Control)

public:
    // ===== Required by Godot ======
    static void _bind_methods();
    void _ready();
    void _process(float delta);

private:
    // Pointers to child nodes
    Label* m_ptrCountdownLabel;
    Label* m_ptrCenterLabel;

    Timer* m_ptrTimer;

public:
    void display_victory_text();
    void display_defeat_text();
    void start_countdown(const int duration);

    // Explicitely declared/exposed to Godot
    void emit_timeout();
};

#endif