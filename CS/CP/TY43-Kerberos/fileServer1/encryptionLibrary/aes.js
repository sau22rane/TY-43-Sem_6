var key, W0, W1, W2, W3, W4, W5, K1, K2, K3;
var S_BOX = [ 0b1001, 0b0100, 0b1010, 0b1011,
            0b1101, 0b0001, 0b1000, 0b0101,
            0b0110, 0b0010, 0b0000, 0b0011,
            0b1100, 0b1110, 0b1111, 0b0111 ];
var S_BOX_INV=[];
var irreducible = 0b10011;
var Me = [[1,4],[4,1]];
var InvMe = [[9,2],[2,9]];

function SubNib(num, n){
    var i = 0, res = 0;
    while (i<n){
        res|= (S_BOX[((num & (15<<i))>>i)]<<i);
        i+=4;
    }

    // if((num>> 2*4) !=0){
    //     res|= (S_BOX[((num & (15<<0*4))>>0*4)]<<0*4);
    //     res|= (S_BOX[((num & (15<<1*4))>>1*4)]<<1*4);
    // }
    // else{
    //     res|= (S_BOX[((num & (15<<0*4))>>0*4)]<<0*4);
    //     res|= (S_BOX[((num & (15<<1*4))>>1*4)]<<1*4);
    //     res|= (S_BOX[((num & (15<<2*4))>>2*4)]<<2*4);
    //     res|= (S_BOX[((num & (15<<3*4))>>3*4)]<<3*4);
    
    // }

    return res;
};

function InvSubNib( num, n){
    var i = 0, res = 0;
    while (i<n){
        res|= (S_BOX_INV[((num & (15<<i))>>i)]<<i);
        i+=4;
    }
    // if((num>> 2*4) !=0){
    //     res|= (S_BOX_INV[((num & (15<<0*4))>>0*4)]<<0*4);
    //     res|= (S_BOX_INV[((num & (15<<1*4))>>1*4)]<<1*4);
    // }
    // else{
    //     res|= (S_BOX_INV[((num & (15<<0*4))>>0*4)]<<0*4);
    //     res|= (S_BOX_INV[((num & (15<<1*4))>>1*4)]<<1*4);
    //     res|= (S_BOX_INV[((num & (15<<2*4))>>2*4)]<<2*4);
    //     res|= (S_BOX_INV[((num & (15<<3*4))>>3*4)]<<3*4);
    // }
    return res;
}
function RotNib( num){
    return ( ((num & 0xF0)>>4) | ((num & 0x0F)<<4) )
}
function RowShift( num){
    return ((num & 0xF0F0) | ((num & 0x000F)<<8) | ((num & 0x0F00)>>8))
}
function PolyReduce( num){
    var i = 7;
    while( (num>>4) >0 && i>3){
        if( (num & (1<<i) ) == 0 ){
            i--;
            continue;
        }
        num^=(irreducible<<(i-4));
        i--;
    }
    return num;
}
function MatMul(a, b){
    var res=[[0,0],[0,0]];
    for(var i = 0; i<2; i++){
        for(var j = 0; j<2; j++){
            var sum = 0;
            for(var k = 0; k<2; k++){
                var t = 0;
                for(var l = 0; l<4; l++){
                    t ^= ( (a[i][k]<<l) * ((b[k][j]>>l)&1) );
                }
                sum^= PolyReduce(t);

            }
            res[i][j] = sum;
        }
    }
    return res;
}
function MixCol(a){
    var res = 0;
    for(var i = 0; i<2; i++){
        for(var j = 0; j<2; j++){
            res |= (a[j][i] << (3 - i*2-j)*4);
        }
    }
    return res;
}

function Encrypt_AES(data){
    // console.log("----Debugging Encrypt----");
    var ARK1 = (data ^ K1);
    // binary(ARK1, 16);
    var subNib = SubNib(ARK1, 16);
    // binary(subNib, 16);
    var shiftRow = RowShift(subNib);
    // binary(shiftRow, 16);
   
    var mat=[[0,0],[0,0]];
    for(var i = 0; i<2; i++){
        for(var j = 0; j<2; j++){
            mat[j][i] = ((shiftRow >> (3 - i*2-j)*4) & 0x0F);
        }
    }

    var S_mat = MatMul(Me, mat);

    var mixColn = MixCol(S_mat);
    // binary(mixColn, 16);
    var ARK2 = (mixColn^K2);
    // binary(ARK2, 16);
    var subNib2 = SubNib(ARK2, 16);
    // binary(subNib2, 16);
    var shiftRow2 = RowShift(subNib2);

    var ARK3 = (shiftRow2^K3);
    
    return ARK3;

}

function Decrypt_AES(data){
    var ARK3 = (data^K3);
    var shiftRow2 = RowShift(ARK3);
    var subNib2 = InvSubNib(shiftRow2, 16);
    var ARK2 = (subNib2^K2);

    var mat= [[0,0],[0,0]];
    for(var i = 0; i<2; i++){
        for(var j = 0; j<2; j++){
            mat[j][i] = ((ARK2 >> (3 - i*2-j)*4) & 0x0F);
        }
    }

    var S_mat= MatMul(InvMe, mat);

    var mixColn = MixCol(S_mat);
    var shiftRow1 = RowShift(mixColn);
    var subNib1 = InvSubNib(shiftRow1, 16);
    var ARK1 = (subNib1^K1);
    return ARK1;
}



function init(k){
    key = k;
    for(var i = 0; i<16; i++){
        S_BOX_INV[ S_BOX[i] ] = i;
    }
}

function binary(num, n){
    var t = "";
    for(var i = 1;i<=n;i++){
        t+=( (num>>(n-i)) & 1);
        if(i%4==0)
            t+=" ";
    }
    // console.log(t);
}


function KeyGenerate(){
    W0 = (key >> 8);
    W1 = (key & 0xFF);
    W2 = (W0 ^ 0x80 ^ SubNib(RotNib(W1), 8) );
    W3 = (W2 ^ W1);
    W4 = (W2 ^ 0x30 ^ SubNib(RotNib(W3), 8) );
    W5 = (W4 ^ W3);
    // console.log("W0:\t");
    // binary(W0, 8);
    // console.log("W1:\t");
    // binary(W1, 8);
    // console.log("W2:\t");
    // binary(W2, 8);
    // console.log("W3:\t");
    // binary(W3, 8);
    // console.log("W4:\t");
    // binary(W4, 8);
    // console.log("W5:\t");
    // binary(W5, 8);

    K1 = ((W0 << 8) | W1);
    K2 = ((W2 << 8) | W3);
    K3 = ((W4 << 8) | W5);
    // console.log("\nK1:\t");
    // binary(K1, 16);
    // console.log("K2:\t");
    // binary(K2, 16);
    // console.log("K3:\t");
    // binary(K3, 16);
}


function binary_debug(num, n){
    var t = "";
    for(var i = 1;i<=n;i++){
        t+=( (num>>(n-i)) & 1);
        if(i%4==0)
            t+=" ";
    }
    console.log(t);
}

exports.init = init;
exports.KeyGenerate = KeyGenerate;
exports.Encrypt_AES = Encrypt_AES;
exports.Decrypt_AES = Decrypt_AES;
