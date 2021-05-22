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
        var decryptedReq = enc.Decrypt( req.body.data  , keys3.private_key , req.body.client_key );
        console.log(decryptedReq);
        var authentic = checkPacket(decryptedReq.client_name, req.body.client_name);
        if(authentic) 
        {    
            console.log("authentic : "+authentic )
            console.log(keys3.private_key);
            console.log(publicKey2);
            decryptedTGT = enc.Decrypt( decryptedReq.enc_TGT , keys3.private_key , publicKey2 );
            
            console.log("decryptedTGT");
            console.log(decryptedTGT);
            const sessionTicket = createSessionTicket(decryptedTGT);
            const timestamp = currentDate.getTime();

            var response = {
                Server_public_key: decryptedTGT.server_key,
                Timestamp: timestamp,
                Lifetime: timestamp+1234567,
                Server: decryptedTGT.server_name,
                enc_sess_ticket: sessionTicket,
                nonce : req.body.nonce
            };

            response = enc.Encrypt(response , keys3.private_key ,  req.body.client_key , 19189 );
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
        client_key: data.client_key,
        client_name: data.client_name,
        Lifetime: timestamp+1000000
    };
    console.log(keys3.private_key);
    console.log(data.server_key);
    return enc.Encrypt(sessionTicket , keys3.private_key ,  data.server_key , 19189 );
    // console.log("Encrypted sessionTicket : "+sessionTicket);
}

setTimeout(fetchUrl1,800);

function checkPacket(name1, name2)
{
    if(name1===name2)
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
    // post_req("http://localhost:3000/getpublicKey");
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