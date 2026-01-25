class SDPIAutocomplete {
    constructor(inputElement, dataSource, onSelect, minChars = 2) {
        this.input = inputElement;
        this.dataSource = dataSource;
        this.onSelect = onSelect;
        this.minChars = minChars;

        this.draftValue = "";
        this.committing = false;
        this.selecting = false;

        this.wrapper = document.createElement("div");
        this.wrapper.className = "sdpi-autocomplete";

        this.list = document.createElement("div");
        this.list.className = "sdpi-autocomplete-list";

        this.input.parentNode.insertBefore(this.wrapper, this.input);
        this.wrapper.appendChild(this.input);
        this.wrapper.appendChild(this.list);

        this.bind();
    }

    bind() {
        this.input.addEventListener("focus", () => {
            this.draftValue = this.input.value;
            this.update();
        });

        this.input.addEventListener("input", () => {
            this.draftValue = this.input.value;
            this.update();
        });

        this.input.addEventListener("blur", () => {
            if (this.selecting || this.committing) return;

            this.committing = true;
            this.input.value = this.draftValue;
            this.input.dispatchEvent(new Event("change", { bubbles: true }));
            this.committing = false;

            this.hide();
        });

        document.addEventListener("mousedown", e => {
            if (!this.wrapper.contains(e.target)) {
                this.hide();
            }
        });
    }

    update() {
        const value = this.input.value.toLowerCase();
        this.list.innerHTML = "";

        if (!value || value.length < this.minChars) {
            this.hide();
            return;
        }

        const matches = this.dataSource()
            .filter(v => v.toLowerCase().includes(value));

        if (matches.length === 1 && matches[0].toLowerCase() === value) {
            this.hide();
            return;
        }

        matches.forEach(match => {
            const item = document.createElement("div");
            item.className = "sdpi-autocomplete-item";
            item.textContent = match;

            item.addEventListener("mousedown", e => {
                e.preventDefault();

                this.selecting = true;
                this.committing = true;

                this.draftValue = match;
                this.input.value = match;

                this.hide();

                this.input.dispatchEvent(new Event("change", { bubbles: true }));

                this.committing = false;
                this.selecting = false;
            });

            this.list.appendChild(item);
        });

        this.list.style.display = matches.length ? "block" : "none";
    }

    hide() {
        this.list.style.display = "none";
    }
}
