import time
import threading
import queue
import socket
import wave
from google.cloud import speech

#user defines
UDP_IP = "192.168.0.100"
UDP_PORT = 5005

q = queue.Queue()

RECEIVE_LIST = []

def SpeechToText(file_name):
    import json
    from os.path import join, dirname
    from ibm_watson import SpeechToTextV1
    from ibm_watson.websocket import RecognizeCallback, AudioSource
    import threading
    from ibm_cloud_sdk_core.authenticators import IAMAuthenticator

    authenticator = IAMAuthenticator('2RMY4vtdyVuRiQRqDKQ5vq-9fbcHNXJrOokjcqA0t24M')
    service = SpeechToTextV1(authenticator=authenticator)
    service.set_service_url('https://api.eu-de.speech-to-text.watson.cloud.ibm.com/instances/7e8f87b9-fa29-4784-91ed-047262e70999')
    
    model = service.get_model('nl-NL_BroadbandModel').get_result()
    # print(json.dumps(model, indent=2))
    with open(join(dirname(__file__), file_name),
            'rb') as audio_file:
        result = json.dumps(
            service.recognize(
                audio=audio_file,
                content_type='audio/wav',
                timestamps=True,
                model="nl-NL_BroadbandModel",
                word_confidence=True).get_result(),
            indent=2)
    print(result)
    try:
        return json.loads(result)['results'][0]['alternatives'][0]['transcript']
    except:
        return "No result"

def MakeAudioFile(audio: bytearray, address):
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


def send_data_to_receivers(list_receive_devices, MESSAGE):
    for ipaddress in list_receive_devices:
        sock = socket.socket()
        sock.connect((ipaddress, UDP_PORT))
        sock.send(str(MESSAGE).encode(),)
        sock.close()
        print("send to ",ipaddress)


def SendDataThread():
     while True:
    	 while not q.empty():
            send_data_to_receivers(RECEIVE_LIST, q.get())
            print("audio send")


def ReceiveDataThread():
    #connect to socket
    sock = socket.socket()

    sock = socket.socket(socket.AF_INET, # Internet
                        socket.SOCK_DGRAM) # UDP
    sock.bind((UDP_IP, UDP_PORT))

    audio_dict = {}
    audio = bytearray()

    #while loop
    while True:
        data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
        #print(data)
        if data == b'start':
            audio = bytearray()
        elif data == b'end':
            print("audio received")
            print("lenght of the audio {}".format(len(audio)/16000))
            if len(audio)/16000 > 1:
                MakeAudioFile(audio, addr)
                q.put(SpeechToText("sound.wav"))

            audio = bytearray()

        elif b'IP' in data and len(data) < 50:
            print(data)
            AddIP(data)
        else:
            for char in data:
                audio.append(char)


print("basisstation started")

R = threading.Thread(target=ReceiveDataThread)
R.start()
SendDataThread()
