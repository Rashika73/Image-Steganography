# Image-Steganography
This is an app to implement steganography technique in c++ by using LSB*(least significant bit)*
Steganography is the practice of communicating using hidden messages. 
The basic structure of Steganography is made up of three components: the carrier, the message, and the key. 
The carrier can be a text, a digital image, or media files.
 It is the object that will ‘carry’ the hidden message and the key is used to decode the hidden message.
## Use of Steganography:
To control data leaks.
To provide the better security for sharing data over internet. 
To hide the message without altering the data in such a way that others cannot discern the presence of hidden message.
To provide the better security for sharing data over internet. 
Algorithm:
## Data Encoding Algorithm:
- Step 1: Extract pixels of the cover image.
- Step 2: Extract characters of the message from respected file in the key.
- Step 3: Choose the first pixel, then pick characters of the key and place it in LSB of first few pixels.
- Step 4: Place some terminating symbol to indicate the end of key.
- Step 5: Insert characters of message in LSB of next component of the pixel.
- Step 6: Repeat step 5 until all the characters have been encoded.
## Data Extraction Algorithm:
- Step 1: Extract the pixels of stego image.
- Step 2: Start from first pixel and check for the key, if the key entered by the user matches the encoded key, then follow step 3.
- Step 3: Go to the next pixels and extract the message characters from LSB.
- Step 4: Repeat the step 3 until terminating symbol is found.
- Step 5: Store the extracted data and display it.


