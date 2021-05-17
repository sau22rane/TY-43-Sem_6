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
        link.href='http://localhost:3000/fileserver/getFile/'+link.text;
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
    var temp_url = "http://localhost:3000/fileserver/getFileList";
    get_req(temp_url);
    createTable(resp_data);
})