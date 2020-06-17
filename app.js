const { exec } = require("child_process");
var express=require('express');
var app=express();
var bodyParser = require("body-parser");


app.get("/:id",(req,res)=>{

    if(req.params.id==="ac_on"){
        executer("ac_on");
    }
    else if(req.params.id==="ac_off"){
        executer("ac_off");
    
    }
    else if(req.params.id==="set"){
        executer("temp"+req.query.temperature);
    }
    else if(req.params.id==="swing"){
        if(req.query.vertical==0){
		executer("swing02");
	}
	else if(req.query.vertical==1){ 
                executer("swing12");
        }
	if(req.query.horizontal==0){ 
                executer("swing20");
        }
	else if(req.query.horizontal==1){ 
                executer("swing21");
        }


    }
    else if(req.params.id==="setxfan"){
        executer("xfan"+req.query.value);
    }
    else if(req.params.id==="setfan"){
        executer("fan"+req.query.value);
    }
    else if(req.params.id==="light_on"){
        executer("light_on");
    }
    else if(req.params.id==="light_off"){
       executer("light_off");
    }

    res.send("Request: "+req.params.id+" Executed!!!!!");
});


function executer(cmd){
    exec("mosquitto_pub -h <Enter your Mosquitoo server adress> -i ESP8266Client- -t test -m "+cmd, (error, stdout, stderr) => {
        if (error) {
            console.log(`error: ${error.message}`);
            return;
        }
        if (stderr) {
            console.log(`stderr: ${stderr}`);
            return;
        }
        console.log(`stdout: ${stdout}`);
    });
	return;
}


app.listen(3000);
