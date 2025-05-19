const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        type: $("#type"),
        header: $("#header"),
        displayVar: $("#displayVar"),
        incEvent: $("#incEvent"),
        decEvent: $("#decEvent"),
        feedbackVar: $("#feedbackVar"),
        pushEvent: $("#pushEvent"),
        secDisplayVar: $("#secDisplayVar"),
        secIncEvent: $("#secIncEvent"),
        secDecEvent: $("#secDecEvent")
    },
    $propEvent = {
        didReceiveSettings() {
            console.log($settings);
            if ($settings.type) {
                $dom.type.value = $settings.type;
            }
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
            if ($settings.feedbackVar) {
                $dom.feedbackVar.value = $settings.feedbackVar;
            }
            if ($settings.pushEvent) {
                $dom.pushEvent.value = $settings.pushEvent;
            }
            if ($settings.secDisplayVar) {
                $dom.secDisplayVar.value = $settings.secDisplayVar;
            }
            if ($settings.secIncEvent) {
                $dom.secIncEvent.value = $settings.secIncEvent;
            }
            if ($settings.secDecEvent) {
                $dom.secDecEvent.value = $settings.secDecEvent;
            }
        },
        sendToPropertyInspector(data) { }
    };

// Helper to send both values together
function updateSettings() {
    const data = {
        type: $dom.type.value,
        header: $dom.header.value,
        displayVar: $dom.displayVar.value,
        incEvent: $dom.incEvent.value,
        decEvent: $dom.decEvent.value,
        feedbackVar: $dom.feedbackVar.value,
        pushEvent: $dom.pushEvent.value,
        secDisplayVar: $dom.secDisplayVar.value,
        secIncEvent: $dom.secIncEvent.value,
        secDecEvent: $dom.secDecEvent.value
    };
    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.type.on("input", updateSettings);
$dom.header.on("input", updateSettings);
$dom.displayVar.on("input", updateSettings);
$dom.incEvent.on("input", updateSettings);
$dom.decEvent.on("input", updateSettings);
$dom.feedbackVar.on("input", updateSettings);
$dom.pushEvent.on("input", updateSettings);
$dom.secDisplayVar.on("input", updateSettings);
$dom.secIncEvent.on("input", updateSettings);
$dom.secDecEvent.on("input", updateSettings);

$propEvent.sendToPropertyInspector = (data) => {
    console.log("From plugin:", data);
};
