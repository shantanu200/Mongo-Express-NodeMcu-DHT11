import express from "express";
import cors from "cors";
import mongoose from "mongoose";

const app = express();


app.use(express.json());
app.use(express.urlencoded());
app.use(cors());

mongoose.connect("mongodb://127.0.0.1:27017/dht11sensorlog",{
    useNewUrlParser: true,
    useUnifiedTopology: true,
}).then(()=>{
    console.log("DB CONNECTED");
});


const sensorSchema = mongoose.Schema({
    temperature : String,
    humidity : String
});

const Sensor = new mongoose.model("Sensor",sensorSchema);

var datavar = [];

app.get("/fetchdata",(res,req)=>{
    Sensor.find({},(err,docs)=>{
        if(err){
            res.json(err);
        }else{
            req.json(docs);
        }
    });

})


app.post("/sensordata",(req,res)=>{
    console.log(req.body);
    const {temperature,humidity} = req.body;
    if(temperature !== '' && humidity !== ''){
        const sensor = new Sensor({
            temperature,
            humidity
        });
        sensor.save().then((sensor)=>{
            res.json({
                message:"New Data is added"
            });
        })
        .catch((err)=>{
            res.json({
                message:"err"
            });
        });
    }else{
        res.json({
            message : "error value"
        })
    }

});

app.listen(5000,()=>{
    console.log("On port 5000");
})



