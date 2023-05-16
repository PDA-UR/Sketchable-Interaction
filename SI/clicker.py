import pyautogui
import time

click_duration = 0.8 # seconds
mouse_up_duration = 0.2 # seconds
repetitions = 1

remaining_time = (click_duration + mouse_up_duration) * repetitions
num_clicks = 0

for i in range(repetitions):
    num_clicks += 1
    print(f"Click {num_clicks}/{repetitions}. Remaining time: {(repetitions - num_clicks) * (click_duration + mouse_up_duration)}")

    pyautogui.mouseDown()
    time.sleep(click_duration)
    pyautogui.mouseUp()
    time.sleep(mouse_up_duration)
