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
        message: "side stair",
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
	const side = Number(answers["side"]) || 1;
	const name = answers["name"] || "stage2";

	calData(name, {w,h,l,t,side });
});

function calData(name, data) {

    let {w,h,l,side,t} = data;
    let b= t+h;
    let r= l+w;
    let currentX=l;
    let currentY=side ==1?b:t;

    appendFile(name+"_objects","\n")
    while (true) {
        if(side ==1 )currentY = currentY -32 * side;
        let data=`0 ${currentX} ${currentY} 32 32 2 1 ${side} 16 16\n`
        appendFile(name+"_objects",data)
        currentX =currentX + 32 ;
        if(side ==-1 )currentY = currentY -32 * side;
        if( currentX > r) break;
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