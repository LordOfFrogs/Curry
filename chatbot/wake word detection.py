import pvporcupine
import os
from pvrecorder import PvRecorder

api_key = os.getenv('PICOVOICE_API_KEY')

porcupine = pvporcupine.create(
    access_key=api_key,
    keyword_paths=['/home/kuri/Kuri/chatbot/Hey-Kuri_en_raspberry-pi_v2_2_0.ppn']
)

recorder = PvRecorder(device_index=-1, frame_length=porcupine.frame_length)

try:
    recorder.start()
    
    while True:
        keyword_index = porcupine.process(recorder.read())
        if keyword_index >= 0:
            print("DETECTED")
except KeyboardInterrupt:
    recorder.stop()
finally:
    porcupine.delete()
    recorder.delete()
