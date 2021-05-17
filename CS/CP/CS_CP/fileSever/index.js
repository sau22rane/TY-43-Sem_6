var fs = require('fs');

var express = require('express')
var bodyParser = require('body-parser')
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const enc = require('./encryptionLibrary/enc');

const keys4 = enc.Key_init();
console.log("\npublic key-4 : "+JSON.stringify(keys4.public_key) );
const currentDate = new Date();

const app = express();
const app1 = express();
var http = require('http').Server(app1);
var io = require('socket.io')(http);

var cors = require('cors');
app.use(cors());
app1.use(cors());

const PORT = 3000;
const PORT1 = 5050;
app.use(bodyParser.json());
let publicKey3 ="publicKey3";


function checkSessionTicket(data){
    decryptedSession = enc.Decrypt(data.enc_sess_ticket , keys4.private_key , publicKey3 )
    console.log("decryptedSession : "+ decryptedSession);
}
app.post('/getpublicKey',(req,res) => {

    publicKey3 = req.body.key;
    console.log("\npublic key-3 : "+JSON.stringify(publicKey3) );
    res.send(keys4.public_key);

});

app.post('/accessServer',(req,res) => {
    
    decryptedPacket = enc.Decrypt(req.body.data , keys4.private_key , req.body.clientPublicKey );
    console.log(decryptedPacket)
    
    decryptedSessionTicket = enc.Decrypt( decryptedPacket.token , keys4.private_key , publicKey3 );
    console.log("decryptedSessionTicket : ");
    console.log(decryptedSessionTicket);

    res.send("session working");
});

app.get('/fileServer/getFileList',(req,res) => {
    
    var files = fs.readdirSync('./public/data/');
    const response = [];
    for (let file of files) {
        const fileDetails = fs.statSync('./public/data/'+file);
        response.push({name:file, size:fileDetails.size, last_modified: fileDetails.mtime});
    }
    res.send(response);
});

app.get('/fileServer/getFile/:fname',(req,res) => {
    res.download('./public/data/'+req.params.fname);
});


app.listen(PORT,() => console.log(`File Server runinng on port : http://localhost:${PORT}`));

app1.use(express.static('public'));
http.listen(PORT1 || 8000, function () {
    console.log('listening on', PORT1);
});