window.addEventListener("DOMContentLoaded", () => {
    bindTextField("type", "type");

    bindTextField("header", "header");
    bindTextField("header2", "header2");

    bindRadioGroup("varFormat", "varFormat");
    bindRadioGroup("varFormat2", "varFormat2");

    bindTextField("scaleFactor1", "scaleFactor1");
    bindTextField("precision1", "precision1");
    bindTextField("scaleFactor2", "scaleFactor2");
    bindTextField("precision2", "precision2");

    bindTextField("bgColor", "bgColor");
    bindTextField("outlineColor", "outlineColor");
    bindTextField("headerColor", "headerColor");
    bindTextField("dataColor", "dataColor");

    bindSimVarField({
        inputId: "toggleEvent",
        buttonId: "applyEvent",
        settingKey: "toggleEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "infoVar",
        buttonId: "applyInfoVar",
        settingKey: "infoVar",
        autocompleteSource: "vars"
    });

    bindSimVarField({
        inputId: "infoVar2",
        buttonId: "applyInfoVar2",
        settingKey: "infoVar2",
        autocompleteSource: "vars"
    });

    // --- Info variables visibility toggle ---
    const typeSelect = document.getElementById("type");
    const infoVars = document.getElementById("infoVars");

    function updateInfoVisibility() {
        const isInfo = typeSelect && typeSelect.value === "info";
        infoVars.style.display = isInfo ? "" : "none";
    }

    typeSelect.addEventListener("change", updateInfoVisibility);

    // --- Scale variables visibility toggle ---
    const scaleSettings1 = document.getElementById("scaleSettings1");
    const scaleSettings2 = document.getElementById("scaleSettings2");

    function updateScale1Visibility() {
        const selectedFormat = document.querySelector('input[name="varFormat"]:checked');

        if (selectedFormat) {
            const isFractional = selectedFormat.value === "fractional";
            scaleSettings1.style.display = isFractional ? "" : "none";
        }
    }

    function updateScale2Visibility() {
        const selectedFormat = document.querySelector('input[name="varFormat2"]:checked');

        if (selectedFormat) {
            const isFractional = selectedFormat.value === "fractional";
            scaleSettings2.style.display = isFractional ? "" : "none";
        }
    }

    document.querySelectorAll('input[name="varFormat"]').forEach(radio => {
        radio.addEventListener("change", updateScale1Visibility);
    });
    document.querySelectorAll('input[name="varFormat2"]').forEach(radio => {
        radio.addEventListener("change", updateScale2Visibility);
    });

    // Update visibility when settings are restored
    SDPICore.bindings.push({
        apply() { setTimeout(updateInfoVisibility, 0);
            setTimeout(updateScale1Visibility, 0);
            setTimeout(updateScale2Visibility, 0);
        }
    });
});
