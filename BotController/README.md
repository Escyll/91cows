
# Folder Overview

The folder is a Rush mono-repository. It contains a number of modules and is structered as follows: the first
layer of folders are the categories and the second layer contains the modules. There are 4 categories, `build`,
`common`, `control`, and `shared`, where `build` contains build and rush related modules; `common` contains
Rush configuration and common scripts; `control`contains control related modules that can be used to control
the bot; and `shared` contains modules that can be shared among different `control` modules.

 | Module           | Description                                                                              |
 | ---------------- | ---------------------------------------------------------------------------------------- |
 | control-manual   | CLI application to control the bot and send commands.                                    |
 | shared-bluetooth | Abstraction layer to deal with the serial bluetooth connection and its discovery         |

# Bot Control

The bot can be controlled by sending command messages over a bluetooth serial connection. The bot will acknowledge
the sent command by printing the action it will perform. The following commands are available, which are also
printed upon start-up to the bluetooth serial connection:
- f:<1-999>     Move bot forward X number of units
- b:<1-999>     Move bot backward X number of units
- c:<1-999>     Rotate bot X units counter-clockwise
- w:<1-999>     Rotate bot X units clockwise

Note that the filled in units are not calibrated and especially the rotation needs better mapping from 1 unit send
to the rotation in real-life. Also the bot does not check if the numbers are within range and therefore it can
happen that sending a forward results in a rotation of the bot.

To connect to the HC-05 or HC-06 module of the bot, use the password '1234' when prompted.

## Using Serial Bluetooth Terminal

To test a connection and the calibration, you could install an mobile application and send the commands. An application
that can be used is the [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en_US).
The application just opens a bluetooth serial connection on which you can send the commands and verify the bots
movement.

## Using Putty

Commands to the bot can also be sent by using Putty. Once the bot is connected to you computer, check the device manager
(on Windows) and check your COM ports. Per bot 2 COM ports are expected, both are named "Bluetooth over Serial Link".
Probably the higher numbered of the two can be used to establish a serial connection. When starting Putty, copy over the
connection settings from the "properties" in the device manager to the Serial connection advanced settings in Putty.
Then press open connection and a serial terminal will be opened. In the terminal the feedback from the bot is printed,
however, the typed input is not visible. Sending the commands should be done "blindly".

Note that the connection using Putty is not stable and can be closed when for a longer period of time no commands are sent.

## Using the BotController

This folder contains a Typescript/Node implementation of a CLI that can be used to connect to the bot as well as discover the
bots address and channel.

### Starting with the BotController

1. Install Node.JS, version 10.15.3
2. `npm install -g @microsoft/rush`
3. Install the following VSCode extensions:
    - [EditorConfig](https://marketplace.visualstudio.com/items?itemName=EditorConfig.EditorConfig)
    - [DotENV](https://marketplace.visualstudio.com/items?itemName=mikestead.dotenv)
    - [Prettier](https://marketplace.visualstudio.com/items?itemName=esbenp.prettier-vscode)
    - [TSLint](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-typescript-tslint-plugin)
    - [YAML](https://marketplace.visualstudio.com/items?itemName=redhat.vscode-yaml)
    - [Paste JSON as Code](https://marketplace.visualstudio.com/items?itemName=quicktype.quicktype)
4. Copy the following contents to the settings file. TODO, which settings file
    ```json
    {
        "typescript.updateImportsOnFileMove.enabled": "always",
        "breadcrumbs.enabled": true,
        "terminal.integrated.shell.windows": "C:\\Program Files\\Git\\bin\\bash.exe",
        "[typescript]": {
            "editor.formatOnSave": true,
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        },
        "[json]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        },
        "[javascript]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        },
        "[html]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"
        },
        "[jsonc]": {
            "editor.defaultFormatter": "esbenp.prettier-vscode"1
        }
    }

    ```

### Installing node-bluetooth

The `node-bluetooth` package build requires the node-gyp package to build something natively on the platform.
That package required Python 2.7 and a few other pieces too. Simple problems sometimes get simple solutions.
Thanks to the node-gyp team, that is the case and they have documented it in the project README.
Using a PowerShell CLI instance with administrative privileges, and use the following code:
`npm install --global --production windows-build-tools`. And that's it.

### Finding a Bluetooth Connection

To determine the address of the Bluetooth controller for you bot, run the `findConnection` in the `shared-bluetooth`
project. Just run `rushx start` of the `shared-bluetooth` project and it starts searching for a connection. Once
connected it will print the channel and address (store this for later use) and it will send a "move forward 1"
command. This command allows you to see if the motors are correctly connected and which bot is connected to. If the
bot move backwards, the motor connections should be swapped (so swap left and right connector on the shield).

## Sending commands

To control a robot using manual control, run the `rushx start` of the `control-manual` project. Important is that
the correct environment file, named `.env` is present in the root of that project. An `example.env` is provided and
contains the different configuration parameters.

In the rare case that you want to connect to multiple bots at the same time, the above command will not work. Instead
you need to provide the environment variables via the command line. The command looks as follows and directly calls
the compiled `basicControl.js`: `BOT_CHANNEL="1" BOT_ADDRESS="<>" node ./lib/basicControl`. The command needs to be run
from the root directory of the `control-manual` project.
