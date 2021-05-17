var resp_data;
var client_key;
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

function openFile(event) {
    var input = event.target;

    var reader = new FileReader();
    reader.onload = function(){
        var text = reader.result;
        var node = document.getElementById('output');
        var obj = JSON.parse(text);
        node.innerText = JSON.stringify(obj);
        console.log(obj);
        client_key = obj;
        return obj;
    };
    
    reader.readAsText(input.files[0]);
};

function download() {
    var a = document.getElementById("a");

    var key = JSON.stringify(Key_init());

    var file = new Blob([key], {type: "application/JSON;charset=utf-8"});
    a.href = URL.createObjectURL(file);
    a.download = "Credentials.json";
    a.click();
}


function begin(){
    console.log("client_key" + client_key);
    console.log(client_key);
    
    var temp_url = "http://localhost:5000/authentication/register";
    var data = {
        name: "sau22rane",
        publicKey: client_key.public_key
    };
    console.log("Registering User");
    post_req(temp_url, data);
    
    
    temp_url = "http://localhost:5000/authentication/getTGT"
    data = {
        name: "sau22rane",
        clientPublicKey: client_key.public_key,
        server: "TGS",
        nonce: 3
    }
    console.log("Requesting AS (Sending AS_req)")
    post_req(temp_url, data);
    console.log(resp_data.data);    
    var packet = Decrypt(resp_data.data  , client_key.private_key , resp_data.AS_public_key ) ;
    console.log("DECRPTED PACKET IS  :  ");
    console.log(packet);
    

    
    
    temp_url = "http://localhost:6001/ticketGeneration/getToken"
    data = {
        name: "sau22rane",
        clientPublicKey: client_key.public_key,
        server: "B",
        enc_TGT: packet.enc_TGT,
        nonce: 3
    }
    data  = Encrypt( data , client_key.private_key , packet.TGS_key , 19189 );
    data = { "data": data , clientPublicKey: client_key.public_key };
    console.log("Requesting session Token (Sending TGS_req)")
    post_req(temp_url, data);
    console.log(resp_data.data);    
    var packet = Decrypt(resp_data.data  , client_key.private_key , resp_data.TGS_key ) ;
    console.log("DECRPTED PACKET IS  :  ");
    console.log(packet);


    temp_url = "http://localhost:3000/accessServer"
    data = {
        name: "sau22rane",
        clientPublicKey: client_key.public_key,
        server: "B",
        token: packet.enc_sess_ticket, 
        nonce: 3
    }
    data  = Encrypt( data , client_key.private_key , packet.Server_public_key , 19189 );
    data = { "data": data , clientPublicKey: client_key.public_key };
    console.log("Requesting session Token (Sending TGS_req)");
    post_req(temp_url, JSON.parse(data));


}

