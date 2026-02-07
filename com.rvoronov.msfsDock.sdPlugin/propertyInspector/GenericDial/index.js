window.addEventListener("DOMContentLoaded", () => {
    initSimVarField("displayVar", "applyDisplayVar", "displayVar", "vars");
    initSimVarField("feedbackVar", "applyFeedbackVar", "feedbackVar", "vars");
    initSimVarField("incEvent", "applyIncEvent", "incEvent", "events");
    initSimVarField("decEvent", "applyDecEvent", "decEvent", "events");
    initSimVarField("toggleEvent", "applyToggleEvent", "toggleEvent", "events");
});
