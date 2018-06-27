import fs from "fs";

WebAssembly.instantiate(fs.readFileSync(process.argv[2])).then(module =>
{
	try {
		module.instance.exports._start();
	}
	catch (error) {
		console.error(error);
		process.exit(1);
	}
});
