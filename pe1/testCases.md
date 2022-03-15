# Test cases for practical exercise 1

## Case 1:
1. A user enters the alarm terminal and inputs "s". A prompt appears, asking for a time in the format "DD:MM:YYYY HH:MM:SS". 
    - This happens every time. ✅
2. After the time is put in, a promt stating that the alarm is created will appear. 
    - If the input is wrong, a prompt saying "something went wrong" will appear, every time. ✅
    - On correct input, a prompt saying when the alarm will ring appears ✅
3. The user regrets making this alarm, and presses "c" for cancelling. 
    - This happens every time, if the input is correct ✅
4. A list of all alarms are displayed, and the user inputs "0", to delete alarm 0. 
    - If the user inputs "0", the alarm is deleted. ✅
    - If the user inputs "9", it will return to the menu ✅
    - If the user inputs anything but a number, the app will fail ❌
    - If the user inputs the number of an empty alarm, a prompt will say that this alarm is empty ✅
    
5. The alarm and associated child process is removed.
    - If input is correct, this will happen ✅



## Case 2:
1. The user wants to wake up at 8 AM and presses "s" for scheduling an alarm. ✅
    
2. The prompt appears and the user inputs "XX:XX:XX 08:00:00". (xx because the day can be anytime).✅
3. the user presses "l" to see the list. ✅
4. The list confirms the users expectation and shows an active alarm for XX:XX:XX 08:00:00 ✅
5. The alarm rings and the alarm is removed, with its child process ✅
## Case 3:
1. The user wants to check out the alarm system. 
2. Presses "l" to see that no alarms are saved from the beginning. ✅
3. Is pleased with this and presses "x" to exit the program.✅ 
4. The program stops and no entries were made.✅ 
## Case 4:
1. The user wants to check that it is possible having two alarms saved simultaneously. ✅ 
2. Schedules an alarm for "XX:XX:XX 08:00:00". ✅
3. "XX:XX:XX 08:30:00"✅
4. Inputs "l" to see that both alarms are listed. ✅
5. Waits until the time is 8 AM. ✅
6. Alarm 0 rings, and plays a sound. ❌ (The sound is a general system sound, because the OS is windows 64 and cannot play the saved sound in the /assets folder)
7. Inputs "l" to see that alarm 0 is empty and alarm 1 is set for 8.30 AM: ✅