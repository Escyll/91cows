

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