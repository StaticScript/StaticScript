#!/usr/bin/env bash

sudo apt-get -y install clang wget unzip

sudo wget https://github.com/StaticScript/StaticScript/releases/latest/download/staticscript-ubuntu-release.zip

sudo unzip staticscript-macos-release.zip

sudo rm -f staticscript-macos-release.zip

sudo mv staticscript-macos-release /usr/local/staticscript

sudo chmod +x /usr/local/staticscript/bin/staticscript

sudo ln -s /usr/local/staticscript/bin/staticscript /usr/local/bin/staticscript

sudo ln -s /usr/local/staticscript/bin/staticscript /usr/local/bin/ssc
