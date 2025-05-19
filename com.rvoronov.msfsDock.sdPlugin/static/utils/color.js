// neon array
const $neonColorArr = [
    rgb(255, 0, 0),
    rgb(255, 0, 255),
    rgb(0, 0, 255),
    rgb(0, 255, 255),
    rgb(0, 255, 0),
    rgb(255, 255, 0),
    rgb(255, 0, 0)
]

// Convert RGB to hexadecimal color value
const $rgbToHex = (r, g, b) => {
    return ((1 << 24) | (r << 16) | (g << 8) | b).toString(16).replace('1', '#');
};

// Convert hexadecimal color value to RGB
const $hexToRgb = (hex) => {
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    };
};

// HUE to RGB
const $hueToRgb = (p, q, t) => {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1 / 6) return p + (q - p) * 6 * t;
    if (t < 1 / 2) return q;
    if (t < 2 / 3) return p + (q - p) * (2 / 3 - t) * 6;
    return p;
};

// RGB to HUE
const $rgbToHsl = (rgb) => {
    const r = rgb[0] / 255;
    const g = rgb[1] / 255;
    const b = rgb[2] / 255;
    const max = Math.max(r, g, b);
    const min = Math.min(r, g, b);

    // Calculate brightness saturation
    let saturation = 0;
    const lightness = (max + min) / 2;
    if (max !== min) {
        const delta = max - min;
        saturation = delta / (1 - Math.abs(2 * lightness - 1));
    }

    // Calculate hue
    let hue = 0;
    if (max === r) hue = ((g - b) / (max - min)) % 6;
    else if (max === g) hue = (b - r) / (max - min) + 2;
    else hue = (r - g) / (max - min) + 4; hue *= 60;
    if (hue < 0) hue += 360;
    return [hue, saturation * 100, lightness * 100];
};

// HUE to RGB
const $hslToRgb = (hsl) => {
    const hue = hsl[0] / 360;
    const saturation = hsl[1] / 100;
    const lightness = hsl[2] / 100;
    let r, g, b;
    if (saturation === 0) {
        r = g = b = lightness;
    } else {
        const q = lightness < 0.5 ? lightness * (1 + saturation) : lightness + saturation - lightness * saturation;
        const p = 2 * lightness - q;
        r = $hueToRgb(p, q, hue + 1 / 3);
        g = $hueToRgb(p, q, hue);
        b = $hueToRgb(p, q, hue - 1 / 3);
    }
    return [r * 255, g * 255, b * 255];
};