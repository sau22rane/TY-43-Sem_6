var express = require('express')
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const enc = require('../encryptionLibrary/enc');

const router = express.Router();
const records =[];

const keys3 = enc.Key_init();
const currentDate = new Date();
let publicKey2 = "publicKey";
let publicKey4 = "publicKey4";

router.get('/getpublicKey',(req,res) => {
    res.send(keys3.public_key);
});

router.post('/getToken',( req,res )=> {
    
        console.log("GET /getToken ===> "+ JSON.stringify(req.body));
        var authentic = checkPacket(req);
        if(authentic) 
        {    
            console.log("authentic : "+authentic )
            decryptedTGT = enc.Decrypt( req.body.data  , keys3.private_key , req.body.clientPublicKey );
            
            console.log("decryptedTGT");
            console.log(decryptedTGT);
            const sessionTicket = createSessionTicket(req.body);
            const timestamp = currentDate.getTime();

            var response = {
                "Server_public_key": publicKey4,
                "Timestamp": timestamp,
                "Lifetime": timestamp+1234567,
                "Server": "<Server>",
                "enc_sess_ticket": sessionTicket,
                nonce : req.body.nonce
            };

            response = enc.Encrypt(response , keys3.private_key ,  req.body.clientPublicKey , 19189 );
            response = { "data" : response , "TGS_key" : keys3.public_key , "status" : 200 };
            res.send(response);

    }
    else{
        res.send({"status" : "INVALID CREDENTIALS" })
    }
    
});

function createSessionTicket(data){
    const timestamp = currentDate.getTime();
    const sessionTicket = {
        "client_key": "temp",
        "name": data.name,
        "Lifetime": timestamp+1000000
    };
    return enc.Encrypt(sessionTicket , keys3.private_key ,  publicKey4 , 19189 );
    // console.log("Encrypted sessionTicket : "+sessionTicket);
}

setTimeout(fetchUrl1,800);

function checkPacket(req)
{
    decryptedTGT = enc.Decrypt( req.body.data  , keys3.private_key , req.body.clientPublicKey );   
    if(decryptedTGT.name == req.body.name)
        return true;
    return false;
}

function fetchUrl1(url)  
{  
    const http = new XMLHttpRequest()
    http.open("GET", "http://localhost:5000/authentication/getpublicKey" )
    http.send()
    http.onload = () => {
        console.log("\npublic key-2 : "+JSON.stringify(JSON.parse(http.responseText)) );
        publicKey2 = JSON.parse(http.responseText);
    };    
    post_req("http://localhost:3000/getpublicKey");
}
function post_req(url){
    var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
    xmlhttp.onreadystatechange = function($evt){
        if(xmlhttp.readyState == 4 && xmlhttp.status == 200){
            // console.log(xmlhttp.responseText);
            let res = JSON.parse(xmlhttp.responseText);
            publicKey4 = res;
            console.log("\npublic key-4 : "+JSON.stringify(publicKey4) );
        }
    }
    xmlhttp.open("POST", url, false);
    xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xmlhttp.send(JSON.stringify( { key : keys3.public_key } ));
}

module.exports = router;