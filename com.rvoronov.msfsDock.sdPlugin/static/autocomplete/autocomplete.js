class SDPIAutocomplete {
    constructor(inputElement, dataSource, onSelect, minChars = 2) {
        this.input = inputElement;
        this.dataSource = dataSource;
        this.onSelect = onSelect;
        this.minChars = minChars;

        this.wrapper = document.createElement("div");
        this.wrapper.className = "sdpi-autocomplete";

        this.list = document.createElement("div");
        this.list.className = "sdpi-autocomplete-list";

        this.input.parentNode.insertBefore(this.wrapper, this.input);
        this.wrapper.appendChild(this.input);
        this.wrapper.appendChild(this.list);

        this.bind();
    }

    onFocus() {
        const value = this.input.value.toLowerCase();
        if (!value || value.length < this.minChars) return;

        const matches = this.dataSource()
            .filter(v => v.toLowerCase().includes(value));

        if (matches.length === 1 && matches[0].toLowerCase() === value) {
            return; // Do not show exact single match
        }

        this.update();
    }

    bind() {
        this.input.addEventListener("focus", () => this.onFocus());
        this.input.addEventListener("input", () => this.update());
        document.addEventListener("click", e => {
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
            .filter(v => v.toLowerCase().includes(value))
//            .slice(0, 20);

        matches.forEach(match => {
            const item = document.createElement("div");
            item.className = "sdpi-autocomplete-item";
            item.textContent = match;

            item.onclick = () => {
                this.input.value = match;
                this.hide();
                if (this.onSelect) this.onSelect(match);
            };

            this.list.appendChild(item);
        });

        this.list.style.display = matches.length ? "block" : "none";
    }

    hide() {
        this.list.style.display = "none";
    }
}
