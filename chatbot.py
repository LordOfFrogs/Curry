import openai
import speech_recognition as sr
import pyttsx3
import os
from subprocess import Popen 
import pvporcupine
from pvrecorder import PvRecorder
import warnings

warnings.filterwarnings('ignore')

pv_api_key = os.getenv('PICOVOICE_API_KEY')

# wakeword recognizer
porcupine = pvporcupine.create(
    access_key=pv_api_key,
    keyword_paths=['/home/kuri/Kuri/chatbot/Hey-Kuri_en_raspberry-pi_v2_2_0.ppn']
)

# passive recording
recorder = PvRecorder(device_index=-1, frame_length=porcupine.frame_length)

openai.api_key = os.getenv("OPENAI_API_KEY")

# initial message to gpt
messages = [ {"role": "system", "content": 
    """you are an intelligent assistant named Kuri"""} ]

r = sr.Recognizer()

engine = pyttsx3.init()
engine.setProperty('voice', 'mb-us1')
engine.setProperty('rate', 100)

def speakText(command):
    # play text to speech
    cmd = Popen(['espeak', '-g10', '-ven-us', command])
    
    recorder.start()
    while cmd.poll() == None:
        if wakeWord():
            recorder.stop()
            cmd.terminate()
            return 1
    recorder.stop()
    print("done speaking")


def getInput():
    # use the microphone as source for input.
    with sr.Microphone() as source:
        # r.adjust_for_ambient_noise(source, duration=1)
        
        #listens for the user's input
        audio = r.listen(source)
        print("done")
    try:
        # Using google to recognize audio
        prompt = r.recognize_google(audio)
        prompt = prompt.lower()

        print("Input: ", prompt)
            
        return prompt
    except sr.RequestError as e:
        print("Could not request results; {0}".format(e))
        return -1 
    except sr.UnknownValueError:
        print("unknown error occurred")
        return 0
    
def getReply(prompt):
    messages.append(
        {"role": "user", "content": prompt}
    ) # send prompt
    chat = openai.ChatCompletion.create(
        model="gpt-3.5-turbo", messages=messages
    ) # get reply
    
    reply = chat.choices[0].message.content
    print(f"Kuri: {reply}")
    messages.append(
        {"role": "assistant", "content": reply}
    ) # add reply to conversation
    
    return reply

# returns if wakeword detected
def wakeWord() -> bool:
    keyword_index = porcupine.process(recorder.read())
    return keyword_index >= 0

question = False
while(1):
    if not question:
        # wait for wake word
        recorder.start()
        print("waiting...")
        while not wakeWord():
            pass
        recorder.stop()
    
    print("listening...", end=' ', flush=True)
    prompt = getInput()
    if prompt == -1:
        speakText("Sorry, I didn't get that")
        continue
    if prompt != 0:
        reply = getReply(prompt)

        if speakText(reply) == 1:
            question = True
            continue
        question = reply[-1]=='?'
