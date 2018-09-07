#!/bin/sh
IP_ADDR="192.168.155.2"
PROJECT="VoiceSpirit"
EXE_DIR="${pwd}build/output/${PROJECT}_debug"
DEPLOY_DIR="~/Desktop/${PROJECT}/bin/"
ssh pi@${IP_ADDR} "[ -d ${DEPLOY_DIR} ] || mkdir -p ${DEPLOY_DIR}"
scp ${EXE_DIR} pi@${IP_ADDR}:~/Desktop/VoiceSpirit/bin/
ssh pi@${IP_ADDR} "cd ~/Desktop/VoiceSpirit/bin/ ; chmod 777 VoiceSpirit_debug ; gdbserver :1234 ./VoiceSpirit_debug &> /dev/pts/0 &"
echo "Deploy done"
