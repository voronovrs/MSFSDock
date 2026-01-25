const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        skin: $("#skin"),
        displayVar: $("#displayVar"),
        display2Var: $("#display2Var"),
        incEvent: $("#incEvent"),
        decEvent: $("#decEvent"),
        inc2Event: $("#inc2Event"),
        dec2Event: $("#dec2Event"),
        toggleEvent: $("#toggleEvent"),
    },
    $propEvent = {
        didReceiveSettings() {
            console.log($settings);
            if ($settings.header) {
                $dom.header.value = $settings.header;
            }
            if ($settings.skin) {
                $dom.skin.value = $settings.skin;
            }
            if ($settings.displayVar) {
                $dom.displayVar.value = $settings.displayVar;
            }
            if ($settings.display2Var) {
                $dom.display2Var.value = $settings.display2Var;
            }
            if ($settings.incEvent) {
                $dom.incEvent.value = $settings.incEvent;
            }
            if ($settings.decEvent) {
                $dom.decEvent.value = $settings.decEvent;
            }
            if ($settings.inc2Event) {
                $dom.inc2Event.value = $settings.inc2Event;
            }
            if ($settings.dec2Event) {
                $dom.dec2Event.value = $settings.dec2Event;
            }
            if ($settings.toggleEvent) {
                $dom.toggleEvent.value = $settings.toggleEvent;
            }
        },
        sendToPropertyInspector(data) { }
    };

// --- Initialize lastSentValue for all fields ---
$dom.header.lastSentValue = $dom.header.value;
$dom.skin.lastSentValue = $dom.skin.value;
$dom.displayVar.lastSentValue = $dom.displayVar.value;
$dom.display2Var.lastSentValue = $dom.display2Var.value;
$dom.incEvent.lastSentValue = $dom.incEvent.value;
$dom.decEvent.lastSentValue = $dom.decEvent.value;
$dom.inc2Event.lastSentValue = $dom.inc2Event.value;
$dom.dec2Event.lastSentValue = $dom.dec2Event.value;
$dom.toggleEvent.lastSentValue = $dom.display2Var.value;

// Helper to send both values together
function updateSettings() {
    const data = {
        header: $dom.header.value,
        skin: $dom.skin.value,
        displayVar: $dom.displayVar.value,
        display2Var: $dom.display2Var.value,
        incEvent: $dom.incEvent.value,
        decEvent: $dom.decEvent.value,
        inc2Event: $dom.inc2Event.value,
        dec2Event: $dom.dec2Event.value,
        toggleEvent: $dom.toggleEvent.value,
    };

    // --- Check if anything really changed ---
    if (
        data.header === $dom.header.lastSentValue &&
        data.skin === $dom.skin.lastSentValue &&
        data.displayVar === $dom.displayVar.lastSentValue &&
        data.display2Var === $dom.display2Var.lastSentValue &&
        data.incEvent === $dom.incEvent.lastSentValue &&
        data.decEvent === $dom.decEvent.lastSentValue &&
        data.inc2Event === $dom.inc2Event.lastSentValue &&
        data.dec2Event === $dom.dec2Event.lastSentValue &&
        data.toggleEvent === $dom.toggleEvent.lastSentValue
    ) {
        return; // Nothing changed, skip sending
    }

    $dom.header.lastSentValue = data.header;
    $dom.skin.lastSentValue = data.skin;
    $dom.displayVar.lastSentValue = data.displayVar;
    $dom.display2Var.lastSentValue = data.display2Var;
    $dom.incEvent.lastSentValue = data.incEvent;
    $dom.decEvent.lastSentValue = data.decEvent;
    $dom.inc2Event.lastSentValue = data.inc2Event;
    $dom.dec2Event.lastSentValue = data.dec2Event;
    $dom.toggleEvent.lastSentValue = data.toggleEvent;

    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.skin.on("change", updateSettings);
$dom.displayVar.on("change", updateSettings);
$dom.display2Var.on("change", updateSettings);
$dom.incEvent.on("change", updateSettings);
$dom.decEvent.on("change", updateSettings);
$dom.inc2Event.on("change", updateSettings);
$dom.dec2Event.on("change", updateSettings);
$dom.toggleEvent.on("change", updateSettings);

// Autocomplete helper
let commonEvents = [];
let commonVars = [];
let autocompleteInitialized = false;

$propEvent.sendToPropertyInspector = (data) => {
    if (data.type === "evt_var_list" && !autocompleteInitialized) {
        autocompleteInitialized = true;
        commonEvents = data.common_events || [];
        commonVars = data.common_variables || [];

        new SDPIAutocomplete(
            $dom.displayVar,
            () => commonVars,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.incEvent,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.decEvent,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.display2Var,
            () => commonVars,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.inc2Event,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.dec2Event,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.toggleEvent,
            () => commonEvents,
            updateSettings
        );
    }
};
