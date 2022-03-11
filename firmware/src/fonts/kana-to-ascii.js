const str = process.argv[2];
const replaced = str.split('')
	.map(c => c.charCodeAt(0))
	.map(c => c === 0x20 || c === 0x3000 ? 0 : c - 0x30A0)
	.map(c => String.fromCodePoint(0x20 + c))
	.join('');
console.log(JSON.stringify(replaced));
