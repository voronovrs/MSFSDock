function bindApplyButton(inputId, buttonId, settingKey) {
    const input = document.getElementById(inputId);
    const button = document.getElementById(buttonId);
    if (!input || !button) return;

    button.disabled = !input.value.trim();

    input.addEventListener("input", () => {
        button.disabled = !input.value.trim();
    });

    input.addEventListener("keydown", e => {
        if (e.key === "Enter" && !button.disabled) button.click();
    });

    button.addEventListener("click", async () => {
        const value = input.value.trim();
        if (!value) return;

        const prevSettings = SDPICore.settings || {};
        const nextSettings = { ...prevSettings, [settingKey]: value };
        console.log("new settings ", nextSettings);

        SDPICore.settings = nextSettings;

        SDPIComponents.streamDeckClient.setSettings(nextSettings);

        // visual feedback
        const row = button.closest(".position-row");
        if (row) {
            row.classList.add("commit-ok");
            setTimeout(() => row.classList.remove("commit-ok"), 850);
        }

    });
}
