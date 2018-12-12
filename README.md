# VoiceSpirit

Voice Spirit is a voice assistant runs on raspberry pi 3. It's based on Google Assistant and SnowBoy. 
It can be actived by say wake word(hot word) jarvis.


# How to use it
```bash
make
cd build/output
./VoiceSpirit
```
Then just say "jarvis" and ask any question you want. For example:
"jarvis, how is the weather today"

# Requirements 
Hardware:
  -PI3 with a USB micphone
Thirdparty Libs:
  -portaudio
  -asound
  -snowboy
  -grpc
