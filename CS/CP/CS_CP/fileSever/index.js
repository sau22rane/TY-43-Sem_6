var express = require('express')
var bodyParser = require('body-parser')
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const enc = require('./encryptionLibrary/enc');

const keys4 = enc.Key_init();
console.log("\npublic key-4 : "+JSON.stringify(keys4.public_key) );
const currentDate = new Date();

const app = express();
const PORT = 3000;
app.use(bodyParser.json());
let publicKey3 ="publicKey3";

function fetchUrl()  
{  
    const http = new XMLHttpRequest()
    http.open("GET", "http://localhost:6001/ticketGeneration/getpublicKey")
    http.send()
    http.onload = () => {
        console.log("\npublic key-3 : "+JSON.stringify(JSON.parse(http.responseText)) );
        publicKey3 = JSON.parse(http.responseText);
    };
}
setTimeout(fetchUrl,6000);

function checkSessionTicket(data){
    decryptedSession = enc.Decrypt(data.enc_sess_ticket , keys4.private_key , publicKey3 )
    console.log("decryptedSession : "+ decryptedSession);
}
app.get('/getpublicKey',(req,res) => {

    res.send(keys4.public_key);

});

app.post('/fileServer/accessServer',(req,res) => {
    
    checkSessionTicket(req.body);
    res.send("session working");
});

app.listen(PORT,() => console.log(`File Server runinng on port : http://localhost:${PORT}`));