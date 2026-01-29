![screenshot](images/plugin_icon.png)
# MSFSDock - plugin for MSFS2024 for Mirabox\Ajazz streamdocks (v0.7.1)

# Usage example
| ![](images/boeing_example.png) | ![](images/airbus_example.png) |
|---------|---------|
| Boeing style skin | Airbus style skin |

# Description
MSFSDock plugin for Ajazz AKP05, Mirabox N4 and other compatible devices. Plugin allows interaction with Microsoft Flight Simulartor 2020 and 2024, read sim variable, toggle actions, change values etc.

# Startup procedure
![](images/no_sim_connection.png)

Since v0.6.0 no autoconnect to sim is available. By default plugin will be in disconnected state.

You will see NO SIM text on a buttons and red border on another actions in that state.

This was done to prevent excessive memory allocation when constantly trying to connect to sim (known SimConnect behavior).

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
## Entering event\variable hints
When entering any text into event\variable input field plugin will show known events\variables list, from which you can peek needed event\varible.
Possibility to enter custom events\variables still remains (L-var or Mobiflight for example).
NOTE: Inserted event\variable will only be set to plugin in next scenarios:
1. Needed event\variable picked from list.
2. Custom event\variable entered and focus changed to another field or out of current field.

Examples:
1. You want a button to trigger parking brakes. Steps would be:
- Add MSFS Dock Generic button action to layout.
- In Button event field start printing (for example) "brakes".
- Click on "PARKING_BRAKES" from list.
- Now pressing this button will trigger prking brakes on and off.
2. You want to display light on a button when AP is active in your 737 Max. This is L-var ariable. Steps would be:
- Add MSFS Dock Generic button action to layout.
- In Status variable field print "L:AP_VS_ACTIVE".
- Click on out of this field (but remain in button settings) or on any other field. This is essential!
- Now light on this button wil display your 737 Max AP status.

# Plugin Actions
## Generic Button
This action intended to implement plane button on Stream Dock button. Button can display header, active state and value if necessary.
Controller - button.
### Generic Button parameters:
- Header - header to be displayed on a button
- Skin - select button skin, either Boeing-like or Airbus-like
- Event Mode - select between Simple or Conditional event mode
  - **Simple mode**: Single event triggered by button press
  - **Conditional mode**: Different events triggered based on variable value
- Button event (Simple mode) - event to be triggered by button press
- Conditional Variable (Conditional mode) - Variable to evaluate for condition
- Operator (Conditional mode) - Comparison operator (==, !=, >, <, >=, <=)
- Condition Value (Conditional mode) - Value to compare against
- Event When True (Conditional mode) - Event to send when condition is true
- Event When False (Conditional mode) - Event to send when condition is false
- Display variable - Variable used in displaying additional data on a button if necessary
- Status variable - Variable used in displaying active status (green light in the button bottom)

#### Conditional Events
Now Generic Button supports conditional events. This allows button to send different events based on SimConnect variable value.
For example, you could configure Autopilot button to send AP_MASTER event to turn AP on when it's off, and same AP_MASTER event to turn it off when it's on.
Or configure Landing Gear button to send GEAR_UP when gear is extended and GEAR_DOWN when gear is retracted, based on GEAR TOTAL PCT EXTENDED variable value.

Conditional mode supports 6 comparison operators:
- == (Equal) - condition is true when variable equals specified value
- != (Not Equal) - condition is true when variable differs from specified value
- > (Greater Than) - condition is true when variable is greater than specified value
- < (Less Than) - condition is true when variable is less than specified value
- >= (Greater or Equal) - condition is true when variable is greater than or equal to specified value
- <= (Less or Equal) - condition is true when variable is less than or equal to specified value

Example configuration for smart Autopilot toggle:
- Event Mode: Conditional
- Conditional Variable: AUTOPILOT MASTER
- Operator: == (Equal)
- Condition Value: 1
- Event When True: AP_MASTER (to turn off when AP is on)
- Event When False: AP_MASTER (to turn on when AP is off)
- Status variable: AUTOPILOT MASTER (to show green light when AP is active)

## Generic Dial (single)
This action intended to implement plane dial on Stream Dock display. Dial can display value, change value by rotating a knob, call event by pressing a knob/screen.
Controller - Knob/Display.
### Generic Dial parameters:
- Header - header to be displayed on a dial
- Skin - select dial skin, either Boeing-like or Airbus-like
- Display variable - Variable to display on a dial
- Inc event - event to be triggered by rotating knob clockwise
- Dec event - event to be triggered by rotating knob counterclockwise
- Knob event - event to be triggered by pushing a knob or screen.
- Status variable - Variable used in displaying active status (bottom field of a dial)

## Dual Dial
This action intended to implement two separate dials on Stream Dock display. Dials can display values, change value of active dial (highlighted) by rotating a knob, active dial changes by pushing knob or display. This could be usefull for pair values like Course, Radios etc, or just to save some space.
Controller - Knob/Display.
### Dual Dial parameters:
- Header - header to be displayed on a dial
- Skin - select dial skin, either Boeing-like or Airbus-like
- Dial 1 variable - Variable to display on dial 1
- Inc 1 event - event to be triggered by rotating knob clockwise when dial 1 is active
- Dec 1 event - event to be triggered by rotating knob counterclockwise when dial 1 is active
- Dial 2 variable - Variable to display on dial 2
- Inc 2 event - event to be triggered by rotating knob clockwise when dial 2 is active
- Dec 2 event - event to be triggered by rotating knob counterclockwise when dial 2 is active

## Generic Gauge
This action intended to display data from Sim on Stream Dock button with gauge like interface.
Controller - Button.
### Generic Gauge parameters:
- Header - header to be displayed on a gauge
- Display variable - Variable used in displaying data
- Format - Format of displayed data, integer or percent value.
- Min value - Minimum value of a gauge
- Max value - Maximum value of a gauge
- Style - Select gauge style, Indicator - display current value with indicator on a scale, Fill - fill scale up to current value
- Scale - choose scale color (default - Yellow)
- Indicator - choose indicator color (default - Red)
- Background - choose background color (default - Black)

## Generic Radio
This action intended to display active and standby radios (NAV, COM etc), change whole and fractional part and swap between them.
Controller - Knob/Display.
### Generic Radio parameters:
- Header - header to be displayed on a radio card
- Skin - select radio skin, either Boeing-like or Airbus-like
- Active Radio var - Variable used to display active radio frequency
- Stdby Radio var - Variable used to display standby radio frequency

- Inc event - event to increase standby frequency whole part
- Dec event - event to dencrease standby frequency whole part
- Inc frac event - event to increase standby frequency fractional part
- Dec frac event - event to dencrease standby frequency fractional part
- Swap event - event to swap standby and active frequency (called by screen doubletap or knob doublepress).

#### Changing frequency
Since we have only 1 knob then only whole part either fractional part could be changed at once.
Part of the frequency modifiable now is highlighted. To switch between parts tap the screen or press the knob once.

#### Notes on frequency Swap event
If you configure Swap event then you can swap between frequencies by double tapping a screen or double pressing a knob.
Since it could be finiky for someone then there is always an option to assign Swap event to separate button.

# Installation and configuration

Copy `com.rvoronov.msfsDock.sdPlugin` folder into `%appdata%/Hotspot/StreamDock/plugins/`.
Create new action, configure appropriate variables and events for action, run sim.

Standart event IDs could be found here https://docs.flightsimulator.com/html/Programming_Tools/Event_IDs/Event_IDs.htm

Standart variables could be found here https://docs.flightsimulator.com/html/Programming_Tools/SimVars/Aircraft_SimVars/Aircraft_AutopilotAssistant_Variables.htm

You may need to do some research to find L-vars for specific plane, configure WASM/Mobiflight variables and events, this is not part of this document.

# Known issues and limitations
- Most actions display only INT values, except RADIO and GAUGE (could display percents as well)
- Probably a lot of bugs with registering\deregistering variables and events

# Links
Github page: https://github.com/voronovrs/MSFSDock
