async function initSimVarField(inputId, buttonId, settingKey, sourceKey) {
    const input = document.getElementById(inputId);
    const button = document.getElementById(buttonId);
    if (!input || !button) return;

    bindApplyButton(inputId, buttonId, settingKey);

    registerAutocomplete(inputId, sourceKey, settingKey);

    SDPIComponents.streamDeckClient.didReceiveSettings.subscribe(event => {
        const settings = event.payload?.settings;
        if (!settings) return;

        // Update only if field is empty
        if (document.activeElement !== input) {
            input.value = settings[settingKey] || "";
        }
    });
}
