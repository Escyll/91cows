import "dotenv/config";
import * as prompts from "prompts";

import { BluetoothConnection } from "@91cows/shared-bluetooth";

const BOT_ADDRESS = process.env.BOT_ADDRESS || "";
const BOT_CHANNEL = parseInt(process.env.BOT_CHANNEL || "-1", 10);
const connection = new BluetoothConnection(BOT_ADDRESS, BOT_CHANNEL);

async function setup(): Promise<void> {
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

async function processCommands(): Promise<void> {
    let keepProcessing = true;
    while (keepProcessing) {
        const answer = await prompts({
            type: "text",
            name: "command",
            message: "Next command [n means stop]:",
        });
        const command = answer.command as string;
        switch (command.toLowerCase()) {
            case "n":
                console.log("OK, will disconnect");
                connection.disconnect();
                keepProcessing = false;
                break;
            default:
                connection.send(command, error => {
                    if (error) {
                        console.log(
                            `Error occured sending message: ${error} for command ${command}`
                        );
                    }
                });
        }
    }
}

async function main(): Promise<void> {
    await setup();
    await processCommands();
}

main()
    .then(() => console.log)
    .catch(console.error);
