window.addEventListener("DOMContentLoaded", () => {
    bindTextField("type", "type");

    bindTextField("header", "header");
    bindTextField("header2", "header2");

    bindRadioGroup("varFormat", "varFormat");
    bindRadioGroup("varFormat2", "varFormat2");

    bindTextField("bgColor", "bgColor");
    bindTextField("outlineColor", "outlineColor");
    bindTextField("headerColor", "headerColor");
    bindTextField("dataColor", "dataColor");

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

    // Update visibility when settings are restored
    SDPICore.bindings.push({
        apply() { setTimeout(updateInfoVisibility, 0); }
    });
});
