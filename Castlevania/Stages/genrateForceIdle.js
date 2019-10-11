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
        name: "side",
        message: "side force",
    },
    {
		type: "input",
		name: "left",
		message: "left last stair",
    },
    {
		type: "input",
		name: "top",
		message: "top last stair",
    },
];

inquirer.prompt(questions).then(answers => {
	const l = Number(answers["left"]) || 0;
	const t = Number(answers["top"]) || 0;
	const side = Number(answers["side"]) || 1;
	const name = answers["name"] || "stage2";

	calData(name, {l,t,side });
});

function calData(name, data) {

    let {l,side,t} = data;
    let currentX=l+22;
    let currentY=t-64;

    appendFile(name+"_objects","\n")
    let temp=`11 ${currentX} ${currentY} 32 6 ${side}\n`
    appendFile(name+"_objects",temp)
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
		}
	});
}