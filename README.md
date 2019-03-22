# canboat-ejdb

Utility to store NMEA messages from the Canboat analyzer to ejdb.

# Install

Clone this repository to your local computer (or just copy the one and only .c file).

## Prerequisites

1. Install [EJDB](http://ejdb.org/doc/install/index.html), the small embeddable JSON document database. The NMEA messages will be stored here.
2. Clone the [Canboat](https://github.com/canboat) project and compile it to get the `actisense-serial` and `analyzer` binaries. In the examples below I have symlinked them to the project root foolder.

## Compile Me

```sh
gcc canboat-ejdb.c -o canboat-ejdb -lejdb
```

## Test

First verify that you get NMEA data from `actisense-serial`. Note that I'm using a Yacht Devices NMEA gateway which appears as `/dev/cu.usbmodem<somenumber>`. Your device will most likely have a different name.

```sh
$ ./actisense-serial /dev/cu.usbmodem00400124 
2019-03-22T15:55:28.844Z,0,262161,0,0,13,01,0e,00,fc,1a,06,00,00,00,00,00,02,00
2019-03-22T15:55:28.846Z,5,130312,52,255,8,ff,00,01,57,74,ff,ff,ff
2019-03-22T15:55:28.846Z,5,130311,52,255,8,ff,c1,57,74,ff,7f,ff,ff
```

A-OK. Next, try if piping it to `analyzer` produces good quality JSON.

```sh
$ ./actisense-serial /dev/cu.usbmodem00400124 | ./analyzer -json
{"timestamp":"2019-03-22T15:56:54.891Z","prio":0,"src":0,"dst":0,"pgn":262161,"description":"Actisense: Operating mode","fields":{"SID":1,"Model ID":14,"Serial ID":400124,"Error ID":0,"Operating Mode":2}}
{"timestamp":"2019-03-22T15:56:54.892Z","prio":5,"src":52,"dst":255,"pgn":130311,"description":"Environmental Parameters","fields":{"Temperature Source":"Outside Temperature","Temperature":24.87}}
```

Indeed it does!

Finally, add `canboat-ejdb` to the pipe to store all messages. You specify the database filename as the first argument. All messages will be stored in the `messages` collection in that file.

```sh
$ ./actisense-serial /dev/cu.usbmodem00400124 | ./analyzer -json | ./canboat-ejdb nmea.db
Opening database nmea.db
Read line {"timestamp":"2019-03-22T15:57:55.979Z","prio":0,"src":0,"dst":0,"pgn":262161,"description":"Actisense: Operating mode","fields":{"SID":1,"Model ID":14,"Serial ID":400124,"Error ID":0,"Operating Mode":2}}

Saved record with oid 5c95060371debd3b00000000
Read line {"timestamp":"2019-03-22T15:57:55.980Z","prio":5,"src":52,"dst":255,"pgn":130311,"description":"Environmental Parameters","fields":{"Temperature Source":"Outside Temperature","Temperature":24.93}}

Saved record with oid 5c95060371debd3b00000001
Read line {"timestamp":"2019-03-22T15:57:55.980Z","prio":5,"src":52,"dst":255,"pgn":130310,"description":"Environmental Parameters","fields":{"Outside Ambient Air Temperature":24.93}}
```

Whoa! It works!