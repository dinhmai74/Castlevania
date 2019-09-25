var fs = require("fs");

const inquirer = require("inquirer");

var questions = [
	{
		type: "input",
		name: "name",
		message: "object name",
	},
	{
		type: "input",
		name: "animName",
		message: "anim name",
	},

	{
		type: "input",
		name: "num",
		message: "number Sprite in texture ",
	},
	{
		type: "input",
		name: "left",
		message: "left texture",
	},
	{
		type: "input",
		name: "top",
		message: "top texture",
	},

	{
		type: "input",
		name: "width",
		message: "width texture",
	},
	{
		type: "input",
		name: "height",
		message: "height texture",
    },

    {
		type: "input",
		name: "bleft",
		message: "bleft texture",
	},
	{
		type: "input",
		name: "btop",
		message: "btop texture",
	},

	{
		type: "input",
		name: "bwidth",
		message: "bwidth texture",
	},
	{
		type: "input",
		name: "bheight",
		message: "bheight texture",
    },

	{
		type: "input",
		name: "duration",
		message: "default anim duration",
	},
];

inquirer.prompt(questions).then(answers => {
	const num = Number(answers["num"]);
	const w = Number(answers["width"]);
	const h = Number(answers["height"]);
	const l = Number(answers["left"]) || 0;
	const t = Number(answers["top"]) || 0;
	const name = answers["name"] || "default_sprites";
	let animName = answers["animName"] || "default";
    animName+="_ani"
    
	const bw = Number(answers["bwidth"]);
	const bh = Number(answers["bheight"]);
	const bl = Number(answers["bleft"]) || 0;
	const bt = Number(answers["btop"]) || 0;

	const duration = Number(answers["duration"]) || 100;
	calData(name, animName, { num, l, t, w, h, bl,bt,bw,bh,duration });
});

function calData(name, animName, data) {
	const { num, w, h, l, t, bw,bl,bt,bh,duration } = data;
	let spriteData = ``;
	let animData = `${name + "_" + animName}\t`;

	const spriteWidht = w / num;
	const bspriteWidht = bw / num;

	for (let i = 0; i < num; i++) {
		const spriteName = `${name + "_" + animName}_${i}`;
		const spritProp = `${Number(l + i * spriteWidht).toFixed(
			0,
		)}\t${t}\t${Number(l+ i * spriteWidht + spriteWidht).toFixed(0)}\t${Number(
			t + h,
        ).toFixed(0)}`;
        
		const bspritProp = `${Number(bl + i * bspriteWidht).toFixed(
			0,
		)}\t${bt}\t${Number(bl+ i * bspriteWidht + bspriteWidht).toFixed(0)}\t${Number(
			bt + bh,
        ).toFixed(0)}`;
 
		const spriteTemp = `${spriteName}\t${spritProp}\t${bspritProp}\n`;

		let animTemp =
			i != num - 1
				? `${spriteName}\t${duration}\t`
				: `${spriteName}\t${duration}`;

		animData += animTemp;
		spriteData += spriteTemp;
	}

	appendFile(name + "_sprites", spriteData);
	appendFile(name + "_anims", animData+"\n");
}

function appendFile(name, data) {
	fs.stat(`${name}.txt`, function(err, stat) {
		if (err == null) {
        } 
        else if(err.code === 'ENOENT') {
        // file does not exist
        data+="\n";
    }
        else {
			console.log("Some other error: ", err.code);
		}
	});

	fs.appendFile(`${name}.txt`, data, function(err) {
		if (err) {
			// append failed
			console.log("error: ", err);
		} else {
			// done
			console.log("insert success!");
		}
	});
}