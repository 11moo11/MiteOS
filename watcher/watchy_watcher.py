import serial
import io
import datetime
import time
from PIL import Image, ImageTk
import tkinter as tk
import numpy as np

window = tk.Tk()

window.title("Watchy")
window.geometry("500x500")
window.configure(background='grey')

panel = tk.Label(window)
panel.pack(side = "bottom", fill = "both", expand = "yes")

def updateImg():
    global panel
    try:
        f = Image.open("buf.png")
        f = f.resize((500, 500))
        img = ImageTk.PhotoImage(f)
        panel.configure(image=img)
        panel.image = img
    except:
        print("No Image found yet")

def byte_array_to_booleans(byte_array):
    boolean_list = []
    
    for byte in byte_array:
        # Konvertiert das Byte in eine Binärdarstellung und füllt es mit führenden Nullen auf (8 Bits)
        bin_string = f"{byte:08b}"
        
        # Konvertiert jedes Bit in einen Boolean
        for bit in bin_string:
            boolean_list.append(bit == '1')
    
    return boolean_list

updateImg()

window.update()

ser = serial.Serial('COM12', baudrate = 115200)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

print("Waiting...")
seq = []
count = 1 ## row index
while True:
    if (ser.inWaiting() > 0):
        line = ser.readline().strip()
        line = line.decode('ascii')
        
        if line.startswith('screenshot|'):
            print("Screenshot")
            imageData = line.split("|")[1]
            imageData = byte_array_to_booleans(bytes.fromhex(imageData))
            
            print(len(imageData))
            data = np.zeros((200,200,3), dtype=np.uint8)
            for x in range(0, 200):
                for y in range(0, 200):
                    if imageData[x * 200 + y]:
                        #data[y, x] = (255, 236, 221)
                        data[y, x] = (255, 255, 255)
            
            im = Image.fromarray(data)
            im.save('buf.png')
            updateImg()
        else:
            print(line);
    window.update()