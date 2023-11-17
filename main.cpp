#include <miral/configuration_option.h>
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
#include <mir/graphics/display_configuration.h>
#include <miral/display_configuration.h>
using namespace miral;
using namespace miral::toolkit;

int main(int argc, char const* argv[])
{
  MirRunner runner{argc, argv}; // obvious
  ExternalClientLauncher external_client_launcher;
  Keymap config_keymap;
  config_keymap.set_keymap("gb");
  
  //  static constexpr mir::graphics::DisplayConfigurationMode test_display_mode{{1920,1080},300};
  //  mir::graphics::DisplayConfigurationOutput vs;
  // vs.preferred_mode_index = 0;
  // vs.current_mode_index = 0;
  // vs.modes.push_back(test_display_mode);
  //  miral::DisplayConfiguration;

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
           external_client_launcher.launch({std::string("konsole")});
		   return true;

	  default:
			 return false;
    }
  };
  DisplayConfiguration display_config{runner};

  return runner.run_with(
  {
    display_config,
    display_config.layout_option(),
	X11Support{},
	config_keymap,
	external_client_launcher,
	AppendEventFilter(keyboard_shortcuts),
	set_window_management_policy<MinimalWindowManager>()
  });
}
