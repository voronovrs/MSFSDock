/**
 * PropertyInspector 2.5.0 new features =>
 *
 * 1 => Tool is separated from the main file - import on demand
 * 2 => $settings - global persistent data proxy ※
 * 3 => No need to pay attention to context - communicate with plugins anytime, anywhere
 * 4 => Note: To avoid naming conflicts, do not use $ related names and JQuery libraries
 *
 * ===== CJHONG ========================================== 2023.10.10 =====>
 */

let $websocket, $uuid, $action, $context, $settings, $lang;

// Send to plugin
WebSocket.prototype.sendToPlugin = function (payload) {
    this.send(JSON.stringify({
        event: "sendToPlugin",
        action: $action,
        context: $uuid,
        payload
    }));
}

// Set status
WebSocket.prototype.setState = function (state) {
    console.log('set state', state)
    this.send(JSON.stringify({
        event: "setState",
        context: $context,
        payload: { state }
    }));
}

// Set background image
WebSocket.prototype.setImage = function (url) {
    console.log('set image', url)
    let image = new Image();
    image.src = url;
    image.onload = () => {
        let canvas = document.createElement("canvas");
        canvas.width = image.naturalWidth;
        canvas.height = image.naturalHeight;
        let ctx = canvas.getContext("2d");
        ctx.drawImage(image, 0, 0);
        this.send(JSON.stringify({
            event: "setImage",
            context: $context,
            payload: {
                target: 0,
                image: canvas.toDataURL("image/png")
            }
        }));
    };
}

// Open URL
WebSocket.prototype.openUrl = function (url) {
    this.send(JSON.stringify({
        event: "openUrl",
        payload: { url }
    }));
}

// Save data
WebSocket.prototype.saveData = $.debounce(function (payload) {
    this.send(JSON.stringify({
        event: "setSettings",
        context: $uuid,
        payload
    }))
}, 0)

//In order to be compatible with Elgato, we use the same function names as them so that the plugin you develop can be compatible with Elgato's software
async function connectElgatoStreamDeckSocket(port, uuid, event, app, info) {
    info = JSON.parse(info);
    $uuid = uuid; $action = info.action; $context = info.context;
    $websocket = new WebSocket('ws://127.0.0.1:' + port);
    $websocket.onopen = () => $websocket.send(JSON.stringify({ event, uuid }));

    // Persistent Data Broker
    $websocket.onmessage = e => {
        let data = JSON.parse(e.data);
        console.log('data', data)
        if (data.event === 'didReceiveSettings') {
            $settings = new Proxy(data.payload.settings, {
                get(target, property) {
                    return target[property];
                },
                set(target, property, value) {
                    target[property] = value;
                    $websocket.saveData(data.payload.settings);
                }
            });
            if (!$back) $dom.main.style.display = 'block';
        }
        $propEvent[data.event]?.(data.payload);
    };

    // Automatically translate pages
    if (!$local) return;
    $lang = await new Promise(resolve => {
        const req = new XMLHttpRequest();
        req.open('GET', `../${JSON.parse(app).application.language}.json`);
        req.send();
        req.onreadystatechange = () => {
            if (req.readyState === 4) {
                resolve(JSON.parse(req.responseText).Localization)
            }
        };
    })

    // Iterate over the text nodes and translate all of them
    const walker = document.createTreeWalker($dom.main, NodeFilter.SHOW_TEXT, (e) => {
        return e.data.trim() && NodeFilter.FILTER_ACCEPT
    });
    while (walker.nextNode()) {
        console.log(walker.currentNode.data);
        walker.currentNode.data = $lang[walker.currentNode.data]
    }
    // placeholder special handling
    const translate = item => {
        if (item.placeholder?.trim()) {
            console.log(item.placeholder);
            item.placeholder = $lang[item.placeholder]
        }
    }
    $('input', true).forEach(translate)
    $('textarea', true).forEach(translate)
}

// StreamDock File path callback
let $FileID = ''; Array.from($('input[type="file"]', true)).forEach(item => {
    item.addEventListener('click', () => $FileID = item.id);
});
const onFilePickerReturn = (url) => $emit.send(`File-${$FileID}`, JSON.parse(url));