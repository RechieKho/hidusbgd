#ifndef HIDUSBGD_HID_DEVICE_HPP
#define HIDUSBGD_HID_DEVICE_HPP

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/error_macros.hpp"
#include "godot_cpp/variant/packed_byte_array.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "hid_device_overview.hpp"
#include "hidapi.h"
#include <cstdint>

namespace HIDUSBGD {

using namespace godot;

namespace implementation {

template <typename TVoidType = void> class HIDDevice final : public RefCounted {
  GDCLASS(HIDDevice, RefCounted)

  template <typename TTVoidType> friend class HIDUSB;

public:
private:
  hid_device *m_raw_device;

protected:
  static auto _bind_methods() -> void {
    ClassDB::bind_method(D_METHOD("is_captured"), &HIDDevice::is_captured);
    ClassDB::bind_method(D_METHOD("write", "data"), &HIDDevice::write);
    ClassDB::bind_method(D_METHOD("read", "byte_count"), &HIDDevice::read);
    ClassDB::bind_method(
        D_METHOD("read_timeout", "byte_count", "timeout_milliseconds"),
        &HIDDevice::read_timeout);
    ClassDB::bind_method(D_METHOD("blocking", "is_blocking"),
                         &HIDDevice::blocking);
    ClassDB::bind_method(D_METHOD("send_feature_report", "data"),
                         &HIDDevice::send_feature_report);
    ClassDB::bind_method(D_METHOD("get_feature_report", "byte_count"),
                         &HIDDevice::get_feature_report);
    ClassDB::bind_method(D_METHOD("send_output_report", "data"),
                         &HIDDevice::send_output_report);
    ClassDB::bind_method(D_METHOD("get_input_report", "byte_count"),
                         &HIDDevice::get_input_report);
    ClassDB::bind_method(D_METHOD("get_device_overview"),
                         &HIDDevice::get_device_overview);
    ClassDB::bind_method(D_METHOD("get_report_descriptor", "descriptor_size"),
                         &HIDDevice::get_report_descriptor);
  }

public:
  HIDDevice() {}
  ~HIDDevice() { release(); }

  auto is_captured() const -> bool { return m_raw_device != nullptr; }

  auto release() -> void {
    if (!is_captured())
      return;
    hid_close(m_raw_device);
    m_raw_device = nullptr;
  }

  auto write(const PackedByteArray &p_data) -> int {
    ERR_FAIL_COND_V_MSG(!is_captured(), 0,
                        "Unable to write to uncaptured (null) device.");

    const auto write_status =
        hid_write(m_raw_device, p_data.ptr(), p_data.size());

    ERR_FAIL_COND_V_MSG(write_status < 0, 0,
                        vformat(("Unable to write to device. [Error: %s]"),
                                String(hid_error(m_raw_device))));

    return write_status;
  }

  auto read(int64_t p_byte_count) const -> PackedByteArray {
    ERR_FAIL_COND_V_MSG(!is_captured(), PackedByteArray(),
                        "Unable to read from uncaptured (null) device.");

    auto result = PackedByteArray();
    result.resize(p_byte_count); // First byte would be the report number if
                                 // the device uses numbered reports.
    auto read_status = hid_read(m_raw_device, result.ptrw(), p_byte_count);

    ERR_FAIL_COND_V_MSG(read_status < 0, PackedByteArray(),
                        vformat(("Unable to read from device. [Error: %s]"),
                                String(hid_read_error(m_raw_device))));
    return result;
  }

  auto read_timeout(int64_t p_byte_count, int p_timeout_milliseconds) const
      -> PackedByteArray {
    ERR_FAIL_COND_V_MSG(!is_captured(), PackedByteArray(),
                        "Unable to read from uncaptured (null) device.");

    auto result = PackedByteArray();
    result.resize(p_byte_count); // First byte would be the report number if
                                 // the device uses numbered reports.
    auto read_status = hid_read_timeout(m_raw_device, result.ptrw(),
                                        p_byte_count, p_timeout_milliseconds);

    ERR_FAIL_COND_V_MSG(read_status < 0, PackedByteArray(),
                        vformat(("Unable to read from device. [Error: %s]"),
                                String(hid_read_error(m_raw_device))));
    return result;
  }

  auto blocking(bool p_is_blocking) -> void {
    ERR_FAIL_COND_MSG(
        !is_captured(),
        "Unable to set blocking mode for uncaptured (null) device.");

    ERR_FAIL_COND_MSG(hid_set_nonblocking(m_raw_device, !p_is_blocking) < 0,
                      vformat("Unable to set blocking mode. [Error: %s]",
                              String(hid_error(m_raw_device))));
  }

  auto send_feature_report(const PackedByteArray &p_data) -> int {
    ERR_FAIL_COND_V_MSG(
        !is_captured(), 0,
        "Unable to send feature report to uncaptured (null) device.");

    const auto send_status =
        hid_send_feature_report(m_raw_device, p_data.ptr(), p_data.size());

    ERR_FAIL_COND_V_MSG(
        send_status < 0, 0,
        vformat(("Unable to send feature report to device. [Error: %s]"),
                String(hid_error(m_raw_device))));

    return send_status;
  }

  auto get_feature_report(int64_t p_byte_count) const -> PackedByteArray {
    ERR_FAIL_COND_V_MSG(
        !is_captured(), PackedByteArray(),
        "Unable to get feature report from uncaptured (null) device.");

    auto result = PackedByteArray();
    result.resize(p_byte_count); // First byte would be the report number if
                                 // the device uses numbered reports.
    const auto get_status =
        hid_get_feature_report(m_raw_device, result.ptrw(), p_byte_count);

    ERR_FAIL_COND_V_MSG(
        get_status < 0, PackedByteArray(),
        vformat(("Unable to get feature report from device. [Error: %s]"),
                String(hid_error(m_raw_device))));
    return result;
  }

  auto send_output_report(const PackedByteArray &p_data) -> int {
    ERR_FAIL_COND_V_MSG(
        !is_captured(), 0,
        "Unable to send output report to uncaptured (null) device.");

    const auto send_status =
        hid_send_output_report(m_raw_device, p_data.ptr(), p_data.size());

    ERR_FAIL_COND_V_MSG(
        send_status < 0, 0,
        vformat(("Unable to send output report to device. [Error: %s]"),
                String(hid_error(m_raw_device))));

    return send_status;
  }

  auto get_input_report(int64_t p_byte_count) const -> PackedByteArray {
    ERR_FAIL_COND_V_MSG(
        !is_captured(), PackedByteArray(),
        "Unable to get input report from uncaptured (null) device.");

    auto result = PackedByteArray();
    result.resize(p_byte_count); // First byte would be the report number if
                                 // the device uses numbered reports.
    const auto get_status =
        hid_get_input_report(m_raw_device, result.ptrw(), p_byte_count);

    ERR_FAIL_COND_V_MSG(
        get_status < 0, PackedByteArray(),
        vformat(("Unable to get input report from device. [Error: %s]"),
                String(hid_error(m_raw_device))));
    return result;
  }

  auto get_device_overview() const -> Ref<HIDDeviceOverview<>> {
    ERR_FAIL_COND_V_MSG(
        !is_captured(), Ref<HIDDeviceOverview<>>(),
        "Unable to get overview from uncaptured (null) device.");

    auto device_info = hid_get_device_info(m_raw_device);
    ERR_FAIL_COND_V_MSG(
        !device_info, Ref<HIDDeviceOverview<>>(),
        vformat("Unable to get overview from device. [Error: %s]",
                String(hid_error(m_raw_device))));

    auto device_overview = Ref<HIDDeviceOverview<>>();
    device_overview.instantiate();
    device_overview->set_path(device_info->path);
    device_overview->set_vendor_id(device_info->vendor_id);
    device_overview->set_product_id(device_info->product_id);
    device_overview->set_serial_number(device_info->serial_number);
    device_overview->set_release_number(device_info->release_number);
    device_overview->set_manufacturer_string(device_info->manufacturer_string);
    device_overview->set_product_string(device_info->product_string);
    device_overview->set_interface_number(device_info->interface_number);
    return device_overview;
  }

  auto get_report_descriptor(
      int64_t p_descriptor_size = HID_API_MAX_REPORT_DESCRIPTOR_SIZE) const
      -> PackedByteArray {
    ERR_FAIL_COND_V_MSG(
        !is_captured(), PackedByteArray(),
        "Unable to get report descriptor from uncaptured (null) device.");

    auto result = PackedByteArray();
    result.resize(p_descriptor_size); // First byte would be the report number
                                      // if the device uses numbered reports.
    const auto get_status = hid_get_report_descriptor(
        m_raw_device, result.ptrw(), p_descriptor_size);

    ERR_FAIL_COND_V_MSG(get_status < 0, PackedByteArray(),
                        "Unable to get input report from device.");

    return result;
  }
};

} // namespace implementation

using HIDDevice = implementation::HIDDevice<>;

} // namespace HIDUSBGD

#endif // HIDUSBGD_HID_DEVICE_HPP