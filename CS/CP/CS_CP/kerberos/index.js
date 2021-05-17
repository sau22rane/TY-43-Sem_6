//mongodb+srv://TY-43:<password>@cluster0.wyvaa.mongodb.net/myFirstDatabase?retryWrites=true&w=majority
var express = require('express')
var bodyParser = require('body-parser')
var mongoose = require('mongoose')
require('dotenv/config');

const app3 = express();
var http = require('http').Server(app3);
var io = require('socket.io')(http);
var authenticationRoutes = require('./routes/authenticationRoutes.js');
var ticketGeneratorRoutes = require('./routes/ticketGenerator.js');


let records = [];
const app1 = express();
const app2 = express();
const app = express();

const PORT = 2000;
const PORT1 = 5000;
const PORT2 = 6001;
const PORT3 = 8080;
var cors = require('cors');

app.use(cors());
app1.use(cors());
app2.use(cors());

app1.use(bodyParser.json());
app.use(bodyParser.json());
app1.use('/authentication',authenticationRoutes);

app2.use(bodyParser.json());
app2.use('/ticketGeneration',ticketGeneratorRoutes);

app1.get('/',(req,res) => {
    res.send("Authentication server running fine");
});
app2.get('/',(req,res) => {
    res.send("Ticket Generating server running fine");
});

mongoose.connect(process.env.DB_CONNECTION,{ useNewUrlParser: true },() => {
    console.log("DB connected");
});

app1.listen(PORT1,() => console.log(`Server runinng on port : http://localhost:${PORT1}`));
app2.listen(PORT2,() => console.log(`Server runinng on port : http://localhost:${PORT2}`));
app.listen(PORT,() => console.log(`Server runinng on port : http://localhost:${PORT}`));

app3.use(cors());
app3.use(express.static('public'));
http.listen(PORT3 || 8000, function () {
    console.log('listening on', PORT3);
});