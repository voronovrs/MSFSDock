![screenshot](com.rvoronov.msfsDock.sdPlugin/images/plugin_icon.png)
# This is work in progress

# Description
MSFSDock plugin for Ajazz AKP05, Mirabox N4 and other compatible devices. Plugin allows interaction with Microsoft Flight Simulartor 2020 and 2024, read sim variable, toggle actions, change values etc.

# Supported vars and events
This plugin supports regular variables along with L-vars and MobiFligtht\WASM events. For example:
- AUTOPILOT ALTITUDE LOCK VAR:3 - regular variable, could pick it from MSFS-SDK
- AUTOPILOT NAV1 LOCK - regular variable, could pick it from MSFS-SDK
- MobiFlight.FCC_ALTITUDE_SEL_Inc - event from MobiFlight WASM module
- MobiFlight.FCC_VERTICAL_SPEED_SEL_Dec - event from MobiFlight WASM module
- L:AS01B_AUTO_THROTTLE_ARM_STATE - L-value variable, you should google them or use Devmode to find them
- L:AP_VS_ACTIVE - L-value variable, you should google them or use Devmode to find them

# Plugin Actions
## Generic Button
This action intended to implement plane button on Stream Dock button. Button can display header, active state and value if necessary.
### Generic Button parameters:
- Header - header to be displayed on a button
- Event - event to be triggered by button
- Status variable - Variable used in displaying active status (green light in the button bottom)
- Display variable - Variable used in displaying additional data on a button if necessary

## Generic Dial
(Not in build yet)
This action intended to implement plane dial on Stream Dock display. Dial can display one or two values, change value by rotating a knob, call event by pressing a knob/screen or switch between dials if dual dial is set and both dials are configured.
### Generic Dial parameters:
#### Common settings
- Type - Singe or Dual dial setup
- Header - header to be displayed on a button
- Display variable - Variable to display on a dial (first dial in Dual dial setup)
- Inc event - event to be triggered by rotating knob clockwise
- Dec event - event to be triggered by rotating knob counterclockwise
#### Single dial settings
- Status variable - Variable used in displaying active status
#### Dual dial settings
- Display variable - Variable to display on a second dial
- Inc event - event to be triggered by rotating knob clockwise when second dial active
- Dec event - event to be triggered by rotating knob counterclockwise when second dial active
#### Dial push settings
- Push event - pushing a knob/screen could trigger event, this setting is available in both single and dual dial configuration, but only if one of the dials is configured in dual case (Inc and Dec events set only to one of the dials, if both dials are configured pushing knob will switch between dials).

# Installation

Copy `com.rvoronov.msfsDock.sdPlugin` folder into `%appdata%/Hotspot/StreamDock/plugins/`.
