var resp_data;
var file_table;
var client_token;
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

function get_req(url){
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
    xmlhttp.open("GET", url, false);
    xmlhttp.send();
}

function openFile(event) {
    var input = event.target;

    var reader = new FileReader();
    reader.onload = function(){
        var text = reader.result;
        // var node = document.getElementById('Rectangle_3_p');
        var obj = JSON.parse(text);
        // node.innerText = JSON.stringify(obj);
        console.log(obj);
        client_token = obj;
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


function addElements(data){
    var top= 355;
    var parent=document.querySelector("#Group_5");

    for(var i = 0; i < data.length; i++){
        var div1 = document.createElement("div");
        var div2 = document.createElement("div");
        var div3 = document.createElement("div");
        var div4 = document.createElement("div");
        var span1 = document.createElement("span");
        var span2 = document.createElement("span");
        var span3 = document.createElement("span");
        var span4 = document.createElement("span");
        div1.classList.add("srno");
        div1.style.top=(top+i*68)+"px";
        div2.classList.add("fname");
        div2.style.top=(top+i*68)+"px";
        div3.classList.add("fsize");
        div3.style.top=(top+i*68)+"px";
        div4.classList.add("modified");
        div4.style.top=(top+i*68)+"px";
        span1.innerText=i+1;

        var link = document.createElement('a');
        link.text=data[i].name;
        link.style.color="rgb(62, 62, 62)";
        link.href='http://localhost:3003/fileserver/getFile/'+link.text;

        span2.appendChild(link);
        span3.innerText=data[i].size;
        span4.innerText=data[i].last_modified;
        div1.appendChild(span1);
        parent.appendChild(div1);
        div2.appendChild(span2);
        parent.appendChild(div2);
        div3.appendChild(span3);
        parent.appendChild(div3);
        div4.appendChild(span4);
        parent.appendChild(div4);
    }
}


function createTable(data){
    var body = document.body,
        tbl  = document.createElement('table');
    tbl.style.width  = '100px';
    tbl.style.border = '1px solid black';

    var tr_top = tbl.insertRow();
    var td_top1 = tr_top.insertCell();
    td_top1.appendChild(document.createTextNode("Sr. no."));
    td_top1 .style.border = '1px solid black';


    var td_top2 = tr_top.insertCell();
    td_top2.appendChild(document.createTextNode("File Name"));
    td_top2 .style.border = '1px solid black';


    var td_top3 = tr_top.insertCell();
    td_top3.appendChild(document.createTextNode("Size"));
    td_top3 .style.border = '1px solid black';


    var td_top4 = tr_top.insertCell();
    td_top4.appendChild(document.createTextNode("Last Modified"));
    td_top4 .style.border = '1px solid black';

    for(var i = 0; i < data.length; i++){
        var tr = tbl.insertRow();
        var td1 = tr.insertCell();
        td1.appendChild(document.createTextNode(i));
        td1 .style.border = '1px solid black';


        var td2 = tr.insertCell();
        var link = document.createElement('a');
        // link.href=data[i].name;
        link.text=data[i].name;
        link.href='http://localhost:3003/fileserver/getFile/'+link.text;
        td2.appendChild(link);
        td2 .style.border = '1px solid black';


        var td3 = tr.insertCell();
        td3.appendChild(document.createTextNode(data[i].size));
        td3 .style.border = '1px solid black';


        var td4 = tr.insertCell();
        td4.appendChild(document.createTextNode(data[i].last_modified));
        td4 .style.border = '1px solid black';
    }
    body.appendChild(tbl);
}

window.addEventListener("load", function(){
    
})

function fetchData(){
    var temp_url = "http://localhost:3003/fileserver/getFileList";
    get_req(temp_url);
    addElements(resp_data);
}

function verify(){
    accessServer();
}

function accessServer(){
    temp_url = "http://localhost:3003/accessServer"
    // data = {
    //     name: "sau22rane",
    //     clientPublicKey: client_key.public_key,
    //     server: "B",
    //     token: global_packet.enc_sess_ticket, 
    //     nonce: 3
    // }
    // data  = Encrypt( data , client_key.private_key , global_packet.Server_public_key , 19189 );
    // data = { "data": data , clientPublicKey: client_key.public_key , name: "sau22rane" };
    // console.log("Requesting session Token (Sending TGS_req)");
    post_req(String(temp_url), client_token);
    if(resp_data.status == 200 )
    {
        console.log("I GOT PERMISSION TO FILE SERVER");
        document.querySelector("a").href="";
        // document.querySelector("#credentials").remove();
        // document.body.appendChild(file_table);
        location.replace("files.html")
        return true;
    }
    else{
        console.log("INVALID TOKEN");
        return false;
    }
}

function startUI(){
    file_table = document.getElementById("data_table");
    document.getElementById("data_table").remove();
}

// startUI()