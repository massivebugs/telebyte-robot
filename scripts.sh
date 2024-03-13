#!/bin/bash

dev:build() {
    "$HOME"/.platformio/penv/Scripts/platformio.exe run -e dev
}

dev:upload() {
    "$HOME"/.platformio/penv/Scripts/platformio.exe run -e dev --target upload
}

prod:build() {
    "$HOME"/.platformio/penv/Scripts/platformio.exe run -e prod
}

prod:upload() {
    "$HOME"/.platformio/penv/Scripts/platformio.exe run -e prod --target upload
}

# Main
case "$1" in
    dev:build)
        dev:build
        ;;
    dev:upload)
        dev:upload
        ;;
    prod:build)
        prod:build
        ;;
    prod:upload)
        prod:upload
        ;;
    *)
        echo "Usage: $0 {env:command}"
        exit 1
        ;;
esac
