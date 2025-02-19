const mongoose = require('mongoose')

async function connect () {
    try {
        await mongoose.connect('mongodb://localhost:27017/RFID');
        console.log('Connected successfully') 
    } 
    catch (error) {
        console.log('connected failed');
    }
}
module.exports = {connect}
