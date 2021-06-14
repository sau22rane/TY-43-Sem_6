function openFile(event) {
    var input = event.target;

    var reader = new FileReader();
    reader.onload = function(){
        var text = reader.result;
        var node = document.getElementById('output');
        var obj = JSON.parse(text);
        node.innerText = JSON.stringify(obj);
        console.log(obj);
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
