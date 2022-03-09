const fs = require('fs');
const { registerFont, createCanvas } = require('canvas');

registerFont('simplepixel/simplepixel.ttf', { family: 'SimplePixel' });
registerFont('simplepixel-bold/simplepixel-bold.ttf', { family: 'SimplePixel-Bold' });

const size = 32;
const offset = 16;

const canvas = createCanvas(size, size);
const ctx = canvas.getContext('2d');
const { fonts, metrics } = renderAllGlyphs();
const code = generateFontCode(fonts, metrics);

fs.writeFileSync('fonts.c', code.c);
fs.writeFileSync('fonts.h', code.h);

function renderAllGlyphs() {
	const fonts = new Map([
		['regular', []],
		['bold', []],
		['kana', []],
	]);

	for (let c = 0x20; c <= 0x7E; ++c) {
		fonts.get('regular')[c-0x20] = renderGlyph(c, '9px SimplePixel');
	}

	for (let c = 0x20; c <= 0x7E; ++c) {
		fonts.get('bold')[c-0x20] = renderGlyph(c, '9px SimplePixel-Bold');
	}

	fonts.get('kana')[0] = renderGlyph(0x3000, '9px SimplePixel')
	for (let c = 0x30A1; c <= 0x30FC; ++c) {
		fonts.get('kana')[c-0x30A0] = renderGlyph(c, '9px SimplePixel');
	}

	const allGlyphs = [].concat(...fonts.values());
	const allTop = Math.min(...allGlyphs.map(g => g.metrics.top));
	const allBottom = Math.max(...allGlyphs.map(g => g.metrics.bottom));

	const baseline = offset - allTop;
	const height = allBottom - allTop + 1;

	for (let glyph of allGlyphs) {
		glyph.image = cropImage(glyph.image, glyph.metrics.left, allTop, glyph.metrics.right, allBottom);
		glyph.image = encodeImage(glyph.image);
	}

	return { fonts, metrics: { baseline, height }};
}

function renderGlyph(codepoint, font) {
	const image = getImageData(String.fromCodePoint(codepoint), font);
	const left = findGlyphLeft(image);
	const top = findGlyphTop(image);
	const bottom = findGlyphBottom(image);

	const image2 = getImageData(String.fromCodePoint(codepoint) + String.fromCodePoint(0x1000), font);
	const right = findGlyphRight(image2);

	return {
		c: String.fromCodePoint(codepoint),
		metrics: { left, top, right, bottom },
		image,
	};
}

function getImageData(text, font) {
	ctx.font = font;
	ctx.clearRect(0, 0, size, size);
	ctx.fillText(text, offset, offset);
	const imageData = ctx.getImageData(0, 0, size, size);
	const result = [];
	for (let y = 0; y < size; ++y) {
		result[y] = [];
		for (let x = 0; x < size; ++x) {
			result[y][x] = imageData.data[(size*y + x)*4 + 3];
		}
	}
	return result;
}

function findGlyphLeft(image) {
	for (let x = 0; x < size; ++x) {
		for (let y = 0; y < size; ++y) {
			if (image[y][x] > 0) {
				return x;
			}
		}
	}
	return offset;
}

function findGlyphTop(image) {
	for (let y = 0; y < size; ++y) {
		for (let x = 0; x < size; ++x) {
			if (image[y][x] > 0) {
				return y;
			}
		}
	}
	return offset;
}

function findGlyphBottom(image) {
	for (let y = size-1; y >= 0; --y) {
		for (let x = 0; x < size; ++x) {
			if (image[y][x] > 0) {
				return y;
			}
		}
	}
	return offset;
}

function findGlyphRight(image2) {
	let x, y;
	outer: for (x = size-1; x >= 0; --x) {
		for (y = size-1; y >= 0; --y) {
			if (image2[y][x] > 0) {
				break outer;
			}
		}
	}
	for (; x >= 0; --x) {
		if (image2[y][x] === 0) {
			return x;
		}
	}
}

function cropImage(image, left, top, right, bottom) {
	return image
		.filter((_, i) => i >= top && i <= bottom)
		.map(row => row.filter((_, i) => i >= left && i <= right));
}

function encodeImage(image) {
	const result = [];
	for (let x = 0; x < image[0].length; ++x) {
		let value = 0;
		for (let y = 0; y < image.length; ++y) {
			if (image[y][x]) {
				value = value | (1<<y);
			}
		}
		result.push(value);
	}
	return result;
}

function generateFontCode(fonts, metrics) {
	let out_h = '#pragma once\n\n';
	out_h += '#include "../defs.h"\n\n';
	out_h += `#define FONT_HEIGHT ${metrics.height}\n`;
	out_h += `#define FONT_BASELINE ${metrics.baseline}\n\n`;

	let out_c = '#include "fonts.h"\n';

	for (let [font, glyphs] of fonts.entries()) {
		const data = [];
		const offsets = [0];
		let offset = 0;

		out_h += `extern const PROGMEM u16 font_${font}[];\n`;
		out_c += `\nconst PROGMEM u16 font_${font}[] = {\n`;

		for (let glyph of glyphs) {
			out_c += '\t' + glyph.image.map(n => '0x' + n.toString(16).padStart(4, '0')).join(', ') + ',  /* ' + glyph.c + ' */\n';
			offset += glyph.image.length;
			offsets.push(offset);
		}

		out_c += '};\n';

		out_h += `extern const PROGMEM u16 font_offset_${font}[];\n`;
		out_c += `\nconst PROGMEM u16 font_offset_${font}[] = {\n`;
		out_c += '\t' + offsets.map(n => String(n)).join(', ') + '\n';
		out_c += '};\n';
	}

	return { h: out_h, c: out_c };
}
