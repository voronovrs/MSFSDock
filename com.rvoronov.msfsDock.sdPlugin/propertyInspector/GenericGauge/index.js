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

    // --- Zero tick checkbox ---
    const showZeroTick = document.getElementById("showZeroTick");

    showZeroTick.addEventListener("change", () => {
        commitSettings({ showZeroTick: showZeroTick.checked });
    });

    SDPICore.bindings.push({
        apply(settings) {
            showZeroTick.checked = settings.showZeroTick !== false;
        }
    });

    // --- Scale markers ---
    const markersContainer = document.getElementById("markersContainer");
    const addMarkerBtn = document.getElementById("addMarkerBtn");

    function addMarkerRow(position, color) {
        const row = document.createElement("div");
        row.style.cssText = "display:flex; gap:4px; align-items:center; margin-bottom:2px;";

        const posInput = document.createElement("input");
        posInput.type = "number";
        posInput.style.cssText = "width:50px;";
        posInput.value = position !== undefined ? position : 0;
        posInput.addEventListener("change", saveMarkers);

        const colorInput = document.createElement("input");
        colorInput.type = "color";
        colorInput.style.cssText = "width:40px; height:24px;";
        colorInput.value = color || "#ffffff";
        colorInput.addEventListener("change", saveMarkers);

        const removeBtn = document.createElement("button");
        removeBtn.textContent = "\u2715";
        removeBtn.style.cursor = "pointer";
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
