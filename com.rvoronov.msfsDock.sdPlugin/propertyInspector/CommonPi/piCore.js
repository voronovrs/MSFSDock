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

console.log("piCore.js loaded");

/* -------- core state ---------- */

window.SDPICore = {
    settings: {},
    bindings: []
};

/* -------- commit settings ----- */

window.commitSettings = function (patch) {
    const next = { ...SDPICore.settings, ...patch };
    SDPICore.settings = next;
    SDPIComponents.streamDeckClient.setSettings(next);
};

/* -------- receive settings ---- */

SDPIComponents.streamDeckClient.didReceiveSettings.subscribe(event => {
    const settings = event.payload?.settings;
    if (!settings) return;

    SDPICore.settings = settings;

    SDPICore.bindings.forEach(b => {
        try {
            b.apply(settings);
        } catch (e) {
            console.error("binding apply failed", e);
        }
    });
});

function bindRestore(getter, setter) {
    SDPICore.bindings.push({
        apply(settings) {
            const value = getter(settings);
            if (value === undefined) return;
            setter(value);
        }
    });
}

/* -------- text field ---------- */

window.bindTextField = function (inputId, settingKey) {
    const input = document.getElementById(inputId);
    if (!input) return;

    input.addEventListener("input", () => {
        commitSettings({ [settingKey]: input.value });
    });

    bindRestore(
        s => s[settingKey],
        v => {
            if (document.activeElement !== input) {
                input.value = v ?? "";
            }
        }
    );
};

/* -------- radio group --------- */

window.bindRadioGroup = function (name, settingKey) {
    const radios = [...document.querySelectorAll(`input[name="${name}"]`)];
    if (!radios.length) return;

    radios.forEach(radio => {
        radio.addEventListener("change", () => {
            if (radio.checked) {
                commitSettings({ [settingKey]: radio.value });
            }
        });
    });

    bindRestore(
        s => s[settingKey],
        v => {
            radios.forEach(r => r.checked = r.value === v);
        }
    );
};

/* -------- apply button -------- */

window.bindApplyButton = function (inputId, buttonId, settingKey) {
    const input = document.getElementById(inputId);
    const button = document.getElementById(buttonId);
    if (!input || !button) return;

    const updateButton = () => {
        button.disabled = !input.value.trim();
    };

    updateButton();
    input.addEventListener("input", updateButton);

    input.addEventListener("keydown", e => {
        if (e.key === "Enter" && !button.disabled) {
            button.click();
        }
    });

    button.addEventListener("click", () => {
        const value = input.value.trim();
        if (!value) return;

        commitSettings({ [settingKey]: value });

        const row = button.closest(".position-row");
        if (row) {
            row.classList.add("commit-ok");
            setTimeout(() => row.classList.remove("commit-ok"), 850);
        }
    });
};

/* -------- autocomplete -------- */

const SDPIAutocompleteRegistry = {
    sources: {},
    pending: []
};

window.registerAutocomplete = function (fieldId, sourceKey, settingKey) {
    SDPIAutocompleteRegistry.pending.push({ fieldId, sourceKey, settingKey });
    tryInitAutocompletes();
};

function tryInitAutocompletes() {
    SDPIAutocompleteRegistry.pending = SDPIAutocompleteRegistry.pending.filter(item => {
        const data = SDPIAutocompleteRegistry.sources[item.sourceKey];
        if (!Array.isArray(data) || !data.length) return true;

        const input = document.getElementById(item.fieldId);
        if (!input) return true;

        new SDPIAutocomplete(
            input,
            () => data,
            value => commitSettings({ [item.settingKey]: value })
        );

        return false;
    });
}

/* -------- data from plugin ---- */

SDPIComponents.streamDeckClient.sendToPropertyInspector.subscribe(event => {
    const payload = event.payload;
    if (payload?.type !== "evt_var_list") return;

    if (Array.isArray(payload.common_variables)) {
        SDPIAutocompleteRegistry.sources.vars = payload.common_variables;
    }

    if (Array.isArray(payload.common_events)) {
        SDPIAutocompleteRegistry.sources.events = payload.common_events;
    }

    tryInitAutocompletes();
});

/* -------- PI appear ----------- */

SDPIComponents.streamDeckClient.propertyInspectorDidAppear?.subscribe?.(() => {
    SDPIComponents.streamDeckClient.sendToPlugin({ request: "var_list" });
});

window.bindRestoreOnlyField = function (inputId, settingKey) {
    const input = document.getElementById(inputId);
    if (!input) return;

    bindRestore(
        s => s[settingKey],
        v => {
            if (document.activeElement !== input) {
                input.value = v ?? "";
            }
        }
    );
};

window.bindSimVarField = function ({
    inputId,
    buttonId,
    settingKey,
    autocompleteSource
}) {
    const input = document.getElementById(inputId);
    const button = document.getElementById(buttonId);
    if (!input || !button) return;

    bindApplyButton(inputId, buttonId, settingKey);

    bindRestore(
        s => s[settingKey],
        v => {
            if (document.activeElement !== input) {
                input.value = v ?? "";
            }
        }
    );

    if (autocompleteSource) {
        registerAutocomplete(inputId, autocompleteSource, settingKey);
    }
};
