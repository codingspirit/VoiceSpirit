#!/bin/sh
IP_ADDR="192.168.50.50"
PROJECT="VoiceSpirit"
EXE_DIR="${pwd}build/output/${PROJECT}_debug"
DEPLOY_DIR="~/Desktop/${PROJECT}/bin/"
if [ "$1" == "zip" ]; then
    zip ${EXE_DIR}.zip ${EXE_DIR}
    ssh pi@${IP_ADDR} "[ -d ${DEPLOY_DIR} ] || mkdir -p ${DEPLOY_DIR}"
    scp ${EXE_DIR}.zip pi@${IP_ADDR}:~/Desktop/${PROJECT}/bin/
    ssh pi@${IP_ADDR} "cd ~/Desktop/${PROJECT}/bin/ ; unzip ${PROJECT}_debug.zip -y; chmod 777 ${PROJECT}_debug ; gdbserver :1234 ./${PROJECT}_debug &> /dev/pts/0 &"
else
    ssh pi@${IP_ADDR} "[ -d ${DEPLOY_DIR} ] || mkdir -p ${DEPLOY_DIR}"
    scp ${EXE_DIR} pi@${IP_ADDR}:~/Desktop/${PROJECT}/bin/
    ssh pi@${IP_ADDR} "cd ~/Desktop/${PROJECT}/bin/ ; chmod u+rwx ${PROJECT}_debug ; gdbserver :1234 ./${PROJECT}_debug &> /dev/pts/0 &"
fi
echo "Deploy done"
