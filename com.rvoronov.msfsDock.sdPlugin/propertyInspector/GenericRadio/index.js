window.addEventListener("DOMContentLoaded", () => {
    initSimVarField("displayVar", "applyDisplayVar", "displayVar", "vars");
    initSimVarField("display2Var", "applyDisplay2Var", "display2Var", "vars");
    initSimVarField("incEvent", "applyIncEvent", "incEvent", "events");
    initSimVarField("decEvent", "applyDecEvent", "decEvent", "events");
    initSimVarField("inc2Event", "applyInc2Event", "inc2Event", "events");
    initSimVarField("dec2Event", "applyDec2Event", "dec2Event", "events");
    initSimVarField("toggleEvent", "applyToggleEvent", "toggleEvent", "events");
});
