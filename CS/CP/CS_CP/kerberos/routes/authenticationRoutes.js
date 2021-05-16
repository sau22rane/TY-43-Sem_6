var express = require('express')
const enc = require('../encryptionLibrary/enc');
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;

// generateKeys publicKey and privateKey
const keys2 = enc.Key_init();
let publicKey3 = "publucKey3";
const currentDate = new Date();

const router = express.Router();
const records =[];
var users = [];
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
    var foundValue = users.filter(obj=>obj.name===data.name);
    const tempTGT = {
        client_key: foundValue[0].name,
        name: data.name,
        Lifetime: timestamp+1000000
    };
    return enc.Encrypt( tempTGT , keys2.private_key , publicKey3 , 19189 );
}

router.post('/register',(req,res) => {
    console.log(req.body);
    const data = req.body;
    var foundValue = users.filter(obj=>obj.name===data.name);
    if(foundValue.length===0){
        users.push(data);
        res.send(users);
    }
    else
        res.send("User already exists..!!!");

});

router.post('/getTGT',(req,res) => {
    const TGT = createTGT(req.body);
    const timestamp = currentDate.getTime();

    const response = {
    TGS_key: publicKey3,
    Timestamp: timestamp,
    Lifetime: timestamp+1000000,
    Server: "SSERVER_TGS",
    enc_TGT: TGT,
    Nonce: req.body.Nonce
    };
    res.send(response);

});

router.get('/getpublicKey',(req,res) => {
    res.send(keys2.public_key);
});

module.exports = router;