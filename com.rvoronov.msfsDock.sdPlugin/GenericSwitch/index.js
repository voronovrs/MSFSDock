const $local = false, $back = false,
    $dom = {
        main: $('.sdpi-wrapper'),
        header: $("#header"),
        numPos: $("#numPos"),
        toggleEvent: $("#toggleEvent"),
        feedbackVar: $("#feedbackVar"),
        positions: $("#positions"),
    },
    $propEvent = {
        didReceiveSettings() {
            console.log($settings);
            if ($settings.header) {
                $dom.header.value = $settings.header;
            }
            if ($settings.numPos) {
                $dom.numPos.value = $settings.numPos;
            }
            rebuildPositionRows(parseInt($dom.numPos.value, 10),$settings.positions || []);
            if ($settings.toggleEvent) {
                $dom.toggleEvent.value = $settings.toggleEvent;
            }
            if ($settings.feedbackVar) {
                $dom.feedbackVar.value = $settings.feedbackVar;
            }
            if (!$settings || Object.keys($settings).length === 0) {
                rebuildPositionRows(parseInt($dom.numPos.value, 10));
                updateSettings();
            }
        },
        sendToPropertyInspector(data) { }
    };

// --- Initialize lastSentValue for all fields ---
$dom.header.lastSentValue = $dom.header.value;
$dom.numPos.lastSentValue = $dom.numPos.value;
$dom.toggleEvent.lastSentValue = $dom.toggleEvent.value;
$dom.feedbackVar.lastSentValue = $dom.feedbackVar.value;
$dom.positionsLastSent = "[]";

function collectPositions() {
  const rows = positionsContainer.querySelectorAll(".position-row");
  const positions = [];

  rows.forEach(row => {
    const value = row.querySelector(".pos-value").value;
    const label = row.querySelector(".pos-label").value;
    positions.push({ value, label });
  });

  return positions;
}

// Helper to send both values together with real-change check
function updateSettings() {
    const data = {
        header: $dom.header.value,
        numPos: $dom.numPos.value,
        toggleEvent: $dom.toggleEvent.value,
        feedbackVar: $dom.feedbackVar.value,
        positions: collectPositions()
    };

    if (
        data.header === $dom.header.lastSentValue &&
        data.numPos === $dom.numPos.lastSentValue &&
        data.toggleEvent === $dom.toggleEvent.lastSentValue &&
        data.feedbackVar === $dom.feedbackVar.lastSentValue &&
        JSON.stringify(data.positions) === $dom.positionsLastSent
    ) {
        return;
    }

    $dom.header.lastSentValue = data.header;
    $dom.numPos.lastSentValue = data.numPos;
    $dom.toggleEvent.lastSentValue = data.toggleEvent;
    $dom.feedbackVar.lastSentValue = data.feedbackVar;
    $dom.positionsLastSent = JSON.stringify(data.positions);

    $websocket.saveData(data);
}

// Listen to input events and send full payload
$dom.header.on("input", updateSettings);
$dom.numPos.on("change", updateSettings);
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

const numPosSelect = document.getElementById("numPos");
const positionsContainer = document.getElementById("positions");
const rowTemplate = document.getElementById("positionRowTemplate");

function rebuildPositionRows(count, existingPositions = []) {
  positionsContainer.innerHTML = "";

  for (let i = 0; i < count; i++) {
    const row = rowTemplate.content.cloneNode(true);

    const valueInput = row.querySelector(".pos-value");
    const labelInput = row.querySelector(".pos-label");

    if (existingPositions[i]) {
      valueInput.value = existingPositions[i].value;
      labelInput.value = existingPositions[i].label;
    } else {
      valueInput.value = i.toString();
      labelInput.value = `POS ${i}`;
    }

    valueInput.addEventListener("input", updateSettings);
    labelInput.addEventListener("input", updateSettings);

    positionsContainer.appendChild(row);
  }
}

numPosSelect.addEventListener("change", () => {
  rebuildPositionRows(parseInt(numPosSelect.value, 10));
  updateSettings();
});
