const express = require('express')
const morgan = require('morgan')
const path = require('path')
const app = express()
const port = 8483
const db = require('./server')
const IOT = require('./model/iot')

db.connect();
app.use(morgan('combined'))
app.get('/', async (req, res) => {
  try {
   var data = await IOT.find({})
   res.json(data)
  } catch (error) {
    res.status(500).send('Failed')
  }
})

app.listen(port,'0.0.0.0')
