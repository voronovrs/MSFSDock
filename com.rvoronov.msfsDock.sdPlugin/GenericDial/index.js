const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        skin: $("#skin"),
        displayVar: $("#displayVar"),
        incEvent: $("#incEvent"),
        decEvent: $("#decEvent"),
        toggleEvent: $("#toggleEvent"),
        feedbackVar: $("#feedbackVar"),
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
            if ($settings.incEvent) {
                $dom.incEvent.value = $settings.incEvent;
            }
            if ($settings.decEvent) {
                $dom.decEvent.value = $settings.decEvent;
            }
            if ($settings.toggleEvent) {
                $dom.toggleEvent.value = $settings.toggleEvent;
            }
            if ($settings.feedbackVar) {
                $dom.feedbackVar.value = $settings.feedbackVar;
            }
        },
        sendToPropertyInspector(data) { }
    };

// --- Initialize lastSentValue for all fields ---
$dom.header.lastSentValue = $dom.header.value;
$dom.skin.lastSentValue = $dom.skin.value;
$dom.displayVar.lastSentValue = $dom.displayVar.value;
$dom.incEvent.lastSentValue = $dom.incEvent.value;
$dom.decEvent.lastSentValue = $dom.decEvent.value;
$dom.toggleEvent.lastSentValue = $dom.toggleEvent.value;
$dom.feedbackVar.lastSentValue = $dom.feedbackVar.value;

// Helper to send both values together
function updateSettings() {
    const data = {
        header: $dom.header.value,
        skin: $dom.skin.value,
        displayVar: $dom.displayVar.value,
        incEvent: $dom.incEvent.value,
        decEvent: $dom.decEvent.value,
        toggleEvent: $dom.toggleEvent.value,
        feedbackVar: $dom.feedbackVar.value,
    };

    // --- Check if anything really changed ---
    if (
        data.header === $dom.header.lastSentValue &&
        data.skin === $dom.skin.lastSentValue &&
        data.displayVar === $dom.displayVar.lastSentValue &&
        data.incEvent === $dom.incEvent.lastSentValue &&
        data.decEvent === $dom.decEvent.lastSentValue &&
        data.toggleEvent === $dom.toggleEvent.lastSentValue &&
        data.feedbackVar === $dom.feedbackVar.lastSentValue
    ) {
        return; // Nothing changed, skip sending
    }

    $dom.header.lastSentValue = data.header;
    $dom.skin.lastSentValue = data.skin;
    $dom.displayVar.lastSentValue = data.displayVar;
    $dom.incEvent.lastSentValue = data.incEvent;
    $dom.decEvent.lastSentValue = data.decEvent;
    $dom.toggleEvent.lastSentValue = data.toggleEvent;
    $dom.feedbackVar.lastSentValue = data.feedbackVar;

    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.skin.on("change", updateSettings);
$dom.displayVar.on("change", updateSettings);
$dom.incEvent.on("change", updateSettings);
$dom.decEvent.on("change", updateSettings);
$dom.toggleEvent.on("change", updateSettings);
$dom.feedbackVar.on("change", updateSettings);

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
            $dom.toggleEvent,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.feedbackVar,
            () => commonVars,
            updateSettings
        );
    }
};
