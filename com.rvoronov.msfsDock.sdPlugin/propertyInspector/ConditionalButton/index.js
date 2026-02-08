window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindTextField("conditionOperator", "conditionOperator");
    bindTextField("conditionValue", "conditionValue");
    bindRadioGroup("skin", "skin");

    bindSimVarField({
        inputId: "eventWhenTrue",
        buttonId: "applyTrueEvent",
        settingKey: "eventWhenTrue",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "eventWhenFalse",
        buttonId: "applyFalseEvent",
        settingKey: "eventWhenFalse",
        autocompleteSource: "events"
    });

    bindSimVarField({
        inputId: "conditionalVar",
        buttonId: "applyConditionalVar",
        settingKey: "conditionalVar",
        autocompleteSource: "vars"
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

});
