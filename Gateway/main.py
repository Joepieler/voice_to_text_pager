from datetime import datetime
import socket
import wave
import azure.cognitiveservices.speech as speechsdk

#user defines
UDP_IP = "192.168.178.119"
UDP_PORT = 5005

RECEIVE_LIST = []

sock = socket.socket()

sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))


def from_file():
    speech_config = speechsdk.SpeechConfig(subscription="7181a5a04645430b849d07f155314e4d", region="westeurope", )
    audio_input = speechsdk.AudioConfig(filename="sound.wav")
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config, audio_config=audio_input, language="nl-NL")
    
    result = speech_recognizer.recognize_once_async().get()

    print(result)
    return result.text


def send_data_to_receivers(list_receive_devices, MESSAGE):
    for ipaddress in list_receive_devices:
        sock.sendto(MESSAGE, (ipaddress, UDP_PORT))


def Main():
    audio = bytearray()

    while True:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        if data == b'start':
            audio = bytearray()
            print("start received")
            now = datetime.now()
        elif data == b'end':
            print("Audio received")
            print("length of audio :", len(audio) / 16000 )
            with wave.open("sound.wav", "wb") as out_f:
                out_f.setnchannels(1)
                out_f.setsampwidth(1) # number of bytes
                out_f.setframerate(16000)
                out_f.writeframesraw(audio)
                out_f.close()
                audio = bytearray()

                messsage = from_file()

                send_data_to_receivers(RECEIVE_LIST, messsage)
            
        elif b'IP' in data:
            IPaddress = ""
            for char in data:
                if char in b'0123456789.':
                    IPaddress += chr(char)
            
            if IPaddress not in RECEIVE_LIST:
                RECEIVE_LIST.append(IPaddress)
            print(RECEIVE_LIST)
                
        else:
            print(data)
            for  char in data: 
                audio.append(char)


Main()