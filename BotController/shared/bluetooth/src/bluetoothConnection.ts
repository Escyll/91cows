import { BluetoothSerialPort } from "bluetooth-serial-port";

export class BluetoothConnection {
    private readonly _address: string;
    private _btSerial: BluetoothSerialPort | undefined;
    private readonly _channel: number;

    constructor(address: string, channel: number) {
        // FIX: add precondition checks here

        this._address = address;
        this._channel = channel;
    }

    public connect(
        onConnected: () => void,
        onError: (error: Error | undefined) => void
    ): void {
        if (!this._btSerial) {
            this._btSerial = new BluetoothSerialPort();
            this._btSerial.connect(
                this._address,
                this._channel,
                onConnected,
                onError
            );
        }
    }

    public disconnect(): void {
        if (this._btSerial) {
            this._btSerial.close();
            this._btSerial = undefined;
        }
    }

    public send(
        message: string,
        onError: (error?: Error | undefined) => void
    ): void {
        if (!this._btSerial) {
            throw new Error("Please connect before sending stuff!");
        }

        this._btSerial.write(Buffer.from(`${message}\n`, "utf8"), onError);
    }
}
