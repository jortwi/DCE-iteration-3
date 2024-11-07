# DCE-iteration-3

A chatbot example with physical embodiment for TU/e Industrial Design's Master course Designing Conversational Experiences. The example builds on an earlier iteration, and adds a light to indicate thinking, a button to re-do an AI response, and a piezo sensor to indicate emotion to the AI.

This example uses PicoCSS, a CSS framework that helps with simplicity. https://picocss.com/
In addition, it uses the AI Foundry Library that build on Data Foundry functionality. For information on how to use this library, take a look at the Github documentation.
The library: https://github.com/jortwi/AI-Foundry-Library
Data Foundry: https://data.id.tue.nl/

Finally, this iteration uses OOCSI to communicate between ESP32 and the web client. For more information: https://oocsi.id.tue.nl/

# How to use

Download these files, and change the `api_token` in the .html file to your Data Foundry API key, which can be generated in the project settings of any Data Foundry project. Then, take the ESP32 code, and fill in your wifi credentials. Make sure this wifi is 2.4 Ghz. If you don't have access to such a network, try creating a mobile hotspot. For this, you may have to manually turn on the 2.4Ghz mobile hotspot, depending on you device. Finally, it is adviced to change the OOCSI channel name in both the ESP32 code and the html file. Make sure these channel names are the same.

# The embodiment

The AI is embodied in a very simple manner. When the AI process is initiated, an OOCSI message is sent "LED_ON", which is received by ESP32, which then turns on the LED. When "LED_OFF" is received, the LED will be turned off again. When the button is pressed, ESP32 sends a message to the web client ("Button pressed"). When this message is received, the previous message is erases and a new answer is generated. This only works at question 4 and 5 as these are the questions where the answer is generated by AI. Finally, the piezo value is measured, which is transfered to an emotion. Very soft touch is considered distant, which is interpreted as "unfriendly". Medium touch is regular, and a firm press extreme. What exactly this means is interpreted by the AI. These words describing the emotion are directly plugged into the prompt, leading to a different result. Note that stopping to press the piezo will also lead the signal to go down, leading to a different response than you may expect. In addition, OOCSI on ESP may repond a bit slow, which can lead to a different piezo value to be used than intended.

### Image of a possible object physicalizing these elements

![Embodiment example](./Embodiment_example.png)

# Iteration 4

If you would like to build further upon this and include multimodel capabilities, you are adviced to take a look at the AI Foundry Library (https://github.com/jortwi/AI-Foundry-Library). An example could be to record audio and transcribe it using `foundry.soundToText()`, then sending this text to AI for a response by using `foundry.textToText()`, and then generating an image based on this response using `foundry.textToImage()`.

# Functions

Different reusable functions are used:

`sendMessage()` to 'send' user input, and provide a response

`userChoice(choice)` to provide a different response for each of the three options provided

`addMessage(type, input)` to place add the messages to the screen. For `type`, choose between `'user'` or `'assistant'`. This will place the message either to the right or left side of the screen. for `input`, provide the text to be placed on the screen.

`OOCSImessage(input)` sends the input to the selected OOCSI channel

If you want to save a message to a variable, you can do so as soon as it is received, such as is done with the `hobby` variable (`hobby = msg`). In addition to this, you have access to the complete message history through the `messageHistory` array. Retrieving that same hobby is possible by running `messageHistory[1].content`.
