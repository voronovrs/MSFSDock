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

// $propEvent.sendToPropertyInspector = (data) => {
//     console.log("From plugin:", data);
// };
