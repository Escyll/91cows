import "dotenv/config";
import { Server, IncomingMessage, ServerResponse } from "http";
import * as url from "url";

import { BluetoothConnection } from "@91cows/shared-bluetooth";

const BOT_ADDRESS = process.env.BOT_ADDRESS || "";
const BOT_CHANNEL = parseInt(process.env.BOT_CHANNEL || "-1", 10);
const LOCALHOST_PORT = parseInt(process.env.LOCALHOST_PORT || "4242", 10);

const connection = new BluetoothConnection(BOT_ADDRESS, BOT_CHANNEL);
const server = new Server(handleRequest);

async function setup(): Promise<void> {
    server.on("error", console.log);
    server.on("connection", socket => {
        console.log(
            `client connected on ${socket.remoteAddress} on port ${
                socket.remotePort
            }`
        );
    });

    return new Promise((resolve, reject) => {
        connection.connect(
            () => {
                console.log("Connected!!");
                resolve();
            },
            error => {
                console.log("Failed to connect! ;-(");
                reject(`Failed to connect! ;-( ${error}`);
            }
        );
    });
}

function handleRequest(req: IncomingMessage, res: ServerResponse): void {
    res.writeHead(200, { "Content-Type": "text/html" });

    if (!req.url) {
        res.end(
            "Provide a url when connecting in the format /?direction=<>&amount=<>"
        );
        return;
    }

    const query = url.parse(req.url, true).query;

    if (!query.direction || !query.amount) {
        res.end(
            "Provide a url when connecting in the format /?direction=<>&amount=<>"
        );
        return;
    }

    const command = `${query.direction}:${query.amount}`;
    console.log(`command received: ${command}`);
    res.end(command);
    try {
        connection.send(command, console.log);
    } catch (err) {
        console.log(err);
    }
}

async function main(): Promise<void> {
    await setup();
    server.listen(LOCALHOST_PORT);
    console.log(
        `server started and listening on port ${LOCALHOST_PORT} and bluetooth connection made  with ${BOT_ADDRESS}`
    );
}

main()
    .then(console.log)
    .catch(console.error);
