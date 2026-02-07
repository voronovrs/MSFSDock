window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindTextField("minVal", "minVal");
    bindTextField("maxVal", "maxVal");

    bindRadioGroup("dataFormat", "dataFormat");
    bindRadioGroup("style", "style");

    bindTextField("scaleColor", "scaleColor");
    bindTextField("indicatorColor", "indicatorColor");
    bindTextField("bgColor", "bgColor");

    bindSimVarField({
        inputId: "displayVar",
        buttonId: "applyDisplayVar",
        settingKey: "displayVar",
        autocompleteSource: "vars"
    });

});
