import time
import threading
import queue
import socket
import wave
import azure.cognitiveservices.speech as speechsdk

#user defines
UDP_IP = "192.168.178.119"
UDP_PORT = 5005

q = queue.Queue()

RECEIVE_LIST = []

def from_file():
    speech_config = speechsdk.SpeechConfig(subscription="7181a5a04645430b849d07f155314e4d", region="westeurope", )
    audio_input = speechsdk.AudioConfig(filename="sound.wav")
    speech_recognizer = speechsdk.SpeechRecognizer(speech_config=speech_config, audio_config=audio_input, language="nl-NL")
    
    result = speech_recognizer.recognize_once_async().get()

    print(result)
    return result.text


def MakeAudioFile(audio: bytearray):
    with wave.open("sound.wav", "wb") as out_f:
        out_f.setnchannels(1)
        out_f.setsampwidth(1) # number of bytes for 8 bit it is 1
        out_f.setframerate(16000)
        out_f.writeframesraw(audio)
        out_f.close()


def AddIP(IPmessage):
    IPaddress = ""
    for char in IPmessage:
        if char in b'0123456789.':
            IPaddress += chr(char)
    
    if IPaddress not in RECEIVE_LIST:
        RECEIVE_LIST.append(IPaddress)
    print(RECEIVE_LIST)


def send_data_to_receivers(sock, list_receive_devices, MESSAGE):
    for ipaddress in list_receive_devices:
        sock.connect((ipaddress, UDP_PORT))
        sock.send(str(MESSAGE).encode(),)
        sock.close()
        print("send to ",ipaddress)


def SendDataThread():
    sock = socket.socket()
    while True:
        while not q.empty():
            send_data_to_receivers(sock, RECEIVE_LIST, q.get())
            print("audio send")


def ReceiveDataThread():
    #connect to socket
    sock = socket.socket()

    sock = socket.socket(socket.AF_INET, # Internet
                        socket.SOCK_DGRAM) # UDP
    sock.bind((UDP_IP, UDP_PORT))

    audio = bytearray()

    #while loop
    while True:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        print(data)
        if data == b'start':
            audio = bytearray()
        elif data == b'end':
            MakeAudioFile(audio)
            audio = bytearray()
            print("audio received")
            #add string to queue
            q.put(from_file())
            
        elif b'IP' in data and len(data) < 50:
            print(data)
            AddIP(data)
            
        else:
            for  char in data: 
                audio.append(char)


R = threading.Thread(target=ReceiveDataThread)
R.start()
SendDataThread()

