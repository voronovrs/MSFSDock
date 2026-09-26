window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
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

    // Update visibility when settings are restored
    SDPICore.bindings.push({
        apply() { setTimeout(updateReleaseSettingsVisibility, 0); }
    });
});
