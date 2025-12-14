const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        skin: $("#skin"),
        displayVar: $("#displayVar"),
        incEvent: $("#incEvent"),
        decEvent: $("#decEvent"),
        display2Var: $("#display2Var"),
        inc2Event: $("#inc2Event"),
        dec2Event: $("#dec2Event"),
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
            if ($settings.display2Var) {
                $dom.display2Var.value = $settings.display2Var;
            }
            if ($settings.inc2Event) {
                $dom.inc2Event.value = $settings.inc2Event;
            }
            if ($settings.dec2Event) {
                $dom.dec2Event.value = $settings.dec2Event;
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
        incEvent: $dom.incEvent.value,
        decEvent: $dom.decEvent.value,
        display2Var: $dom.display2Var.value,
        inc2Event: $dom.inc2Event.value,
        dec2Event: $dom.dec2Event.value,
    };
    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.skin.on("change", updateSettings);
$dom.displayVar.on("change", updateSettings);
$dom.incEvent.on("change", updateSettings);
$dom.decEvent.on("change", updateSettings);
$dom.display2Var.on("change", updateSettings);
$dom.inc2Event.on("change", updateSettings);
$dom.dec2Event.on("change", updateSettings);

// $propEvent.sendToPropertyInspector = (data) => {
//     console.log("From plugin:", data);
// };
