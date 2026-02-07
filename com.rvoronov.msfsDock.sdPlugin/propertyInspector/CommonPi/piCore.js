window.SDPICore = {
    settings: {}
};

// Get settings from SDPI
SDPIComponents.streamDeckClient.didReceiveSettings.subscribe(event => {
    const settings = event.payload?.settings;
    if (!settings) return;
    SDPICore.settings = settings;
});

// Query vars/events list on PI open
SDPIComponents.streamDeckClient.propertyInspectorDidAppear?.subscribe?.(() => {
    SDPIComponents.streamDeckClient.sendToPlugin({ request: "var_list" });
});
