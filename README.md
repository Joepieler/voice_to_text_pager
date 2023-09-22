# Voice to Text pager
## Made By Bryan Campagne for Capgemini for graduation.

## Working System
The system consists of 3 parts. these are the transmitter base station and receiver. These parts communicate with each other via wifi. When the red button is pressed, a voice recording is made until the button is released. As soon as the button is released, the voice message is sent to the base station. The base station receives the message and sends it to the IBM whatson speech to text API. The translated text is shared with the recipient. The receiver shows this text on the screen.
## Get started

The base station must be connected to the Access Point via the internet cable. The access point must be started first and then the raspberry pi. The script on the raspberry starts automatically. If it is started then the sender and receiver can be started. Wait until the Led is green on the sender and the screen on the receiver says Ready. The red button on de sender can now be pressed.
