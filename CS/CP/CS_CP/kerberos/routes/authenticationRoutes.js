var express = require('express')
const enc = require('../encryptionLibrary/enc');
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;

const router = express.Router();
const currentDate = new Date();
const keys2 = enc.Key_init();

let publicKey3 = "publucKey3";
const records =[];
var users = [];
var servers = [];

router.get('/getpublicKey',(req,res) => {
    res.send(keys2.public_key);
});

router.get('/getServerNames',(req,res) => {
    var temp_names=[];
    for(var i = 0; i<servers.length; i++)
        temp_names.push(servers[i].name);
    res.send({data:temp_names});
});

router.post('/register',(req,res) => {
    console.log("GET /register ===> "+ JSON.stringify(req.body));
    const data = req.body;
    if(data.type===1){
        var foundValue = users.filter(obj=>obj.name===data.name);
        if(foundValue.length===0){
            users.push(data);
            console.log("User Registered:")
            console.log(data);

            res.send(users);
        }
        else
            res.send( {"status" : 200 } );
    }
    else if(data.type===2){
        var foundValue = servers.filter(obj=>obj.name===data.name);
        if(foundValue.length===0){
            servers.push(data);
            console.log("Server Registered:")
            console.log(data);

            res.send(servers);
        }
        else
            res.send( {"status" : 200 } );
    }


});

router.post('/getTGT',(req,res) => {
    console.log("GET /getTGT ===> "+ JSON.stringify(req.body));
    var data=req.body;
    console.log(data);
    var server_alias = servers.filter(obj=>obj.name===data.server)[0];
    const TGT = createTGT(req.body, server_alias);
    const timestamp = currentDate.getTime();

    const packet = {
    TGS_key: publicKey3,
    Timestamp: timestamp,
    Lifetime: timestamp+1000000,
    Server: "SSERVER_TGS",
    enc_TGT: TGT,
    nonce: req.body.nonce
    };

    response = enc.Encrypt( packet , keys2.private_key , req.body.client_key , 19189 );
    response = { "data" : response, "AS_public_key" : keys2.public_key };
    res.send(response);

});

function createTGT(data, server_alias){
    const timestamp = currentDate.getTime();
    var foundValue = users.filter(obj=>obj.name===data.client_name);
    // check user exist//remaining
    console.log("client:");
    console.log(foundValue[0]);
    const tempTGT = {
        server_key: server_alias.public_key,
        server_name: server_alias.name,
        client_key: foundValue[0].public_key,
        client_name: data.client_name,
        Lifetime: timestamp+1000000
    };
    console.log("");
    console.log("TGT");
    console.log(tempTGT);
    return enc.Encrypt( tempTGT , keys2.private_key , publicKey3 , 19189 );
}

function fetchUrl(url)  
{  
    const http = new XMLHttpRequest()
    http.open("GET", "http://localhost:6001/ticketGeneration/getpublicKey" )
    http.send()
    http.onload = () => {
        console.log("\npublic key-3  : "+JSON.stringify(JSON.parse(http.responseText)) );
        publicKey3 = { "exp" : JSON.parse(http.responseText).exp , "n": JSON.parse(http.responseText).n };
    };   
}
setTimeout(fetchUrl,1600);



module.exports = router;