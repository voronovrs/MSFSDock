![screenshot](images/plugin_icon.png)
# This is work in progress

# Example setup
| ![](images/boeing_example.png) | ![](images/airbus_example.png) |
|---------|---------|
| Boeing style skin | Airbus style skin |

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
- Button event - event to be triggered by button press
- Display variable - Variable used in displaying additional data on a button if necessary
- Status variable - Variable used in displaying active status (green light in the button bottom)

## Generic Dial (single)
This action intended to implement plane dial on Stream Dock display. Dial can display value, change value by rotating a knob, call event by pressing a knob/screen.
### Generic Dial parameters:
- Header - header to be displayed on a dial
- Display variable - Variable to display on a dial
- Inc event - event to be triggered by rotating knob clockwise
- Dec event - event to be triggered by rotating knob counterclockwise
- Knob event - event to be triggered by pushing a knob or screen.
- Status variable - Variable used in displaying active status (bottom field of a dial)

## Dual Dial
This action intended to implement two separate dials on Stream Dock display. Dials can display values, change value of active dial (highlighted) by rotating a knob, active dial changes by pushing know or display. This could be usefull for pair values like Course, Radios etc, or just to save some space.
### Dual Dial parameters:
- Header - header to be displayed on a dial
- Dial 1 variable - Variable to display on dial 1
- Inc 1 event - event to be triggered by rotating knob clockwise when dial 1 is active
- Dec 1 event - event to be triggered by rotating knob counterclockwise when dial 1 is active
- Dial 2 variable - Variable to display on dial 2
- Inc 2 event - event to be triggered by rotating knob clockwise when dial 2 is active
- Dec 2 event - event to be triggered by rotating knob counterclockwise when dial 2 is active

## Generic Gauge
This action intended to display data from Sim on Stream Dock button with gauge like interface.
### Generic Gauge parameters:
- Header - header to be displayed on a button
- Display variable - Variable used in displaying data
- Min value - Minimum value of a gauge
- Max value - Maximum value of a gauge
- Style - Select gauge style, Indicator - display current value with indicator on a scale, Fill - fill scale up to current value
- Scale - choose scale color (default - Yellow)
- Indicator - choose indicator color (default - Red)
- Background - choose background color (default - Black)

# Installation

Copy `com.rvoronov.msfsDock.sdPlugin` folder into `%appdata%/Hotspot/StreamDock/plugins/`.

# Known issues and limitations
- No choice of variable type, only integer so far
- Probably a lot of bugs with registering\deregistering variables and events
