from pynput import keyboard
import matplotlib.pyplot as plt
x = 0
y = 0
def on_press(key):
    if key == keyboard.Key.esc:
        plt.close()
        return False  
    try:
        k = key.char 
    except:
        k = key.name 
    if k in ['left', 'right','up',"down","space"]:
        global x,y
        if k == "left":
            x -= 1
        elif k == "right":
            x += 1
        elif k == "up":
            y += 1
        elif k == "down":
            y -= 1
        elif k == "space":
            plt.cla()
        point = plt.plot(x, y, "g^")
        plt.draw()

listener = keyboard.Listener(on_press=on_press)
listener.start() 
point = plt.plot(x, y, "g^")
plt.show()
listener.join()  