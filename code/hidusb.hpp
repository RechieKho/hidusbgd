#ifndef HIDUSB_HPP
#define HIDUSB_HPP

#include <hidapi.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/object.hpp>

namespace HIDUSB
{

    using namespace godot;

    namespace implementation
    {

        template <typename = void>
        class HIDUSB : public Object
        {
            GDCLASS(HIDUSB, Object);

        private:
            using Self = HIDUSB<>;

        protected:
            static auto _bind_methods() -> void
            {
                ClassDB::bind_static_method("HIDUSB", D_METHOD("print_hello_world"), &Self::print_hello_world);
            }

        public:
            static auto print_hello_world() -> void
            {
                UtilityFunctions::print("Hello world!");
            }
        };

    } // namespace implementation

    using HIDUSB = implementation::HIDUSB<>;

} // namespace HIDUSB

#endif // HIDUSB_HPP