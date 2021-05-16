var express = require('express')
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const enc = require('../encryptionLibrary/enc');

const router = express.Router();
const records =[];

const keys3 = enc.Key_init();
const currentDate = new Date();
// generateKeys publicKey and privateKey
let publicKey2 = "publicKey";
let publicKey4 = "publicKey4";


function fetchUrl1(url)  
{  
    const http = new XMLHttpRequest()
    http.open("GET", "http://localhost:5000/authentication/getpublicKey" )
    http.send()
    http.onload = () => {
        console.log("\npublic key-2 : "+JSON.stringify(JSON.parse(http.responseText)) );
        publicKey2 = JSON.parse(http.responseText);
    };    

    const http1 = new XMLHttpRequest()
    http1.open("GET", "http://localhost:3000/getpublicKey" )
    http1.send()
    http1.onload = () => {
        console.log("\npublic key-4 : "+JSON.stringify(JSON.parse(http1.responseText)) );
        publicKey4 = JSON.parse(http1.responseText);
    }

}

setTimeout(fetchUrl1,800);

function checkPacket(data)
{
   decryptedTGT = enc.Decrypt(data.enc_TGT  , keys3.private_key , publicKey2);
    console.log("decryptedTGT : "+ decryptedTGT);
    if(decryptedTGT.name == data.name)
        return true;
    return false;
}
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
router.post('/getToken',( req,res )=> {
    if(checkPacket(req.body))
    {
        const sessionTicket = createSessionTicket(req.body);
        const timestamp = currentDate.getTime();
        const response = {
            "Server_key": "<Server_public_key>",
            "Timestamp": timestamp,
            "Lifetime": timestamp+1234567,
            "Server": "<Server>",
            "enc_sess_ticket": sessionTicket,
            "Nonce": req.body.Nonce
        };
        res.send(response);
    }
    else
    {
        res.send("INVALID CREDENTIAL!!!");
    }
});


router.get('/getpublicKey',(req,res) => {
    res.send(keys3.public_key);
});

module.exports = router;