

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
Using a PowerShell CLI instance with administrative priviledges, and use the following code:
`npm install --global --production windows-build-tools`. And that's it.

## Bluetooth Connection

To determine the address of the Bluetooth controller for you bot, run the `findConnection` in the `shared-bluetooth`
project. Just run `rushx start` of the `shared-bluetooth` project and it starts searching for a connection. Once
connected it will print the channel and address (store this for later use) and it will send a "move forward 1"
command. This command allows you to see if the motors are correctly connected and which bot is connected to. If the
bot move backwards, the motor connections should be swapped (so swap left and right connector on the shield).

## Sending commands

To control a robot using manual control, run the `rushx start` of the `control-manual` project. Improtant is that
the correct environment file, named `.env` is present in the root of that project. An `example.env` is provided and
contains the different configuration parameters.

In the rare case that you want to connect to multiple bots at the same time, the above commadn will not work. Instead
you need to provide the environment veriables via the command line. The command looks as follows and directly calls
the compiled `basicControl.js`: `BOT_CHANNEL="1" BOT_ADDRESS="<>" node basicControl`. The command needs to be run from
the root directory of the `control-manual` project.