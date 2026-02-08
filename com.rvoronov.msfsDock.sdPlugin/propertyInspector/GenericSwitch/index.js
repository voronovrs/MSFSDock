window.addEventListener("DOMContentLoaded", () => {
    bindTextField("header", "header");
    bindRadioGroup("numPos", "numPos");
    bindTextField("pos0_value", "pos0_value");
    bindTextField("pos0_label", "pos0_label");
    bindTextField("pos1_value", "pos1_value");
    bindTextField("pos1_label", "pos1_label");
    bindTextField("pos2_value", "pos2_value");
    bindTextField("pos2_label", "pos2_label");

    bindSimVarField({
        inputId: "toggleEvent",
        buttonId: "applyEvent",
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
