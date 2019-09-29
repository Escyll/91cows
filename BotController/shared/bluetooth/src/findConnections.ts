import { BluetoothSerialPort } from "bluetooth-serial-port";

const btSerial = new BluetoothSerialPort();

btSerial.on("found", (address: string, name: string) => {
    btSerial.findSerialPortChannel(
        address,
        channel => {
            btSerial.connect(
                address,
                channel,
                () => {
                    console.log("connected");
                    console.log(
                        `Name: ${name}, Channel: ${channel}, Address: ${address}`
                    );

                    btSerial.on("data", (buffer: Buffer) => {
                        console.log(buffer.toString("utf8"));
                    });

                    btSerial.write(Buffer.from("f:1\n", "utf-8"), err => {
                        if (err) {
                            console.log(`Error occured while sending: ${err}`);
                        }
                    });
                },
                () => {
                    console.log("cannot connect");
                }
            );

            // close the connection when you're ready
            // note that we only can close the connection once we did sent all the data
            // otherwise we get errors upon sending
            // btSerial.close();
        },
        () => {
            console.log("found nothing");
        }
    );
});

btSerial.inquire();
