var express = require('express')
const enc = require('../encryptionLibrary/enc');
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;

// generateKeys publicKey and privateKey
const keys2 = enc.Key_init();
let publicKey3 = "publucKey3";
const currentDate = new Date();

const router = express.Router();
const records =[];
var users = {"mitanshu" : 119};
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

function createTGT(data){
    const timestamp = currentDate.getTime();
    const tempTGT = {
        "client_key": users[data.name],
        "name": data.name,
        "Lifetime": timestamp+1000000
    };
    return enc.Encrypt( tempTGT , keys2.private_key , publicKey3 , 19189 );
}

router.post('/register',(req,res) => {
    console.log('post working');
    const data = req.body;
    users[data.name] = data.publicKey;

    res.send(users);

});

router.post('/getTGT',(req,res) => {
    const TGT = createTGT(req.body);
    const timestamp = currentDate.getTime();

    const response = {
    "TGS_key": publicKey3,
    "Timestamp": timestamp,
    "Lifetime": timestamp+1000000,
    "Server": "SSERVER_TGS",
    "enc_TGT": TGT,
    "Nonce": req.body.Nonce
    };
    res.send(response);

});

router.get('/getpublicKey',(req,res) => {
    res.send(keys2.public_key);
});

module.exports = router;