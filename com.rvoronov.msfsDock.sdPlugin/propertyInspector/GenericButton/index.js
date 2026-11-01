window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindTextField("scaleFactor", "scaleFactor");
    bindTextField("precision", "precision");
    bindRadioGroup("skin", "skin");
    bindRadioGroup("pmdgType", "pmdgType");
    bindRadioGroup("buttonType", "buttonType");
    bindRadioGroup("varFormat", "varFormat");

    bindSimVarField({
        inputId: "toggleEvent",
        buttonId: "applyEvent",
        settingKey: "toggleEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "toggleReleaseEvent",
        buttonId: "applyReleaseEvent",
        settingKey: "toggleReleaseEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "displayVar",
        buttonId: "applyDisplayVar",
        settingKey: "displayVar",
        autocompleteSource: "vars"
    });

    bindSimVarField({
        inputId: "feedbackVar",
        buttonId: "applyFeedbackVar",
        settingKey: "feedbackVar",
        autocompleteSource: "vars"
    });

    // --- Release settings visibility toggle ---
    const buttonReleaseSettings = document.getElementById("buttonReleaseSettings");

    function updateReleaseSettingsVisibility() {
        const sel = document.querySelector('input[name="buttonType"]:checked');
        const isMomentary = sel && sel.value === "momentary";
        buttonReleaseSettings.style.display = isMomentary ? "" : "none";
    }

    document.querySelectorAll('input[name="buttonType"]').forEach(r => {
        r.addEventListener("change", updateReleaseSettingsVisibility);
    });

    // --- Scale variables visibility toggle ---
    const scaleSettings = document.getElementById("scaleSettings");

    function updateScaleVisibility() {
        const selectedFormat = document.querySelector('input[name="varFormat"]:checked');

        if (selectedFormat) {
            const isFractional = selectedFormat.value === "fractional";
            scaleSettings.style.display = isFractional ? "" : "none";
        }
    }

    document.querySelectorAll('input[name="varFormat"]').forEach(radio => {
        radio.addEventListener("change", updateScaleVisibility);
    });

    // Update visibility when settings are restored
    SDPICore.bindings.push({
        apply() { setTimeout(updateReleaseSettingsVisibility, 0);
            setTimeout(updateScaleVisibility, 0);
        }
    });
});
