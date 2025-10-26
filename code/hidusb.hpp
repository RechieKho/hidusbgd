#ifndef HIDUSBGD_HIDUSB_HPP
#define HIDUSBGD_HIDUSB_HPP

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "hid_device.hpp"
#include "hid_device_overview.hpp"
#include "hidapi.h"
#include <cstdint>

namespace HIDUSBGD {

using namespace godot;

namespace implementation {

template <typename TVoidType = void> class HIDUSB final : public Object {
  GDCLASS(HIDUSB, Object);

private:
  static HIDUSB *sm_singleton;

  bool m_is_initialized;

protected:
  static auto _bind_methods() -> void {
    ClassDB::bind_method(D_METHOD("is_initialized"), &HIDUSB::is_initialized);
    ClassDB::bind_method(D_METHOD("initialize"), &HIDUSB::initialize);
    ClassDB::bind_method(D_METHOD("get_device_overviews"),
                         &HIDUSB::get_device_overviews);
  }

public:
  static auto get_singleton_name() -> StringName {
    return StringName("HIDUSB");
  }

  static auto capture_singleton() -> HIDUSB * {
    if (!sm_singleton)
      sm_singleton = memnew(HIDUSB);
    return sm_singleton;
  }

  static auto release_singleton() -> void {
    if (!sm_singleton)
      return;
    memdelete(sm_singleton);
    sm_singleton = nullptr;
  }

  HIDUSB() { initialize(); }

  ~HIDUSB() {
    if (m_is_initialized)
      hid_exit();
  }

  auto is_initialized() const -> bool { return m_is_initialized; }

  auto initialize() -> bool {
    if (m_is_initialized)
      return true;
    m_is_initialized = hid_init() == 0;
    return m_is_initialized;
  }

  auto get_device_overviews() -> Array {
    ERR_FAIL_COND_V_MSG(
        !is_initialized(), Array(),
        "Unable to obtain device overviews, driver is not initialized.");

    auto result = Array();

    auto enumeration = hid_enumerate(0, 0);
    for (auto device_info = enumeration; device_info != nullptr;
         device_info = device_info->next) {
      auto device_overview = Ref<HIDDeviceOverview<>>();
      device_overview.instantiate();
      device_overview->set_path(device_info->path);
      device_overview->set_vendor_id(device_info->vendor_id);
      device_overview->set_product_id(device_info->product_id);
      device_overview->set_serial_number(device_info->serial_number);
      device_overview->set_release_number(device_info->release_number);
      device_overview->set_manufacturer_string(
          device_info->manufacturer_string);
      device_overview->set_product_string(device_info->product_string);
      device_overview->set_interface_number(device_info->interface_number);
      result.append(device_overview);
    }
    hid_free_enumeration(enumeration);

    return result;
  }

  auto open(int64_t p_vendor_id, int64_t p_product_id) -> Ref<HIDDevice<>> {
    ERR_FAIL_COND_V_MSG(
        !is_initialized(), Ref<HIDDevice<>>(),
        "Unable to obtain device overviews, driver is not initialized.");

    const auto raw_device = hid_open(p_vendor_id, p_product_id, NULL);
    auto device = Ref<HIDDevice<>>();
    device.instantiate();
    device->m_raw_device = raw_device;
    return device;
  }

  auto open_from_path(const String &p_path) -> Ref<HIDDevice<>> {
    ERR_FAIL_COND_V_MSG(
        !is_initialized(), Ref<HIDDevice<>>(),
        "Unable to obtain device overviews, driver is not initialized.");

    const auto raw_device = hid_open_path(p_path.utf8().ptr());
    auto device = Ref<HIDDevice<>>();
    device.instantiate();
    device->m_raw_device = raw_device;
    return device;
  }
};

template <typename TVoidType>
HIDUSB<TVoidType> *HIDUSB<TVoidType>::sm_singleton = nullptr;

} // namespace implementation

using HIDUSB = implementation::HIDUSB<>;

} // namespace HIDUSBGD

#endif // HIDUSBGD_HIDUSB_HPP