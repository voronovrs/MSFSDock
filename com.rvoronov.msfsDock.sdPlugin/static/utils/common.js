// CustomEvent Class
class EventPlus {
    constructor() {
        this.event = new EventTarget();
    }
    on(name, callback) {
        this.event.addEventListener(name, e => callback(e.detail));
    }
    send(name, data) {
        this.event.dispatchEvent(new CustomEvent(name, {
            detail: data,
            bubbles: false,
            cancelable: false
        }));
    }
}

// Zero padding
String.prototype.fill = function () {
    return this >= 10 ? this : '0' + this
}

// unicode to string
String.prototype.uTs = function () {
    return eval('"' + Array.from(this).join('') + '"');
};

// String to unicode
String.prototype.sTu = function (str = '') {
    Array.from(this).forEach(item => str += `\\u${item.charCodeAt(0).toString(16)}`);
    return str;
};

// Global variables/methods
const $emit = new EventPlus(), $ = (selector, isAll = false) => {
    const element = document.querySelector(selector), methods = {
        on: function (event, callback) {
            this.addEventListener(event, callback)
        },
        attr: function (name, value = '') {
            value && this.setAttribute(name, value);
            return this;
        }
    }
    if (!isAll && element) {
        return Object.assign(element, methods)
    } else if (!isAll && !element) {
        throw `HTML does not have a ${selector} element! Please check if there is a spelling error`
    }
    return Array.from(document.querySelectorAll(selector)).map(item => Object.assign(item, methods))
}

// Throttle function
$.throttle = (fn, delay) => {
    let Timer = null;
    return function () {
        if (Timer) return;
        Timer = setTimeout(() => {
            fn.apply(this, arguments);
            Timer = null;
        }, delay);
    };
};

// Debounce function
$.debounce = (fn, delay) => {
    let Timer = null;
    return function () {
        clearTimeout(Timer);
        Timer = setTimeout(() => fn.apply(this, arguments), delay);
    };
};

// Limit number
$.num = (selector) => {
    if (!selector.value || /^\d+$/.test(selector.value)) return;
    selector.value = selector.value.slice(0, -1);
    $.num(selector);
};

// Binding Restriction Number Method
Array.from($('input[type="num"]', true)).forEach(item => {
    item.addEventListener('input', () => $.num(item));
});