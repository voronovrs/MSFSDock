window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindRadioGroup("skin", "skin");
    bindRadioGroup("pmdgType", "pmdgType");
    bindRadioGroup("accelMultiplier", "accelMultiplier");

    bindSimVarField({
        inputId: "displayVar",
        buttonId: "applyDisplayVar",
        settingKey: "displayVar",
        autocompleteSource: "vars"
    });

    bindSimVarField({
        inputId: "incEvent",
        buttonId: "applyIncEvent",
        settingKey: "incEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "decEvent",
        buttonId: "applyDecEvent",
        settingKey: "decEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "singleEvent",
        buttonId: "applySingleEvent",
        settingKey: "singleEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "doubleEvent",
        buttonId: "applyDoubleEvent",
        settingKey: "doubleEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "feedbackVar",
        buttonId: "applyFeedbackVar",
        settingKey: "feedbackVar",
        autocompleteSource: "vars"
    });

});
