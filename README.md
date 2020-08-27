# Image-Steganography

# Background

Image Steganography is a CLI application to implement steganography technique in c++ by using LSB(least significant bit).

> Image steganography refers to hiding information i.e. text, images or audio files in another image or video files. 

Steganography could be a secure communicating using hidden messages which will appear ordinary at first glance and not attract attention to themselves but within them is the hidden message. The basic structure of Steganography is made up of three components:
- The Carrier
- The Message
- The Key
The carrier used in this application is BMP image.

# Motivation
I found the concept behind Image Steganography intriguing. We often use social networking sites for communication but if we want to share confidential information then they can use this application to encode their message before sending it. Also implementing this concept not only demands coding skill but also the knowledge of binary operations and image processing and overall I found this as a great project to work on. 

# How this Application Works

1. Sender can upload an image.
2. Sender can encrypt the message.
3. Sender is provided with a key.
4. Using the same key, the receiver could access the message.

# Install Image Steganography Application
```bash
g++ -o <name-you-want-to-give> mainprog.cpp
```
            
# Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
