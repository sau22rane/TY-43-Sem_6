var SELF_IP="localhost"

var fs = require('fs');

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

const app1 = express();
var http = require('http').Server(app1);
var io = require('socket.io')(http);

app1.use(cors());

const PORT = 3000;
const PORT1 = 5051;
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
        decryptedPacket = enc.Decrypt(req.body.data , keys4.private_key , req.body.client_key );
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
    console.log(req.body);
    decryptedPacket = enc.Decrypt(req.body.data , keys4.private_key , req.body.client_key );   
    flag = checksessionTicket(decryptedPacket);
    if(decryptedPacket.client_name === req.body.client_name && flag)
        return true;
    return false;
}
function checksessionTicket(decryptedPacket)
{
    decryptedSessionTicket = enc.Decrypt( decryptedPacket.token , keys4.private_key , publicKey3 );
    console.log(decryptedSessionTicket);
    console.log(decryptedPacket.client_name);
    console.log(decryptedSessionTicket.client_name);
    if(decryptedSessionTicket.client_name === decryptedPacket.client_name)
        return true;
    return false;
}

function post_req(url, data){
    var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
    xmlhttp.onreadystatechange = function($evt){
        if(xmlhttp.readyState == 4 && xmlhttp.status == 200){
            // console.log(xmlhttp.responseText);
            let res = JSON.parse(xmlhttp.responseText);
            console.log("response: ");
            console.log(res);
            resp_data = res;
        }
    }
    xmlhttp.open("POST", url, false);
    xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xmlhttp.send(JSON.stringify(data));
}

function register(data){
    var temp_url = "http://"+SELF_IP+":5000/authentication/register";
    console.log("Registering User");
    post_req(temp_url, data);
}

function fetchUrl(url)  
{  
    const http = new XMLHttpRequest()
    http.open("GET", "http://"+SELF_IP+":6001/ticketGeneration/getpublicKey" )
    http.send()
    http.onload = () => {
        console.log("\npublic key-3  : "+JSON.stringify(JSON.parse(http.responseText)) );
        publicKey3 = { "exp" : JSON.parse(http.responseText).exp , "n": JSON.parse(http.responseText).n };
    };   
}

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


app.listen(PORT,() => console.log(`File Server runinng on port : http://`+SELF_IP+`:${PORT}`));

app1.use(express.static('public'));
http.listen(PORT1 || 8000, function () {
    console.log('listening on', PORT1);
});


register({
    type: 2,
    name: "System Software",
    public_key: keys4.public_key
});

setTimeout(fetchUrl,1600);