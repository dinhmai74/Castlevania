var fs = require("fs");

const inquirer = require("inquirer");

var questions = [
	{
		type: "input",
		name: "name",
		message: "stage name",
    },
    {
		type: "input",
		name: "left",
		message: "left",
    },
    {
		type: "input",
		name: "top",
		message: "top",
    },
    {
		type: "input",
		name: "width",
		message: "width",
    },
    {
		type: "input",
		name: "height",
		message: "height",
    },
];

inquirer.prompt(questions).then(answers => {
	const w = Number(answers["width"]);
	const h = Number(answers["height"]);
	const l = Number(answers["left"]) || 0;
	const t = Number(answers["top"]) || 0;
	const name = answers["name"] || "stage2";

	calData(name, {w,h,l,t });
});

function calData(name, data) {

    let {w,h,l,t} = data;
    let b= t+h;
    let r= l+w;
    let currentX=l;
    let currentY=b;
    let data="\n"

    appendFile(name+"_objects",data)
    while (currentX < r) {
        currentY -=32;
        data=`0 ${currentX} ${currentY} 32 32 2 1 1 16 16\n`
        appendFile(name+"_objects",data)
        currentX +=32;
    }
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