#!/bin/sh
IP_ADDR="192.168.1.50"
scp ${pwd}out/VoiceSpirit_debug pi@$(IP_ADDR):~/Desktop/VoiceSpirit/bin/
ssh pi@$(IP_ADDR) "cd ~/Desktop/VoiceSpirit/bin/ ; chmod 777 VoiceSpirit_debug ; gdbserver :1234 ./VoiceSpirit_debug &> /dev/pts/0 &"
echo "Deploy done"
