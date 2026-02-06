const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        skin: $("#skin"),
        toggleEvent: $("#toggleEvent"),
        feedbackVar: $("#feedbackVar"),
        displayVar: $("#displayVar"),
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
            if ($settings.toggleEvent) {
                $dom.toggleEvent.value = $settings.toggleEvent;
            }
            if ($settings.feedbackVar) {
                $dom.feedbackVar.value = $settings.feedbackVar;
            }
            if ($settings.displayVar) {
                $dom.displayVar.value = $settings.displayVar;
            }
        },
        sendToPropertyInspector(data) { }
    };

// --- Initialize lastSentValue for all fields ---
$dom.header.lastSentValue = $dom.header.value;
$dom.skin.lastSentValue = $dom.skin.value;
$dom.toggleEvent.lastSentValue = $dom.toggleEvent.value;
$dom.feedbackVar.lastSentValue = $dom.feedbackVar.value;
$dom.displayVar.lastSentValue = $dom.displayVar.value;

// Helper to send both values together with real-change check
function updateSettings() {
    const data = {
        header: $dom.header.value,
        skin: $dom.skin.value,
        toggleEvent: $dom.toggleEvent.value,
        feedbackVar: $dom.feedbackVar.value,
        displayVar: $dom.displayVar.value,
    };

    // --- Check if anything really changed ---
    if (
        data.header === $dom.header.lastSentValue &&
        data.skin === $dom.skin.lastSentValue &&
        data.toggleEvent === $dom.toggleEvent.lastSentValue &&
        data.feedbackVar === $dom.feedbackVar.lastSentValue &&
        data.displayVar === $dom.displayVar.lastSentValue
    ) {
        return; // Nothing changed, skip sending
    }

    // --- Save current values as last sent ---
    $dom.header.lastSentValue = data.header;
    $dom.skin.lastSentValue = data.skin;
    $dom.toggleEvent.lastSentValue = data.toggleEvent;
    $dom.feedbackVar.lastSentValue = data.feedbackVar;
    $dom.displayVar.lastSentValue = data.displayVar;

    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.skin.on("change", updateSettings);
$dom.toggleEvent.on("change", updateSettings);
$dom.feedbackVar.on("change", updateSettings);
$dom.displayVar.on("change", updateSettings);

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
            $dom.toggleEvent,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.displayVar,
            () => commonVars,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.feedbackVar,
            () => commonVars,
            updateSettings
        );
    }
};
