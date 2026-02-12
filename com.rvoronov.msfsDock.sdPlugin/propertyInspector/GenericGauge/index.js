window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindTextField("minVal", "minVal");
    bindTextField("maxVal", "maxVal");

    bindRadioGroup("dataFormat", "dataFormat");
    bindRadioGroup("style", "style");
    bindRadioGroup("skin", "skin");

    bindTextField("scaleColor", "scaleColor");
    bindTextField("indicatorColor", "indicatorColor");
    bindTextField("bgColor", "bgColor");

    bindSimVarField({
        inputId: "displayVar",
        buttonId: "applyDisplayVar",
        settingKey: "displayVar",
        autocompleteSource: "vars"
    });

    // --- Skin visibility toggle ---
    const verticalOptions = document.getElementById("verticalOptions");

    function updateSkinVisibility() {
        const sel = document.querySelector('input[name="skin"]:checked');
        const isVertical = sel && sel.value === "vertical";
        verticalOptions.style.display = isVertical ? "" : "none";
    }

    document.querySelectorAll('input[name="skin"]').forEach(r => {
        r.addEventListener("change", updateSkinVisibility);
    });

    // Update visibility when settings are restored
    SDPICore.bindings.push({
        apply() { setTimeout(updateSkinVisibility, 0); }
    });

    // --- Scale markers ---
    const markersContainer = document.getElementById("markersContainer");
    const addMarkerBtn = document.getElementById("addMarkerBtn");

    function addMarkerRow(position, color) {
        const row = document.createElement("div");
        row.classList.add("position-row");

        const posInput = document.createElement("input");
        posInput.type = "number";
        posInput.value = position !== undefined ? position : 0;
        posInput.classList.add("sdpi-item-value");
        posInput.addEventListener("change", saveMarkers);

        const colorInput = document.createElement("input");
        colorInput.type = "color";
        colorInput.value = color || "#ffffff";
        colorInput.classList.add("sdpi-item-value");
        colorInput.addEventListener("change", saveMarkers);

        const removeBtn = document.createElement("sdpi-button");
        removeBtn.textContent = "\u2715";
        removeBtn.addEventListener("click", () => { row.remove(); saveMarkers(); });

        row.appendChild(posInput);
        row.appendChild(colorInput);
        row.appendChild(removeBtn);
        markersContainer.appendChild(row);
    }

    function collectMarkers() {
        const markers = [];
        markersContainer.querySelectorAll("div").forEach(row => {
            const pos = row.querySelector('input[type="number"]');
            const col = row.querySelector('input[type="color"]');
            if (pos && col) {
                markers.push({ position: parseInt(pos.value) || 0, color: col.value });
            }
        });
        return markers;
    }

    function saveMarkers() {
        commitSettings({ scaleMarkers: collectMarkers() });
    }

    addMarkerBtn.addEventListener("click", () => {
        addMarkerRow(0, "#ffffff");
        saveMarkers();
    });

    // Restore markers from settings
    SDPICore.bindings.push({
        apply(settings) {
            markersContainer.innerHTML = "";
            if (settings.scaleMarkers && Array.isArray(settings.scaleMarkers)) {
                settings.scaleMarkers.forEach(m => addMarkerRow(m.position, m.color));
            }
        }
    });
});
