![screenshot](com.rvoronov.msfsDock.sdPlugin/images/plugin_icon.png)
# Description

MSFSDock plugin for Ajazz AKP05, Mirabox N4 and other compatible devices. Plugin allows interaction with Microsoft Flight Simulartor 2020 and 2024, read sim variable, toggle actions, change values etc.

# Plugin Actions
## Generic Button
This action intended to implement plane button on Stream Dock button. Button can display header, active state and value if necessary.
### Generic Button parameters:
- Header - header to be displayed on a button
- Event - event to be triggered by button
- Status variable - Variable used in displaying active status
- Display variable - Variable used in displaying additional data on a button if necessary

## Generic Dial
This action intended to implement plane dial on Stream Dock knob. Dial can display one or two values, change value by rotating a knob, call event by pressing a knob or switch between dials if dual dial is set and both dials are configurable.
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
- Push event - pushing a knob could trigger event, this setting is available in both single and dual dial configuration, but only if one of the dials is configurable in dual case (Inc and Dec events set only to one of the dials, if both dials are configurable in dual dial pushing knob will switch between dials.

# Installation

Copy `com.rvoronov.msfsDock.sdPlugin` folder into `%appdata%/Hotspot/StreamDock/plugins/`.
