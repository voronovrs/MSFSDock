const SDPIAutocompleteRegistry = {
    sources: {},
    pending: []
};

function registerAutocomplete(fieldId, sourceKey, settingKey) {
    SDPIAutocompleteRegistry.pending.push({ fieldId, sourceKey, settingKey });
    tryInitAutocompletes();
}

function tryInitAutocompletes() {
    SDPIAutocompleteRegistry.pending = SDPIAutocompleteRegistry.pending.filter(item => {
        const data = SDPIAutocompleteRegistry.sources[item.sourceKey];
        if (!Array.isArray(data) || data.length === 0) return true;

        const input = document.getElementById(item.fieldId);
        if (!input) return true;

        new SDPIAutocomplete(
            input,
            () => data,
            value => {
                const oldSettings = SDPICore.settings || {};
                const newSettings = { ...oldSettings, [item.settingKey]: value };
                SDPIComponents.streamDeckClient.setSettings(newSettings);
            }
        );

        return false;
    });
}

// Data from plugin
SDPIComponents.streamDeckClient.sendToPropertyInspector.subscribe(event => {
    const payload = event.payload;
    if (payload.type !== "evt_var_list") return;

    if (Array.isArray(payload.common_variables)) {
        SDPIAutocompleteRegistry.sources.vars = payload.common_variables;
    }

    if (Array.isArray(payload.common_events)) {
        SDPIAutocompleteRegistry.sources.events = payload.common_events;
    }

    tryInitAutocompletes();
});
