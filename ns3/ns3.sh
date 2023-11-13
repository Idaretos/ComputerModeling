#!/bin/zsh

filename=$1

cp ${filename} /Users/kevin/vscode/cpp/_include/ns-allinone-3.36.1/ns-3.36.1/${filename}
cd /Users/kevin/vscode/cpp/_include/ns-allinone-3.36.1/ns-3.36.1
./ns3 run scratch/${filename}
