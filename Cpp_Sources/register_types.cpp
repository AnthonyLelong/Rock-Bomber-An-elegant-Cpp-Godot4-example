#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "main_rockbomber.h"
#include "hero.h"
#include "bomb.h"
#include "explosion.h"
#include "rock.h"
#include "hud.h"

using namespace godot;

void initialize_RockBomber_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
        return;

    ClassDB::register_class<Main_RockBomber>();
    ClassDB::register_class<Hero>();
    ClassDB::register_class<Bomb>();
    ClassDB::register_class<Explosion>();
    ClassDB::register_class<Rock>();
    ClassDB::register_class<Hud>();
}

void uninitialize_RockBomber_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
        return;

    // No code to execute for this specific game
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT RockBomber_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
    {

        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_RockBomber_module);
        init_obj.register_terminator(uninitialize_RockBomber_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}