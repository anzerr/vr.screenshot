
const fs = require('fs'),
	Map = require('image.util'),
	bmp = require('bmp.util');

const width = 1364, height = 1516; // these are the values I found I think they are static for the "valve index"

const map = new Map({
	width: width,
	height: height,
	mask: {r: 0, g: 1, b: 2, a: 3}
});

const toFile = (file, out) => {
	const data = fs.readFileSync(file);
	map.mask = {r: 0, g: 1, b: 2, a: 3};
	map.load(data);
	map.mask = {r: 3, g: 2, b: 1, a: 0};
	fs.writeFileSync(out, bmp.encode({
		width: width,
		height: height,
		endian: true,
		data: map.toBuffer()
	}).data);
};

toFile('./x64/Release/left.dump', './assets/left.bmp');
toFile('./x64/Release/right.dump', './assets/right.bmp');
