import openai
import speech_recognition as sr
from gtts import gTTS
from pydub import AudioSegment
from pydub.playback import play
import os

openai.api_key = os.getenv("OPENAI_API_KEY")

messages = [ {"role": "system", "content": 
    """you are an intelligent assistant named Kuri"""} ]

r = sr.Recognizer()

def speakText(command):
    engine = gTTS(text=command)
    engine.save("output.mp3")
    p = AudioSegment.from_file("output.mp3")
    play(p)
    print("done speaking")

def getInput():
    # use the microphone as source for input.
    with sr.Microphone() as source:
        # wait for a second to let the recognizer
        # adjust the energy threshold based on
        # the surrounding noise level
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
    )
    chat = openai.ChatCompletion.create(
        model="gpt-3.5-turbo", messages=messages
    )
    
    reply = chat.choices[0].message.content
    print(f"Kuri: {reply}")
    messages.append(
        {"role": "assistant", "content": reply}
    )
    
    return reply

while(1):
    print("listening...", end=' ', flush=True)
    prompt = getInput()
    if prompt == -1:
        speakText("Sorry, I didn't get that")
        continue
    if prompt != 0:
        reply = getReply(prompt)
        speakText(reply)