// getting dom elements
var divSelectRoom = document.querySelector("#RoomSelection");
var divConsultingRoom = document.querySelector("#consultingRoom");
var inputRoomNumber = document.querySelector("#roomNumber");
var btnGoRoom = document.querySelector("#goRoom");
var localVideo = document.querySelector("#localVideo");
var flag = 0;
var length = 0;
var socketId;
var mixed;
var remoteVideo =[]
// remoteVideo.push(document.querySelector("#remoteVideo1"));
// remoteVideo.push(document.querySelector("#remoteVideo2"));

// variables
var roomNumber;
var localStream;
var remoteStream1;
var remoteStream2;

var configuration = { iceServers: [{
    urls: "stun:stun.services.mozilla.com",
    username: "louis@mozilla.com", 
    credential: "webrtcdemo"
}, {
    urls: ["stun:stun.example.com", "stun:stun-1.example.com"]
}]
};

var rtcPeerConnection = [];
// var rtcPeerConnection[length]1 = new RTCPeerConnection[length](configuration);
var streamConstraints = { audio: true, video: true };
var isCaller;
var cur = 0;
var count = 0;
var mixer;

// Let's do this
var socket = io();
var isChannelReady = false;
var isInitiator = false;
var isStarted = false;

btnGoRoom.onclick = function () {
    if (inputRoomNumber.value === '') {
        alert("Please type a room number")
    } else {
        roomNumber = inputRoomNumber.value;
        socket.emit('create or join', roomNumber);
        divSelectRoom.style = "display: none;";
        divConsultingRoom.style = "display: block;";
    }
};

// message handlers
socket.on('created', function (room) {
    navigator.mediaDevices.getUserMedia(streamConstraints).then(function (stream) {
        localStream = stream;
        
        localVideo.srcObject = stream;
        isCaller = true;
    }).catch(function (err) {
        console.log('An error ocurred when accessing media devices', err);
    });
    isInitiator = true;
    console.log("created "+ socket.id);
    socketId = socket.id;
});


socket.on('joined', function (room) {
    navigator.mediaDevices.getUserMedia(streamConstraints).then(function (stream) {
        localStream = stream;
        localVideo.srcObject = stream;
        socket.emit('ready', {
            rn: roomNumber,
            id: socketId
        }
        );
    }).catch(function (err) {
        console.log('An error ocurred when accessing media devices', err);
    });
    isChannelReady = true;
    socketId = socket.id;
    console.log("joined "+socketId);
});

socket.on('message', function(message) {
    //console.log('Client received message:', message);
  });


socket.on('candidate', function (event) {
    var candidate = new RTCIceCandidate({
        sdpMLineIndex: event.label,
        candidate: event.candidate
    });
    rtcPeerConnection[length].addIceCandidate(candidate);
    
});

socket.on('ready', function (event) {
    console.log("Ready Here: "+event.id+" "+socketId);
    if (isCaller) {
        console.log("Creating Offer ");
        length = rtcPeerConnection.length;
        rtcPeerConnection.push(new RTCPeerConnection(configuration));
        rtcPeerConnection[length].onicecandidate = onIceCandidate;
        rtcPeerConnection[length].ontrack = onAddStream;    
        rtcPeerConnection[length].addTrack(localStream.getTracks()[0], localStream);
        rtcPeerConnection[length].addTrack(localStream.getTracks()[1], localStream);
        rtcPeerConnection[length].createOffer()
            .then(sessionDescription => {
                rtcPeerConnection[length].setLocalDescription(sessionDescription);
                socket.emit('offer', {
                    type: 'offer',
                    sdp: sessionDescription,
                    room: roomNumber,
                    id: socketId
                });
            })
            .catch(error => {
                console.log(error)
            });

        console.log(rtcPeerConnection[length]); 
        sendMessage('answer');
    }
});

socket.on('offer', function (event) {
    console.log("Offer Here: "+event.id+" "+socketId);
    console.log("Creating Answer ");
    if (!isCaller) {
        length = rtcPeerConnection.length;
        rtcPeerConnection.push(new RTCPeerConnection(configuration));
        rtcPeerConnection[length].onicecandidate = onIceCandidate;
        rtcPeerConnection[length].ontrack = onAddStream;
        rtcPeerConnection[length].addTrack(localStream.getTracks()[0], localStream);
        rtcPeerConnection[length].addTrack(localStream.getTracks()[1], localStream);
        rtcPeerConnection[length].setRemoteDescription(new RTCSessionDescription(event.sdp));
        rtcPeerConnection[length].createAnswer()
            .then(sessionDescription => {
                rtcPeerConnection[length].setLocalDescription(sessionDescription);
                socket.emit('answer', {
                    type: 'answer',
                    sdp: sessionDescription,
                    room: roomNumber,
                    to: event.id
                });
            })
            .catch(error => {
                console.log(error)
            })
            console.log('Offer Recieved');

        flag = 1;
        console.log(rtcPeerConnection[length]); 
        console.log('Sending Answer');
    }
});

socket.on('answer', function (event) {
    console.log('Answer Recieved '+length);
    rtcPeerConnection[length].setRemoteDescription(new RTCSessionDescription(event.sdp));
    flag = 1;
})

// handler functions
function onIceCandidate(event) {
    if (event.candidate) {
        console.log('sending ice candidate');
        socket.emit('candidate', {
            type: 'candidate',
            label: event.candidate.sdpMLineIndex,
            id: event.candidate.sdpMid,
            candidate: event.candidate.candidate,
            room: roomNumber
        })

    sendMessage('sentIcecadidate');
    }
}

function onAddStream(event) {
    var temp = remoteVideo.length;
    if(length<=temp){
        remoteVideo.push(document.querySelector("#remoteVideo"+length));
        remoteVideo[length].srcObject = event.streams[0];
        console.log('got user media');
    }
}

function sendMessage(message) {
    socket.emit('message', message);
}