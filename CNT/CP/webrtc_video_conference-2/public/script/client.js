// getting dom elements
var divConsultingRoom = document.querySelector("#consultingRoom");
var localVideo = document.querySelector("#local-video");
var newMessage = document.querySelector("#new_msg");
var chatViewList = document.querySelector("#chat-view-list");

var count = 0;
var length = 0;

var localVideoText = document.querySelector("#local-video-text");

var muteBtn = document.querySelector("#mute-button");
var vdoBtn = document.querySelector("#video-pause");
var endCallBtn = document.querySelector("#end-call-button");
var participantsBtn = document.querySelector("#show-participants-button");
var chatIcon = document.querySelector("#chat-icon");

var socketId;
var remoteVideo =[]
var remoteVideoText =[]
var textMessages=[]

var userName="";

// variables
var Client_IDs = [];
var Client_Uname = [];
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
var streamConstraints = { audio: true, video: true };
var isCaller;

// Let's do this
var socket = io();
var isChannelReady = false;
var isInitiator = false;
var isStarted = false;

newMessage.addEventListener('keyup', function (e) {
    if (e.key === 'Enter') {
        socket.emit("message", {
            rn: roomNumber,
            username: userName,
            message: newMessage.value
        });
        textMessages.push({
            username: userName,
            message: newMessage.value
        });
        console.log(textMessages);

    var msg = document.createElement("div");
    msg.classList.add("talk-bubble-self","tri-right","round","right-top");
    var textArea = document.createElement("div");
    textArea.classList.add("talktext");
    textArea.innerHTML = `<p style="color: white">You</p><p style="color: white">${newMessage.value}</p>`;

    newMessage.value = "";
    msg.appendChild(textArea);
    chatViewList.appendChild(msg);
    }
    
});

participantsBtn.onclick = function(){
    var ic = document.querySelector("#participants-icon");
    var txt = document.querySelector("#participants-text");

    ic.classList.toggle("fa-user");
    ic.classList.toggle("fa-user-slash");

    var state = document.querySelector("#entire-participants").style.visibility;
    console.log(state);
    if(state === "hidden"){

        if(document.querySelector("#entire-chat").style.visibility === "visible"){
            toggleChat();
        }

        document.querySelector("#entire-participants").style.visibility = "visible";
    }
    else{
    document.querySelector("#entire-participants").style.visibility = "hidden";
    }

    if(txt.innerText === "Show Participants"){
        txt.innerText = "Hide Participants";
    }
    else{
        txt.innerText = "Show Participants";
    }

    updateParticipants();

}

chatIcon.onclick = toggleChat;

endCallBtn.onclick = function(){
    localStream.getTracks()[0].enabled = false;
    localStream.getTracks()[1].enabled = false;

    socket.emit('end_connection', {
            id: socketId, 
            rn: roomNumber
        });
    
    while(rtcPeerConnection.length!=0){
        rtcPeerConnection[0].close();
        rtcPeerConnection.shift();
    }
    window.open("index.html","_self");
}

muteBtn.onclick = function () {
    var state = localStream.getTracks()[0].enabled;
    localStream.getTracks()[0].enabled = !state;
    document.querySelector("#mic-icon").classList.toggle("fa-microphone-slash");
    document.querySelector("#mic-icon").classList.toggle("fa-microphone");
    if(document.querySelector("#mic-text").innerText === "Mute"){
        document.querySelector("#mic-text").innerText = "Unmute";
    }
    else{
        document.querySelector("#mic-text").innerText = "Mute";
    }

}

vdoBtn.onclick = function () {
    var state = localStream.getTracks()[1].enabled;
    localStream.getTracks()[1].enabled = !state;
    document.querySelector("#video-icon").classList.toggle("fa-video-slash");
    document.querySelector("#video-icon").classList.toggle("fa-video");

    if(document.querySelector("#video-text").innerText === "Pause Video"){
        document.querySelector("#video-text").innerText = "Resume Video";
    }
    else{
        document.querySelector("#video-text").innerText = "Pause Video";
    }
}

function startUp() {
    
    roomNumber = Number(localStorage.getItem('roomnumber'));
    socket.emit('create or join', roomNumber);
    userName = String(localStorage.getItem('username'));
    console.log(userName);
    
};

// message handlers
socket.on('created', function (room) {
    navigator.mediaDevices.getUserMedia(streamConstraints).then(function (stream) {
        localStream = stream;
        
        localVideo.srcObject = stream;
        localVideoText.innerHTML = ` <span class="dot"></span> ${userName}`;
        localVideoText.style.color="rgb(125, 255, 3)"
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
        localVideoText.innerHTML = ` <span class="dot"></span> ${userName}`;
        localVideoText.style.color="rgb(125, 255, 3)"
        socket.emit('ready', {
            rn: roomNumber,
            id: socketId,
            username: userName
        }
        );
    }).catch(function (err) {
        console.log('An error ocurred when accessing media devices', err);
    });
    isChannelReady = true;
    socketId = socket.id;
    console.log("joined "+socketId);

});

socket.on('message', function(event) {
    textMessages.push({
        username: event.username,
        message: event.message
    });
    console.log(textMessages);
    
    var msg = document.createElement("div");
    msg.classList.add("talk-bubble-other","tri-left","round","left-top");
    var textArea = document.createElement("div");
    textArea.classList.add("talktext");
    textArea.innerHTML = `<p style="color: white">${event.username}</p><p style="color: white">${event.message}</p>`;

    newMessage.value = "";
    msg.appendChild(textArea);
    chatViewList.appendChild(msg);
    
});




socket.on('end_connection', function(event) {
    var indx = Client_IDs.indexOf(event.id);
    rtcPeerConnection[indx].close();
    document.querySelector("#"+remoteVideo[indx].id+"div").remove();
    console.log("#"+remoteVideo[indx].id+"div");
    
    Client_IDs.splice(indx, 1);
    Client_Uname.splice(indx, 1);
    rtcPeerConnection.splice(indx, 1);
    remoteVideo.splice(indx, 1);

    Dish();
    updateParticipants()
});


socket.on('candidate', function (event) {
    if(!Client_IDs.includes(event.from)){

        Client_IDs.push(event.from);
        Client_Uname.push(event.username);

        var candidate = new RTCIceCandidate({
            sdpMLineIndex: event.label,
            candidate: event.candidate
        });
        rtcPeerConnection[length].addIceCandidate(candidate);
        console.log("recieved candidate from: jjj "+event.username);
    }
    
    
});

socket.on('ready', function (event) {
    console.log("Ready Here: "+event.id+" "+socketId+" "+event.username);

        if (event.id!=socketId) {
            

            add("remoteVideo"+count);      
            remoteVideo.push(document.querySelector("#remoteVideo"+count));
            remoteVideoText.push(document.querySelector("#remoteVideo"+count+"text"));

            remoteVideoText[count].innerHTML = ` <span class="dot"></span> ${event.username}`;
            remoteVideoText[count].style.color="rgb(125, 255, 3)";


        count++;
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
                        to: t,
                        username: userName
                    })
            
                console.log('to: ttttt '+temp);
                }
            };

            rtcPeerConnection[length].ontrack = function (event, t = temp) {
                
                remoteVideo[length].srcObject = event.streams[0];
                console.log('got user media');
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
                        id: socketId,
                        username: userName
                    });
                })
                .catch(error => {
                    console.log(error)
                });
            
            console.log(rtcPeerConnection[length]);
        }
    // });
});

socket.on('offer', function (event) {
    console.log("Offer Here: "+event.id+" "+socketId+" "+event.username);
    console.log("Creating Answer ");
    if (!isCaller) {
        if(!Client_IDs.includes(event.id)){

            Client_IDs.push(event.id);
            Client_Uname.push(event.username);

            add("remoteVideo"+count);      
            remoteVideo.push(document.querySelector("#remoteVideo"+count));
            remoteVideoText.push(document.querySelector("#remoteVideo"+count+"text"));

            
            remoteVideoText[count].innerHTML = ` <span class="dot"></span> ${Client_Uname[count]}`;
            remoteVideoText[count].style.color="rgb(125, 255, 3)";

            count++;
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
                    to: t,
                    username: userName
                })
        
            console.log('to: ttttt '+temp);
            }
        };

        var ind = Client_IDs.indexOf(event.id);
        rtcPeerConnection[ind].ontrack = function(event, t = ind) {
            remoteVideo[t].srcObject = event.streams[0];
            console.log(Client_Uname[t]);
            console.log(remoteVideoText[t]);
            console.log('got user media');
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
    console.log('Answer Recieved '+length);
    rtcPeerConnection[length].setRemoteDescription(new RTCSessionDescription(event.sdp));
})




window.addEventListener('beforeunload', function (e) {
    // e.preventDefault();
    // e.returnValue = '';
    localStream.getTracks()[0].enabled = false;
    localStream.getTracks()[1].enabled = false;

    socket.emit('end_connection', {
            id: socketId, 
            rn: roomNumber
        });
    
    while(rtcPeerConnection.length!=0){
        rtcPeerConnection[0].close();
        rtcPeerConnection.shift();
    } 
});

function toggleChat() {
    var state = document.querySelector("#entire-chat").style.visibility;
    console.log(state);
    if(state === "hidden"){

        if(document.querySelector("#entire-participants").style.visibility === "visible"){
            document.querySelector("#entire-participants").style.visibility = "hidden";
            document.querySelector("#participants-icon").classList.toggle("fa-user");
            document.querySelector("#participants-icon").classList.toggle("fa-user-slash");

            if(document.querySelector("#participants-text").innerText === "Show Participants"){
                document.querySelector("#participants-text").innerText = "Hide Participants";
            }
            else{
                document.querySelector("#participants-text").innerText = "Show Participants";
            }
        }

        document.querySelector("#entire-chat").style.visibility = "visible";
    }
    else{
    document.querySelector("#entire-chat").style.visibility = "hidden";
    }
    chatIcon.classList.toggle("fa-comment");
    chatIcon.classList.toggle("fa-comment-slash");

    if(document.querySelector("#chat-text").innerText === "Show Chat"){
        document.querySelector("#chat-text").innerText = "Hide chat";
    }
    else{
        document.querySelector("#chat-text").innerText = "Show Chat";
    }

}

function updateParticipants(){
    var participant_list = document.querySelector("#participants");
    while(participant_list.children.length>0){
        participant_list.children[0].remove();
    }
    let hr1 = document.createElement("hr");
    participant_list.appendChild(hr1);

    for(var i = 0; i<Client_Uname.length;i++){
        let Div = document.createElement("div");
        let ele = document.createElement("p");
        let hr = document.createElement("hr");
        ele.innerHTML = ` &nbsp<span class="dot"></span> &nbsp ${Client_Uname[i]}`;
        ele.style.color="white";
        ele.style.padding = "10px";
        Div.appendChild(ele);
        Div.appendChild(hr);
        participant_list.appendChild(Div);
    }
}



startUp();