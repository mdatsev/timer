const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const port = new SerialPort('/dev/ttyUSB0', { baudRate: 9600 })

const parser = new Readline()
port.pipe(parser)
const times = []
parser.on('data', line => {
    times.push(parseFloat(line))
    console.log(
`
${'\n'.repeat(100)}
TOP 10:
${times.sort((a, b) => a - b).slice(0, 10).join('\n')}
`
    )
})