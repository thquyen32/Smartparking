const { default: mongoose } = require("mongoose");

const Schema = mongoose.Schema;

const IOT = new Schema({
  name: String,
  id: String,
});

module.exports = mongoose.model('IOT',IOT)
