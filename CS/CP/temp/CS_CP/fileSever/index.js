var express = require('express')
var bodyParser = require('body-parser')
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const enc = require('./encryptionLibrary/enc');
const app = express();

var cors = require('cors');
app.use(cors());
const keys4 = enc.Key_init();
console.log("\npublic key-4 : "+JSON.stringify(keys4.public_key) );
const currentDate = new Date();


const PORT = 3000;
app.use(bodyParser.json());
let publicKey3 ="publicKey3";

app.post('/getpublicKey',(req,res) => {

    publicKey3 = req.body.key;
    console.log("\npublic key-3 : "+JSON.stringify(publicKey3) );
    res.send(keys4.public_key);

});

app.post('/accessServer',(req,res) => {

    var authentic = checkPacket(req);
    if(authentic)
    {
        decryptedPacket = enc.Decrypt(req.body.data , keys4.private_key , req.body.clientPublicKey );
        console.log(decryptedPacket)
    
        decryptedSessionTicket = enc.Decrypt( decryptedPacket.token , keys4.private_key , publicKey3 );
        console.log("decryptedSessionTicket : ");
        console.log(decryptedSessionTicket);
    
        res.send( { "status": 200 } );
    }
    else
    {
        console.log("decryptedSessionTicket : ");
        console.log("INVALID TOKEN ");
        res.send( { "status" : "INVALID TOKEN" } );
    }
   
});

function checkPacket(req)
{
    decryptedPacket = enc.Decrypt(req.body.data , keys4.private_key , req.body.clientPublicKey );    
    flag = checksessionTicket(decryptedPacket);
    if(decryptedPacket.name === req.body.name && flag)
        return true;
    return false;
}
function checksessionTicket(decryptedPacket)
{
    decryptedSessionTicket = enc.Decrypt( decryptedPacket.token , keys4.private_key , publicKey3 );
    if(decryptedSessionTicket.name === decryptedPacket.name)
        return true;
    return false;
}

app.listen(PORT,() => console.log(`File Server runinng on port : http://localhost:${PORT}`));