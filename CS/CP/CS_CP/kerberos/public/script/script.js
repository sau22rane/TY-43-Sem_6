var resp_data;
var client_key;
var global_packet;
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
        var obj = JSON.parse(text);
        console.log(obj);
        client_key = obj;
        return obj;
    };
    
    reader.readAsText(input.files[0]);
};

function download() {
    var temp_name = document.getElementById("Rectangle_3_in").value;
    var a = document.getElementById("a");
    var t = Key_init(temp_name);
    register({
        type: 1,
        name: temp_name,
        public_key: t.public_key
    });
    var key = JSON.stringify(t);
    document.getElementById("Rectangle_3_p").innerText = key;
    var file = new Blob([key], {type: "application/JSON;charset=utf-8"});
    a.href = URL.createObjectURL(file);
    a.download = "Credentials.json";
    a.click();
}


function begin(){
    console.log("client_key" + client_key);
    console.log(client_key);
    
    getTGT();
    getToken();
    accessServer();
}

function register(data){
    var temp_url = "http://localhost:5000/authentication/register";
    console.log("Registering User");
    post_req(temp_url, data);
}

function getTGT()
{
    temp_url = "http://localhost:5000/authentication/getTGT"
    data = {
        client_name: client_key.user_name,
        client_key: client_key.public_key,
        server: document.getElementById("Rectangle_3_in_1").value,
        nonce: 3
    }
    console.log("Requesting AS (Sending AS_req)")
    post_req(temp_url, data);
    console.log(resp_data.data);    
    var packet = Decrypt(resp_data.data  , client_key.private_key , resp_data.AS_public_key ) ;
    console.log("DECRPTED PACKET IS  :  ");
    console.log(packet);   
    global_packet = packet;
}

function getToken(){
    temp_url = "http://localhost:6001/ticketGeneration/getToken"
    data = {
        client_name: client_key.user_name,
        client_key: client_key.public_key,
        server: document.getElementById("Rectangle_3_in_1").value,
        enc_TGT: global_packet.enc_TGT,
        nonce: 3
    }
    data  = Encrypt( data , client_key.private_key , global_packet.TGS_key , 19189 );
    data = { data: data , client_key: client_key.public_key , client_name: client_key.user_name };
    console.log("Requesting session Token (Sending TGS_req)")
    post_req(temp_url, data);
    console.log(resp_data.data);
    if(resp_data.status == 200)
    {
        var packet = Decrypt(resp_data.data  , client_key.private_key , resp_data.TGS_key ) ;
        console.log("DECRPTED PACKET IS  :  ");
        console.log(packet);
    }    
    else
    {
        console.log("DECRPTED PACKET IS  :  ");
        console.log(JSON.stringify(resp_data));
    }
    global_packet = packet;
    
}

function accessServer(){
    console.log("Global_packet");
    console.log(global_packet);
    temp_url = "http://localhost:3000/accessServer"
    data = {
        client_name: client_key.user_name,
        client_key: client_key.public_key,
        server: document.getElementById("Rectangle_3_in_1").value,
        token: global_packet.enc_sess_ticket, 
        nonce: 3
    }
    data  = Encrypt( data , client_key.private_key , global_packet.Server_public_key , 19189 );
    data = { data: data , client_key: client_key.public_key , client_name: client_key.user_name };
    
    var a = document.getElementById("a");
    var node = document.getElementById('Rectangle_3_p');
    var data = JSON.stringify(data);
    node.innerText=data;
    var file = new Blob([data], {type: "application/JSON;charset=utf-8"});
    a.href = URL.createObjectURL(file);
    a.download = "Token.json";
    a.click();
    // location.replace("http://localhost:5050")
}

function fetchServerList(){
    const http = new XMLHttpRequest()
    http.open("GET", "http://localhost:5000/authentication/getServerNames" )
    http.send()
    http.onload = () => {
        var temp_list = JSON.parse(http.responseText).data;
        console.log("\ndata  : "+JSON.stringify(temp_list) );
        var drop_down = document.getElementById("Rectangle_3_in_1");
        for(var i = 0; i<temp_list.length; i++){
            var opt = document.createElement("option");
            opt.value=temp_list[i];
            opt.innerText=temp_list[i];   
            drop_down.appendChild(opt);
        }
    };
}