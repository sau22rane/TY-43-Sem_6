function Encrypt_JSON(json_data, key){
    init(key);
    KeyGenerate();

    var b = JSON.stringify(json_data);
    var enc_word = [];
    var charCode;

    for (var i = 0; i < b.length; ++i){
        charCode = b.charCodeAt(i);
        enc_word.push(Encrypt_AES(charCode));
    }

    var msg = "";
    for (var i = 0; i < enc_word.length; i++)
        msg+= String.fromCharCode(enc_word[i]);
    // console.log(msg);
    return msg;
}

function Decrypt_JSON(encoded, key){
    init(key);
    KeyGenerate();

    var de_word = [];
    var charCode;

    for (var i = 0; i < encoded.length; ++i){
        charCode = encoded.charCodeAt(i);
        de_word.push(Decrypt_AES(charCode));
    }

    var msg = "";
    for (var i = 0; i < de_word.length; i++)
        msg+= String.fromCharCode(de_word[i]);
    // console.log(msg);
    return msg;
}

function Encrypt_Key(aes_key, key1, key2){
    return Encrypt_RSA(Encrypt_RSA(aes_key,key1), key2);
}

function Decrypt_Key(enc_aes_key, key1, key2){
    return Decrypt_RSA(Decrypt_RSA(enc_aes_key,key2), key1);
}

function Encrypt(json_data, key1, key2, aes_key){
    return {
    enc_data: Encrypt_JSON(json_data, aes_key),
    enc_key: Encrypt_Key(aes_key, key1, key2)
    }
}

function Decrypt(enc_data, key1, key2){
    var aes_key = Decrypt_Key(enc_data.enc_key, key1, key2);
    return Decrypt_JSON(enc_data.enc_data, aes_key);
}



var data = '{ "name": "Rane", "id": "11810752", "stream": "CS"}';
var obj = JSON.parse(data);

console.log(obj);

// p = 193, q = 223

var RSA_KEYS1 = {
    public_key: {
        exp: 107,
        n: 43039
    },
    private_key: {
        exp: 30275,
        n: 43039
    }
}

var RSA_KEYS2 = {
    public_key: {
        exp: 113,
        n: 43039
    },
    private_key: {
        exp: 20369,
        n: 43039
    }
}

var aes_key = 19189;

// var e = Encrypt_Key(aes_key, RSA_KEYS1.private_key, RSA_KEYS2.public_key);
// var d = Decrypt_Key(e, RSA_KEYS1.public_key, RSA_KEYS2.private_key)

var e = Encrypt(obj, RSA_KEYS1.private_key, RSA_KEYS2.public_key, aes_key);
var d = Decrypt(e, RSA_KEYS1.public_key, RSA_KEYS2.private_key);

console.log(e);
console.log(d);