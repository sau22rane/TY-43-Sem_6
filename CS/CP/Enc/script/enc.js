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
    console.log(aes_key);
    return Decrypt_JSON(enc_data.enc_data, aes_key);
}

function isPrime(num){
    for(var i = 2; i<(num); i++){
        if(num%i === 0)
            return false;
    }
    return true;
}

function Key_init(){
    var p = 181, q = 179, phi, e, d, n;
    
    phi = (p-1) * (q-1);
    n = p * q;

    do{
        e = Math.floor(Math.random() * 100) + 100;
    }
    while(GCD( phi, e) != 1 || e===p || e===q);

    d = inv(e, phi);

    console.log(e+" "+d);
    return{
        public_key: {
            exp: e,
            n: n
        },
        private_key: {
            exp: d,
            n: n
        },
        AES: Math.floor(Math.random() * 10000)
    }
}


var data = '{ "name": "Rane", "id": "11810752", "stream": "CS"}';
var obj = JSON.parse(data);

console.log(obj);

var RSA_KEYS1 = Key_init();
var RSA_KEYS2 = Key_init();

console.log(RSA_KEYS1);
console.log(RSA_KEYS2);

var aes_key = 19189;

var e = Encrypt(obj, RSA_KEYS1.private_key, RSA_KEYS2.public_key, aes_key);
var d = Decrypt(e, RSA_KEYS1.public_key, RSA_KEYS2.private_key);

console.log(e);
console.log(d);