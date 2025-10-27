#ifndef HIDUSBGD_HID_DEVICE_OVERVIEW_HPP
#define HIDUSBGD_HID_DEVICE_OVERVIEW_HPP

#include <cstdint>
#include <hidapi.h>

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/array.hpp"
#include "godot_cpp/variant/variant.hpp"

namespace HIDUSBGD {

using namespace godot;

namespace implementation {

template <typename TVoidType = void>
class HIDDeviceOverview final : public RefCounted {
  GDCLASS(HIDDeviceOverview, RefCounted);

public:
private:
  String m_path;
  int64_t m_vendor_id;
  int64_t m_product_id;
  String m_serial_number;
  int64_t m_release_number;
  String m_manufacturer_string;
  String m_product_string;
  int64_t m_interface_number;

protected:
  static auto _bind_methods() -> void {

    ClassDB::bind_method(D_METHOD("set_path", "path"),
                         &HIDDeviceOverview::set_path);
    ClassDB::bind_method(D_METHOD("get_path"), &HIDDeviceOverview::get_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "path"), "set_path", "get_path");

    ClassDB::bind_method(D_METHOD("set_vendor_id", "vendor_id"),
                         &HIDDeviceOverview::set_vendor_id);
    ClassDB::bind_method(D_METHOD("get_vendor_id"),
                         &HIDDeviceOverview::get_vendor_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "vendor_id"), "set_vendor_id",
                 "get_vendor_id");

    ClassDB::bind_method(D_METHOD("set_product_id", "product_id"),
                         &HIDDeviceOverview::set_product_id);
    ClassDB::bind_method(D_METHOD("get_product_id"),
                         &HIDDeviceOverview::get_product_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "product_id"), "set_product_id",
                 "get_product_id");

    ClassDB::bind_method(D_METHOD("set_serial_number", "serial_number"),
                         &HIDDeviceOverview::set_serial_number);
    ClassDB::bind_method(D_METHOD("get_serial_number"),
                         &HIDDeviceOverview::get_serial_number);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "serial_number"),
                 "set_serial_number", "get_serial_number");

    ClassDB::bind_method(D_METHOD("set_release_number", "release_number"),
                         &HIDDeviceOverview::set_release_number);
    ClassDB::bind_method(D_METHOD("get_release_number"),
                         &HIDDeviceOverview::get_release_number);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "release_number"),
                 "set_release_number", "get_release_number");

    ClassDB::bind_method(
        D_METHOD("set_manufacturer_string", "manufacturer_string"),
        &HIDDeviceOverview::set_manufacturer_string);
    ClassDB::bind_method(D_METHOD("get_manufacturer_string"),
                         &HIDDeviceOverview::get_manufacturer_string);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "manufacturer_string"),
                 "set_manufacturer_string", "get_manufacturer_string");

    ClassDB::bind_method(D_METHOD("set_product_string", "product_string"),
                         &HIDDeviceOverview::set_product_string);
    ClassDB::bind_method(D_METHOD("get_product_string"),
                         &HIDDeviceOverview::get_product_string);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "product_string"),
                 "set_product_string", "get_product_string");

    ClassDB::bind_method(D_METHOD("set_interface_number", "interface_number"),
                         &HIDDeviceOverview::set_interface_number);
    ClassDB::bind_method(D_METHOD("get_interface_number"),
                         &HIDDeviceOverview::get_interface_number);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "interface_number"),
                 "set_interface_number", "get_interface_number");
  }

  auto _to_string() const -> String {
    return vformat("<DeviceOverview: [P=%d, V=%d, N=%d]>", m_product_id,
                   m_vendor_id, m_interface_number);
  }

public:
  HIDDeviceOverview() {}
  ~HIDDeviceOverview() {}

  auto get_path() const -> String { return m_path; }
  auto set_path(const String &p_path) -> void { m_path = p_path; }

  auto get_vendor_id() const -> int64_t { return m_vendor_id; }
  auto set_vendor_id(const int64_t &p_vendor_id) -> void {
    m_vendor_id = p_vendor_id;
  }

  auto get_product_id() const -> int64_t { return m_product_id; }
  auto set_product_id(const int64_t &p_product_id) -> void {
    m_product_id = p_product_id;
  }

  auto get_serial_number() const -> String { return m_serial_number; }
  auto set_serial_number(const String &p_serial_number) -> void {
    m_serial_number = p_serial_number;
  }

  auto get_release_number() const -> int64_t { return m_release_number; }
  auto set_release_number(const int64_t &p_release_number) -> void {
    m_release_number = p_release_number;
  }

  auto get_manufacturer_string() const -> String {
    return m_manufacturer_string;
  }
  auto set_manufacturer_string(const String &p_manufacturer_string) -> void {
    m_manufacturer_string = p_manufacturer_string;
  }

  auto get_product_string() const -> String { return m_product_string; }
  auto set_product_string(const String &p_product_string) -> void {
    m_product_string = p_product_string;
  }

  auto get_interface_number() const -> int64_t { return m_interface_number; }
  auto set_interface_number(int64_t p_interface_number) -> void {
    m_interface_number = p_interface_number;
  }
};

} // namespace implementation

using HIDDeviceOverview = implementation::HIDDeviceOverview<>;

} // namespace HIDUSBGD

#endif // HIDUSBGD_HID_DEVICE_OVERVIEW_HPP