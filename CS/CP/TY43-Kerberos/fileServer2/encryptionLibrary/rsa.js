function RSA(p, q, e){
    phi = (p-1) * (q-1);

    if(GCD( phi, e) == 1){
        console.log("Valid Exponent e = "+e);
    }
    else{
        console.log("Invalid Exponent e = "+e);
    }

    n = (p) * (q);
    d = inv(e, phi);
    return {
        mod: n,
        public_key: e,
        private_key: d 
    }
}

function Encrypt_RSA(num, key){
    return exp(num, key.exp, key.n);
}

function Decrypt_RSA(num, key){
    return exp(num, key.exp, key.n);
}

function exp(num, index, mod){
    var y=1, u=num % mod;
    while (index >0){
        if ((index % 2)==1)
            y=(y*u) % mod;
        if (index > 0)
            index=Math.floor(index / 2);
        u=(u*u) % mod;
    }
    return y%mod;
}

function inv(num, phi){
    var a = num, m = phi, m0 = phi;
    var y = 0, x = 1;
 
    if (m == 1)
        return 0;
 
    while (a > 1) {
        var q = Math.floor(a / m);
        var t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0)
        x += m0;
    return x;
}

function GCD(a, b){
    if(b == 1)
        return 1;
    if(b == 0)
        return 0;
    return GCD(b, a%b);
}
exports.GCD = GCD;
exports.inv = inv;
exports.Decrypt_RSA = Decrypt_RSA;
exports.Encrypt_RSA = Encrypt_RSA;
