![screenshot](/com.rvoronov.msfsDock.sdPlugin/docs/images/plugin_icon.png)
# MSFSDock - plugin for MSFS2024 for Mirabox\Ajazz Streamdock and Elgato Streamdeck devices

# Example
| ![](/com.rvoronov.msfsDock.sdPlugin/docs/images/boeing_example.png) | ![](/com.rvoronov.msfsDock.sdPlugin/docs/images/airbus_example.png) |
|---------|---------|
| Boeing style skin | Airbus style skin |

# Features
- Display generic MSFS2024 variables
- Display MSFS2024 L-vars
- Trigger generic MSFS2024 events
- Trigger Mobiflight\WASM events
- PMDG support!!!
- Support for both buttons and encoders

# Supported devices
- Mirabox/Ajazz or other streamdeck clones.
- Elgato Streamdeck - only buttons verified, knobs may work or not.

# Startup procedure
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/no_sim_connection.png)

By default plugin will be in disconnected state (you will see NO SIM text or red outline).

Plugin will attempt to connect to sim on any Button\Knob\Display press if in disconnect state.

So correct procedure would be:
- Start sim
- After sim loaded atleast to menu press any button\knob\screen on streamdock.
- If you see NO SIM text and red borders gone then you good to go.
- Plugin will remain connected until sim closed (or crashed, as usual), in that case start from first step again.

# Supported vars and events
This plugin supports regular variables along with L-vars and MobiFligtht\WASM events. For example:
- AUTOPILOT ALTITUDE LOCK VAR:3 - regular variable, could pick it from MSFS-SDK
- AUTOPILOT NAV1 LOCK - regular variable, could pick it from MSFS-SDK
- MobiFlight.FCC_ALTITUDE_SEL_Inc - event from MobiFlight WASM module
- MobiFlight.FCC_VERTICAL_SPEED_SEL_Dec - event from MobiFlight WASM module
- L:AS01B_AUTO_THROTTLE_ARM_STATE - L-var variable, you should google them or use Devmode to find them
- L:AP_VS_ACTIVE - L-var variable, you should google them or use Devmode to find them

# Plugin Actions
Supported actions:
- Generic Button
- PMDG Button
- Conditional Button
- Generic Switch
- PMDG Switch
- Generic Gauge
- Generic Dial (single)
- PMDG Dial (single)
- Generic Dual Dial
- PMDG Dual Dial
- Generic Radio

For more information see [ACTIONS.md](/com.rvoronov.msfsDock.sdPlugin/docs/ACTIONS.md)

# Installation and configuration

## Mirabox/Ajazz
Copy `com.rvoronov.msfsDock.sdPlugin` folder into `%appdata%/Hotspot/StreamDock/plugins/`.
Create new action, configure appropriate variables and events for action, run sim.

## Elgato
Doubleclick com.rvoronov.msfsDock.sdPlugin.streamDeckPlugin file, Elgato app will open and offer you to install a plugin.
Create new action, configure appropriate variables and events for action, run sim.

# Known issues and limitations
- Most actions display only INT values, except RADIO and GAUGE (could display percents as well).
- For PMDG only binary switches (2 position switches, buttons) and dials are supported for now.

# Updating plugin from pre 0.8 to 0.8+
Version 0.8.0.0 has breaking changes for old actions. If you update plugin from version less than 0.8.0.0 to 0.8.0.0+ your action won't work.

But there is a way to save then. To do so follow this instruction.
- Export your scene(s) with MSFSDock actions from software (SDProfile files)
- Run convert.py script located in utils folder of a plugin (python 3.6+ required)

    python convert.py PATH_TO_EXPORTED_SDProfile_file

- This will create new SDProfile file nearby. Now import it into software and it should work.
- If you had Gauge actions, you will need to repick colors in action settings.

# Links and info
Github page: https://github.com/voronovrs/MSFSDock

Standart event IDs could be found here https://docs.flightsimulator.com/html/Programming_Tools/Event_IDs/Event_IDs.htm

Standart variables could be found here https://docs.flightsimulator.com/html/Programming_Tools/SimVars/Aircraft_SimVars/Aircraft_AutopilotAssistant_Variables.htm

You may need to do some research to find L-vars for specific plane, configure WASM/Mobiflight variables and events, this is not part of this document.
