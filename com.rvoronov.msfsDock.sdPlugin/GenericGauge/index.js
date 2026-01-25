const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        displayVar: $("#displayVar"),
        dataFormat: $("#dataFormat"),
        minVal: $("#minVal"),
        maxVal: $("#maxVal"),
        style: $("#style"),
        scaleColor: $("#scaleColor"),
        indicatorColor: $("#indicatorColor"),
        bgColor: $("#bgColor"),
    },
    $propEvent = {
        didReceiveSettings() {
            console.log($settings);
            if ($settings.header) {
                $dom.header.value = $settings.header;
            }
            if ($settings.displayVar) {
                $dom.displayVar.value = $settings.displayVar;
            }
            if ($settings.dataFormat) {
                $dom.dataFormat.value = $settings.dataFormat;
            }
            if ($settings.minVal) {
                $dom.minVal.value = $settings.minVal;
            }
            if ($settings.maxVal) {
                $dom.maxVal.value = $settings.maxVal;
            }
            if ($settings.style) {
                $dom.style.value = $settings.style;
            }
            if ($settings.scaleColor) {
                $dom.scaleColor.value = $settings.scaleColor;
            }
            if ($settings.indicatorColor) {
                $dom.indicatorColor.value = $settings.indicatorColor;
            }
            if ($settings.bgColor) {
                $dom.bgColor.value = $settings.bgColor;
            }
        },
        sendToPropertyInspector(data) { }
    };

function toInt(value, def) {
    const v = parseInt(value, 10);
    return Number.isFinite(v) ? v : def;
}

// --- Initialize lastSentValue for all fields ---
$dom.header.lastSentValue = $dom.header.value;
$dom.displayVar.lastSentValue = $dom.displayVar.value;
$dom.dataFormat.lastSentValue = $dom.dataFormat.value;
$dom.minVal.lastSentValue = $dom.minVal.value;
$dom.maxVal.lastSentValue = $dom.maxVal.value;
$dom.style.lastSentValue = $dom.style.value;
$dom.scaleColor.lastSentValue = $dom.scaleColor.value;
$dom.indicatorColor.lastSentValue = $dom.indicatorColor.value;
$dom.bgColor.lastSentValue = $dom.bgColor.value;

// Helper to send both values together
function updateSettings() {
    const data = {
        header: $dom.header.value,
        displayVar: $dom.displayVar.value,
        dataFormat: $dom.dataFormat.value,
        minVal: toInt($dom.minVal.value, 0),
        maxVal: toInt($dom.maxVal.value, 10000),
        style: $dom.style.value,
        scaleColor: $dom.scaleColor.value,
        indicatorColor: $dom.indicatorColor.value,
        bgColor: $dom.bgColor.value,
    };

    // --- Check if anything really changed ---
    if (
        data.header === $dom.header.lastSentValue &&
        data.displayVar === $dom.displayVar.lastSentValue &&
        data.dataFormat === $dom.dataFormat.lastSentValue &&
        data.minVal === $dom.minVal.lastSentValue &&
        data.maxVal === $dom.maxVal.lastSentValue &&
        data.style === $dom.style.lastSentValue &&
        data.scaleColor === $dom.scaleColor.lastSentValue &&
        data.indicatorColor === $dom.indicatorColor.lastSentValue &&
        data.bgColor === $dom.bgColor.lastSentValue
    ) {
        return; // Nothing changed, skip sending
    }

    $dom.header.lastSentValue = data.header;
    $dom.displayVar.lastSentValue = data.displayVar;
    $dom.dataFormat.lastSentValue = data.dataFormat;
    $dom.minVal.lastSentValue = data.minVal;
    $dom.maxVal.lastSentValue = data.maxVal;
    $dom.style.lastSentValue = data.style;
    $dom.scaleColor.lastSentValue = data.scaleColor;
    $dom.indicatorColor.lastSentValue = data.indicatorColor;
    $dom.bgColor.lastSentValue = data.bgColor;

    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.displayVar.on("change", updateSettings);
$dom.dataFormat.on("change", updateSettings);
$dom.minVal.on("change", updateSettings);
$dom.maxVal.on("change", updateSettings);
$dom.style.on("change", updateSettings);
$dom.scaleColor.on("change", updateSettings);
$dom.indicatorColor.on("change", updateSettings);
$dom.bgColor.on("change", updateSettings);

// Autocomplete helper
let commonVars = [];
let autocompleteInitialized = false;

$propEvent.sendToPropertyInspector = (data) => {
    if (data.type === "evt_var_list" && !autocompleteInitialized) {
        autocompleteInitialized = true;
        commonVars = data.common_variables || [];

        new SDPIAutocomplete(
            $dom.displayVar,
            () => commonVars,
            updateSettings
        );
    }
};
