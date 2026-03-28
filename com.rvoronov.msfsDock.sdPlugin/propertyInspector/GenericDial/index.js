window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindRadioGroup("skin", "skin");
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
        inputId: "toggleEvent",
        buttonId: "applyToggleEvent",
        settingKey: "toggleEvent",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "feedbackVar",
        buttonId: "applyFeedbackVar",
        settingKey: "feedbackVar",
        autocompleteSource: "vars"
    });

});
