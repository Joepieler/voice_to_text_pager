import socket
import wave
import azure.cognitiveservices.speech as speechsdk


def from_file():
    speech_config = speechsdk.SpeechConfig(subscription="-", region="-", )
    audio_input = speechsdk.AudioConfig(filename="sound.wav")
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config, audio_config=audio_input, language="nl-NL")
    
    result = speech_recognizer.recognize_once_async().get()
    print(result.text)



UDP_IP = "192.168.178.119"
UDP_PORT = 5005

sock = socket.socket()

sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

audio = bytearray()

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    if data == b'start':
        audio = bytearray()
    elif data == b'end':
        print("Audio received")
        with wave.open("sound.wav", "wb") as out_f:
            out_f.setnchannels(1)
            out_f.setsampwidth(1) # number of bytes
            out_f.setframerate(16000)
            out_f.writeframesraw(audio)
            out_f.close()
            audio = bytearray()
            from_file()
    else:
        print(data)
        for  char in data: 
            audio.append(char)