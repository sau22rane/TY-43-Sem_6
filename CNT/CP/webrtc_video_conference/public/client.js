// getting dom elements
var divSelectRoom = document.querySelector("#RoomSelection");
var divConsultingRoom = document.querySelector("#consultingRoom");
var inputRoomNumber = document.querySelector("#roomNumber");
var btnGoRoom = document.querySelector("#goRoom");
var localVideo = document.querySelector("#localVideo");
var flag = 0;
var length = 0;
var socketId;
var remoteVideo =[]
// remoteVideo.push(document.querySelector("#remoteVideo1"));
// remoteVideo.push(document.querySelector("#remoteVideo2"));

var offerCreated = false;
var ready = false;

// variables
var Client_IDs = [];
var roomNumber;
var localStream;

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
    var i = 0;
    while (i<6) {
        remoteVideo.push(document.querySelector("#remoteVideo"+i));
        i++;
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
        ready = true;
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
    if(!Client_IDs.includes(event.from)){

        Client_IDs.push(event.from);
        var candidate = new RTCIceCandidate({
            sdpMLineIndex: event.label,
            candidate: event.candidate
        });
        rtcPeerConnection[length].addIceCandidate(candidate);
        console.log("recieved candidate from: jjj "+event.from);
    }
    
    
});

socket.on('ready', function (event) {
    offerCreated = true;
    console.log("Ready Here: "+event.id+" "+socketId);
    document.querySelector("#create_offer").style.visibility = "visible";

    // document.querySelector("#create_offer").addEventListener("click", function(){
        if (event.id!=socketId) {
            
            var temp = event.id;
            console.log("Creating Offer ");
            length = rtcPeerConnection.length;
            rtcPeerConnection.push(new RTCPeerConnection(configuration));
            rtcPeerConnection[length].onicecandidate = function(event, t = temp) {
                if (event.candidate) {
                    console.log('sending ice candidate');
                    socket.emit('candidate', {
                        type: 'candidate',
                        label: event.candidate.sdpMLineIndex,
                        id: event.candidate.sdpMid,
                        candidate: event.candidate.candidate,
                        room: roomNumber,
                        from: socketId,
                        to: t
                    })
            
                console.log('to: ttttt '+temp);
                }
            };

            rtcPeerConnection[length].ontrack = function (event, t = temp) {
                // while(Client_IDs.indexOf(t) === -1);
                remoteVideo[length].srcObject = event.streams[0];
                console.log('got user media');
                console.log(rtcPeerConnection);
                console.log(remoteVideo);    
            };    
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
            document.querySelector("#create_offer").style.visibility = "hidden";
        }
    // });
});

socket.on('offer', function (event) {
    ready = false;
    console.log("Offer Here: "+event.id+" "+socketId);
    console.log("Creating Answer ");
    if (!isCaller) {
        if(!Client_IDs.includes(event.id)){

            Client_IDs.push(event.id);
        }
        var temp = event.id;
        length = rtcPeerConnection.length;
        rtcPeerConnection.push(new RTCPeerConnection(configuration));

        rtcPeerConnection[length].onicecandidate = function(event, t = temp) {
            if (event.candidate) {
                console.log('sending ice candidate');
                socket.emit('candidate', {
                    type: 'candidate',
                    label: event.candidate.sdpMLineIndex,
                    id: event.candidate.sdpMid,
                    candidate: event.candidate.candidate,
                    room: roomNumber,
                    from: socketId,
                    to: t
                })
        
            console.log('to: ttttt '+temp);
            }
        };

        var ind = Client_IDs.indexOf(event.id);

        rtcPeerConnection[ind].ontrack = function(event, t = ind) {
            remoteVideo[t].srcObject = event.streams[0];
            console.log('got user media');
            console.log(rtcPeerConnection);
            console.log(remoteVideo);    
        };
        rtcPeerConnection[ind].addTrack(localStream.getTracks()[0], localStream);
        rtcPeerConnection[ind].addTrack(localStream.getTracks()[1], localStream);
        rtcPeerConnection[ind].setRemoteDescription(new RTCSessionDescription(event.sdp));
        rtcPeerConnection[ind].createAnswer()
            .then(sessionDescription => {
                rtcPeerConnection[ind].setLocalDescription(sessionDescription);
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
        console.log(rtcPeerConnection); 
        console.log(Client_IDs); 
        console.log('Sending Answer');
    }
});

socket.on('answer', function (event) {
    offerCreated = false;
    console.log('Answer Recieved '+length);
    rtcPeerConnection[length].setRemoteDescription(new RTCSessionDescription(event.sdp));
    console.log(rtcPeerConnection);
    console.log(remoteVideo);
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
            room: roomNumber,
            from: socketId
        })

    sendMessage('sentIcecadidate');
    }
}

function onAddStream(event) {
    remoteVideo[0].srcObject = event.streams[0];
    console.log('got user media');
    console.log(rtcPeerConnection);
    console.log(remoteVideo);    
}

function sendMessage(message) {
    socket.emit('message', message);
}