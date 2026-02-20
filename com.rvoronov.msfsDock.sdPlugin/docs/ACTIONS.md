# Plugin Actions

## Autocomplete
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/autocomplete.png)

All variable and event fields support autocomplete. Plugin will show possible values when entering two or more characters.

For Generic actions suggested values are from MSFS SDK list of known variables and events and DOES NOT cover all plane systems and possible variants. There are still posibility to show L-vars, WASM variables and events etc.

For PMDG actions suggested variables and events comes from PMDG SDK and covers ALL available values.

## Entering event\variable hints
**Inserted event\variable will only be saved and sent to plugin in next scenarios:**
1. After variable or event input [✓] button to the right of input field pressed.
2. After variable or event input enter is pressed when input field is active.

Examples:
1. You want a button to trigger parking brakes. Steps would be:
- Add MSFS Dock Generic button action to layout.
- In Button event field start printing (for example) "brakes".
- Click on "PARKING_BRAKES" from list.
- Press [✓] button to the right of input field or hit enter.
- Now pressing this button will trigger parking brakes on and off.
2. You want to display light on a button when AP is active in your 737 Max. This is L-var variable. Steps would be:
- Add MSFS Dock Generic button action to layout.
- In Status variable field print "L:AP_VS_ACTIVE".
- Press [✓] button to the right of input field or hit enter.
- Now light on this button wil display your 737 Max AP status.


## Generic Button
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/button_action.png)

This action intended to implement plane button on Stream Dock button. Button can display header, active state and value if necessary.
Controller - button.
### Generic Button parameters:
- Header - header to be displayed on a button
- Skin - select button skin, either Boeing-like or Airbus-like
- Button event - event to be triggered by button press
- Display variable - Variable used in displaying additional data on a button if necessary
- Status variable - Variable used in displaying active status (green light in the button bottom)

## PMDG Button
Same as Generic button, autocomplete will suggest PMDG variables and events.

## Conditional Button
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/conditional_button_action.png)

This action intended to implement plane events depending on conditions. This allows button to send different events based on SimConnect variable value.
For example, you could configure Autopilot button to send AP_MASTER event to turn AP on when it's off, and same AP_MASTER event to turn it off when it's on.
Or configure Landing Gear button to send GEAR_UP when gear is extended and GEAR_DOWN when gear is retracted, based on GEAR TOTAL PCT EXTENDED variable value.
### Conditional Button parameters:
- Header - header to be displayed on a button
- Skin - select button skin, either Boeing-like or Airbus-like
- Compare Var - Variable to evaluate for condition
- Operator - Comparison operator (==, !=, >, <, >=, <=)
- Compare Value - Value to compare against
- When True - Event to send when condition is true
- When False - Event to send when condition is false
- Display variable - Variable used in displaying additional data on a button if necessary
- Status variable - Variable used in displaying active status (green light in the button bottom)

#### Conditional Events
Conditional mode supports 6 comparison operators:
- == (Equal) - condition is true when variable equals specified value
- != (Not Equal) - condition is true when variable differs from specified value
- \> (Greater Than) - condition is true when variable is greater than specified value
- < (Less Than) - condition is true when variable is less than specified value
- \>= (Greater or Equal) - condition is true when variable is greater than or equal to specified value
- <= (Less or Equal) - condition is true when variable is less than or equal to specified value

Suggested configuration for smart Autopilot altitude lock toggle:
- Conditional Variable: AUTOPILOT ALTITUDE LOCK
- Operator: == (Equal)
- Condition Value: 1
- Event When True: AP_ALT_HOLD_OFF (to turn off AP ALTITUDE LOCK when it is on)
- Event When False: AP_ALT_HOLD_ON (to turn on AP ALTITUDE LOCK when it is off)
- Status variable: AUTOPILOT ALTITUDE LOCK (to show green light when ALTITUDE LOCK is active)

## Generic Switch
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/switch_action.png)

This action intended to implement plane switch on Stream Dock button. Switch can have either 2 or 3 position, positions could be labled.
Controller - button.
### Generic Switch parameters:
- Header - header to be displayed on a switch.
- Switch event - event to be triggered by button press, usually switch toggle event.
- Switch variable - Variable used in displaying switch position.
- Num positions - Select 2 or 3 position switch mode.
- Positions - Define mapping of switch position value in sim (left field) and displayed text (right field, 5 characters max).

## PMDG Switch
Same as Generic switch, autocomplete will suggest PMDG variables and events.

## Generic Gauge
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/gauge_action.png)

This action intended to display data from Sim on Stream Dock button with gauge like interface.
Controller - Button.
### Generic Gauge parameters:
- Header - header to be displayed on a gauge
- Display variable - Variable used in displaying data
- Format - Format of displayed data, integer or percent value.
- Style - Select gauge style, Indicator - display current value with indicator on a scale, Fill - fill scale up to current value
- Min value - Minimum value of a gauge
- Max value - Maximum value of a gauge
- Scale - choose scale color (default - Yellow)
- Indicator - choose indicator color (default - Red)
- Background - choose background color (default - Black)

## Generic Dial (single)
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/dial_action.png)

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

## PMDG Dial
Same as Generic dial, autocomplete will suggest PMDG variables and events.

## Generic Dual Dial
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/dual_dial_action.png)

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

## PMDG Dual Dial
Same as Generic dual dial, autocomplete will suggest PMDG variables and events.

## Generic Radio
![](/com.rvoronov.msfsDock.sdPlugin/docs/images/radio_action.png)

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
