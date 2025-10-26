#include "register_types.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "hid_device.hpp"
#include "hid_device_overview.hpp"
#include "hidusb.hpp"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_hidusbgd_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  GDREGISTER_CLASS(HIDUSBGD::HIDDeviceOverview);
  GDREGISTER_CLASS(HIDUSBGD::HIDDevice);

  ClassDB::register_class<HIDUSBGD::HIDUSB>();
  Engine::get_singleton()->register_singleton(
      HIDUSBGD::HIDUSB::get_singleton_name(),
      HIDUSBGD::HIDUSB::capture_singleton());
}

void uninitialize_hidusbgd_module(ModuleInitializationLevel p_level) {
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  Engine::get_singleton()->unregister_singleton(
      HIDUSBGD::HIDUSB::get_singleton_name());
  HIDUSBGD::HIDUSB::release_singleton();
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT
hidusbgd_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                      GDExtensionClassLibraryPtr p_library,
                      GDExtensionInitialization *r_initialization) {
  godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library,
                                                 r_initialization);

  init_obj.register_initializer(initialize_hidusbgd_module);
  init_obj.register_terminator(uninitialize_hidusbgd_module);
  init_obj.set_minimum_library_initialization_level(
      MODULE_INITIALIZATION_LEVEL_SCENE);

  return init_obj.init();
}
}