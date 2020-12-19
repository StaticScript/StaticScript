#!/usr/bin/env bash

if [[ ! $(command -v clang) ]]; then
  sudo xcode-select --install
fi

if [[ ! $(command -v wget) ]]; then
  brew install wget
fi

sudo wget https://github.com/StaticScript/StaticScript/releases/latest/download/staticscript-macos-release.zip

sudo unzip staticscript-macos-release.zip

sudo rm -f staticscript-macos-release.zip

sudo mv staticscript-macos-release /usr/local/staticscript

sudo chmod +x /usr/local/staticscript/bin/staticscript

sudo ln -s /usr/local/staticscript/bin/staticscript /usr/local/bin/staticscript

sudo ln -s /usr/local/staticscript/bin/staticscript /usr/local/bin/ssc
