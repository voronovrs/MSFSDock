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

// $propEvent.sendToPropertyInspector = (data) => {
//     console.log("From plugin:", data);
// };
