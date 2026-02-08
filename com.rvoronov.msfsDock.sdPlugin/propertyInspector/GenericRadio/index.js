window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindRadioGroup("skin", "skin");

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
        inputId: "display2Var",
        buttonId: "applyDisplay2Var",
        settingKey: "display2Var",
        autocompleteSource: "vars"
    });

    bindSimVarField({
        inputId: "inc2Event",
        buttonId: "applyInc2Event",
        settingKey: "inc2Event",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "dec2Event",
        buttonId: "applyDec2Event",
        settingKey: "dec2Event",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "toggleEvent",
        buttonId: "applyToggleEvent",
        settingKey: "toggleEvent",
        autocompleteSource: "events"
    });

});
