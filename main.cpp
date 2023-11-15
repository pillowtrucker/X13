#include <iostream>
#include <miral/runner.h>
#include <miral/minimal_window_manager.h>
#include <miral/set_window_management_policy.h>
#include <miral/x11_support.h>
#include <miral/toolkit_event.h>
#include <miral/keymap.h>
#include <linux/input.h>
#include <miral/append_event_filter.h>
#include <xkbcommon/xkbcommon-keysyms.h>
#include <miral/external_client.h>

using namespace miral;
using namespace miral::toolkit;

int main(int argc, char const* argv[])
{
    MirRunner runner{argc, argv};
	ExternalClientLauncher external_client_launcher;
	auto const keyboard_shortcuts = [&](MirEvent const* event)
        {
			
            if (mir_event_get_type(event) != mir_event_type_input)
                return false;

            MirInputEvent const* input_event = mir_event_get_input_event(event);
            if (mir_input_event_get_type(input_event) != mir_input_event_type_key)
                return false;

            MirKeyboardEvent const* kev = mir_input_event_get_keyboard_event(input_event);
            if (mir_keyboard_event_action(kev) != mir_keyboard_action_down)
                return false;

            MirInputEventModifiers mods = mir_keyboard_event_modifiers(kev);
            if (!(mods & mir_input_event_modifier_alt) || !(mods & mir_input_event_modifier_ctrl))
                return false;

            switch (mir_keyboard_event_scan_code(kev))
            {
            case KEY_BACKSPACE:
                runner.stop();
                return true;

            case KEY_T:
                external_client_launcher.launch({"konsole"});
                return true;

            default:
                return false;
            }
        };
    return runner.run_with(
            {
                    X11Support{},
                    set_window_management_policy<MinimalWindowManager>()
            });
}
