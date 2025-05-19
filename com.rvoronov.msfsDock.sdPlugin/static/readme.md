# Stream Dock Chinese Documentation

Only common simple APIs are recorded. Please read the official documentation for more detailed properties.

## Property Inspector HTML Template

<!-- input -->
<div class="sdpi-item">
    <div class="sdpi-item-label">xxx</div>
    <input class="sdpi-item-value"></input>
</div>

<!-- button -->
<div class="sdpi-item">
    <div class="sdpi-item-label">Button</div>
    <button class="sdpi-item-value">Click Me</button>
</div>

<!-- textarea -->
<div type="textarea" class="sdpi-item">
    <div class="sdpi-item-label">xxx</div>
    <textarea class="sdpi-item-value" type="textarea"></textarea>
</div>

<!-- select -->
<div type="select" class="sdpi-item">
    <div class="sdpi-item-label">xxx</div>
    <select class="sdpi-item-value">
        <option value="xxx">xxx</option>
    </select>
</div>

<!-- checkbox -->
<div type="checkbox" class="sdpi-item">
    <div class="sdpi-item-label">Check Me</div>
    <div class="sdpi-item-value">
        <span class="sdpi-item-child">
            <input id="chk1" type="checkbox" value="left">
            <label for="chk1"><span></span>left</label>
        </span>
        <span class="sdpi-item-child">
            <input id="chk2" type="checkbox" value="right">
            <label for="chk2"><span></span>right</label>
        </span>
    </div>
</div>

<!-- radio -->
<div type="radio" class="sdpi-item">
    <div class="sdpi-item-label">Radio</div>
    <div class="sdpi-item-value">
        <span class="sdpi-item-child">
            <input id="rdio1" type="radio" name="rdio" checked>
            <label for="rdio1" class="sdpi-item-label"><span></span>on</label>
        </span>
        <span class="sdpi-item-child">
            <input id="rdio2" type="radio" value="off" name="rdio">
            <label for="rdio2" class="sdpi-item-label"><span></span>off</label>
        </span>
        <span class="sdpi-item-child">
            <input id="rdio3" type="radio" value="mute" name="rdio">
            <label for="rdio3" class="sdpi-item-label"><span></span>mute</label>
        </span>
    </div>
</div>

<!-- range -->
<div type="range" class="sdpi-item" id="temperatureslider">
    <div class="sdpi-item-label">xxx</div>
    <input type="range" class="sdpi-item-value" min="0" max="100" value=37>
</div>

## Events that plugins can trigger

didReceiveSettings Operation persistent data trigger

{
  "action": "com.example.action1",
  "event": "didReceiveSettings",
  "context": uniqueValue,
  "device": uniqueValue,
  "payload": {
   "settings": {<json data>},
    "coordinates": {
      "column": 3,
      "row": 1
    },
    "isInMultiAction": false
  }
}

keyDown/keyUp/touchTap Triggered when pressed/released/touched

{
    "action": "com.example.action1",
    "event": "keyUp",
    "context": uniqueValue,
    "device": uniqueValue,
    "payload": {
    "settings": {<json data>},
    "coordinates": {
        "column": 3,
        "row": 1
    },
    "state": 0,
    "userDesiredState": 1,
    "isInMultiAction": false
    }
}

willAppear/willDisappear Triggered when a plugin is created/deleted

{
    "action": "com.example.action1",
    "event": "willAppear",
    "context": uniqueValue,
    "device": uniqueValue,
    "payload": {
    "settings": {<json data>},
    "coordinates": {
        "column": 3,
        "row": 1
    },
    "state": 0,
    "isInMultiAction": false
    }
}

titleParametersDidChange Triggered when the user modifies the title/title parameters

{
  "action": "com.example.action1",
  "event": "titleParametersDidChange",
  "context": "uniqueValue",
  "device": "uniqueValue",
  "payload": {
    "coordinates": {
      "column": 3,
      "row": 1
    },
    "settings": {<json data>},
    "state": 0,
    "title": "",
    "titleParameters": {
      "fontFamily": "",
      "fontSize": 12,
      "fontStyle": "",
      "fontUnderline": false,
      "showTitle": true,
      "titleAlignment": "bottom",
      "titleColor": "#ffffff"
    }
  }
}

deviceDidConnect/deviceDidDisconnect Triggered when a device is plugged/unplugged from the computer

{
 "event": "deviceDidConnect",
    "device": uniqueValue,
     "deviceInfo": {
        "name": "Device Name",
        "type": 0,
         "size": {
            "columns": 5,
            "rows": 3
        }
    },
}

propertyInspectorDidAppear/propertyInspectorDidDisappear Triggered when the attribute selector appears/hides in the UI

{
  "action": "com.example.action1",
  "event": "propertyInspectorDidAppear",
  "context": uniqueValue,
  "device": uniqueValue
}

sendToPlugin Triggered when an attribute selector uses the sendToPlugin event

{
  "action": "com.example.action1",
  "event": "sendToPlugin",
  "context": uniqueValue,
  "payload": {<json data>}
}

## Events that attribute selectors can trigger

didReceiveSettings Operation persistent data trigger

sendToPropertyInspector Triggered when a plugin uses the sendToPropertyInspector event

{
  "action": "com.example.action1",
  "event": "sendToPropertyInspector",
  "context": uniqueValue,
  "payload": {<json data>}
}

## Events that plugins can send

setSettings Persistently saves data for an action instance

openUrl Opens a URL in the default browser

setTitle Dynamically changes the title of an action instance

setImage Dynamically changes the image displayed by an action instance

showAlert Temporarily displays an alert icon on a key, or flashes a touch screen red via an action instance

showOk Temporarily displays an OK checkmark icon on an image displayed by an action instance

setState Changes the state of an action instance that supports multiple states

sendToPropertyInspector Sends a payload to a property inspector

## Events that a property selector can send

setSettings Persistently saves data for an action instance

openUrl Opens a URL in the default browser

logMessage Writes debug logs to a log file

sendToPlugin Sends a payload to a plugin