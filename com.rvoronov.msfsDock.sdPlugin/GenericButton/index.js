const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        skin: $("#skin"),
        useConditionalEvents: $("#useConditionalEvents"),
        toggleEvent: $("#toggleEvent"),
        conditionalVar: $("#conditionalVar"),
        conditionOperator: $("#conditionOperator"),
        conditionValue: $("#conditionValue"),
        eventWhenTrue: $("#eventWhenTrue"),
        eventWhenFalse: $("#eventWhenFalse"),
        feedbackVar: $("#feedbackVar"),
        displayVar: $("#displayVar"),
        legacySection: $("#legacySection"),
        conditionalSection: $("#conditionalSection"),
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
            if ($settings.useConditionalEvents !== undefined) {
                $dom.useConditionalEvents.value = String($settings.useConditionalEvents);
            }
            if ($settings.toggleEvent) {
                $dom.toggleEvent.value = $settings.toggleEvent;
            }
            if ($settings.conditionalVar) {
                $dom.conditionalVar.value = $settings.conditionalVar;
            }
            if ($settings.conditionOperator) {
                $dom.conditionOperator.value = $settings.conditionOperator;
            }
            if ($settings.conditionValue !== undefined) {
                $dom.conditionValue.value = $settings.conditionValue;
            }
            if ($settings.eventWhenTrue) {
                $dom.eventWhenTrue.value = $settings.eventWhenTrue;
            }
            if ($settings.eventWhenFalse) {
                $dom.eventWhenFalse.value = $settings.eventWhenFalse;
            }
            if ($settings.feedbackVar) {
                $dom.feedbackVar.value = $settings.feedbackVar;
            }
            if ($settings.displayVar) {
                $dom.displayVar.value = $settings.displayVar;
            }
            updateConditionalMode();
        },
        sendToPropertyInspector(data) { }
    };

// --- Initialize lastSentValue for all fields ---
$dom.header.lastSentValue = $dom.header.value;
$dom.skin.lastSentValue = $dom.skin.value;
$dom.useConditionalEvents.lastSentValue = $dom.useConditionalEvents.value;
$dom.toggleEvent.lastSentValue = $dom.toggleEvent.value;
$dom.conditionalVar.lastSentValue = $dom.conditionalVar.value;
$dom.conditionOperator.lastSentValue = $dom.conditionOperator.value;
$dom.conditionValue.lastSentValue = $dom.conditionValue.value;
$dom.eventWhenTrue.lastSentValue = $dom.eventWhenTrue.value;
$dom.eventWhenFalse.lastSentValue = $dom.eventWhenFalse.value;
$dom.feedbackVar.lastSentValue = $dom.feedbackVar.value;
$dom.displayVar.lastSentValue = $dom.displayVar.value;

// Update visibility based on conditional mode
function updateConditionalMode() {
    const useConditional = ($dom.useConditionalEvents.value === 'true');
    
    if (useConditional) {
        $dom.conditionalSection.style.display = 'block';
        $dom.legacySection.style.display = 'none';
    } else {
        $dom.conditionalSection.style.display = 'none';
        $dom.legacySection.style.display = 'block';
    }
}

// Helper to send both values together with real-change check
function updateSettings() {
    const data = {
        header: $dom.header.value,
        skin: $dom.skin.value,
        useConditionalEvents: ($dom.useConditionalEvents.value === 'true'),
        toggleEvent: $dom.toggleEvent.value,
        conditionalVar: $dom.conditionalVar.value,
        conditionOperator: $dom.conditionOperator.value,
        conditionValue: parseFloat($dom.conditionValue.value) || 0,
        eventWhenTrue: $dom.eventWhenTrue.value,
        eventWhenFalse: $dom.eventWhenFalse.value,
        feedbackVar: $dom.feedbackVar.value,
        displayVar: $dom.displayVar.value,
    };

    // --- Check if anything really changed ---
    if (
        data.header === $dom.header.lastSentValue &&
        data.skin === $dom.skin.lastSentValue &&
        String(data.useConditionalEvents) === $dom.useConditionalEvents.lastSentValue &&
        data.toggleEvent === $dom.toggleEvent.lastSentValue &&
        data.conditionalVar === $dom.conditionalVar.lastSentValue &&
        data.conditionOperator === $dom.conditionOperator.lastSentValue &&
        data.conditionValue === $dom.conditionValue.lastSentValue &&
        data.eventWhenTrue === $dom.eventWhenTrue.lastSentValue &&
        data.eventWhenFalse === $dom.eventWhenFalse.lastSentValue &&
        data.feedbackVar === $dom.feedbackVar.lastSentValue &&
        data.displayVar === $dom.displayVar.lastSentValue
    ) {
        return; // Nothing changed, skip sending
    }

    // --- Save current values as last sent ---
    $dom.header.lastSentValue = data.header;
    $dom.skin.lastSentValue = data.skin;
    $dom.useConditionalEvents.lastSentValue = String(data.useConditionalEvents);
    $dom.toggleEvent.lastSentValue = data.toggleEvent;
    $dom.conditionalVar.lastSentValue = data.conditionalVar;
    $dom.conditionOperator.lastSentValue = data.conditionOperator;
    $dom.conditionValue.lastSentValue = data.conditionValue;
    $dom.eventWhenTrue.lastSentValue = data.eventWhenTrue;
    $dom.eventWhenFalse.lastSentValue = data.eventWhenFalse;
    $dom.feedbackVar.lastSentValue = data.feedbackVar;
    $dom.displayVar.lastSentValue = data.displayVar;

    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.skin.on("change", updateSettings);
$dom.useConditionalEvents.on("change", () => {
    updateConditionalMode();
    updateSettings();
});
$dom.toggleEvent.on("change", updateSettings);
$dom.conditionalVar.on("change", updateSettings);
$dom.conditionOperator.on("change", updateSettings);
$dom.conditionValue.on("input", updateSettings);
$dom.eventWhenTrue.on("change", updateSettings);
$dom.eventWhenFalse.on("change", updateSettings);
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
            $dom.conditionalVar,
            () => commonVars,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.eventWhenTrue,
            () => commonEvents,
            updateSettings
        );

        new SDPIAutocomplete(
            $dom.eventWhenFalse,
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
