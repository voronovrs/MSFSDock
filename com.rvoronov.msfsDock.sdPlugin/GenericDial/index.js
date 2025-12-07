const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        displayVar: $("#displayVar"),
        incEvent: $("#incEvent"),
        decEvent: $("#decEvent"),
        toggleEvent: $("#toggleEvent"),
        feedbackVar: $("#feedbackVar"),
        skin: $("#skin"),
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
            if ($settings.skin) {
                $dom.skin.value = $settings.skin;
            }
        },
        sendToPropertyInspector(data) { }
    };

// Helper to send both values together
function updateSettings() {
    const data = {
        header: $dom.header.value,
        displayVar: $dom.displayVar.value,
        incEvent: $dom.incEvent.value,
        decEvent: $dom.decEvent.value,
        toggleEvent: $dom.toggleEvent.value,
        feedbackVar: $dom.feedbackVar.value,
        skin: $dom.skin.value,
    };
    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.displayVar.on("change", updateSettings);
$dom.incEvent.on("change", updateSettings);
$dom.decEvent.on("change", updateSettings);
$dom.toggleEvent.on("change", updateSettings);
$dom.feedbackVar.on("change", updateSettings);
$dom.skin.on("change", updateSettings);

// $propEvent.sendToPropertyInspector = (data) => {
//     console.log("From plugin:", data);
// };
