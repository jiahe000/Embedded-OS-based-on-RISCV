CONTENT
--Introduction--
--Prerequisites--
--Building the Project--
--Expected Performance--
--Existing Problem--



--Introduction--
This is the ported game cartridge of Group5 to the OS developed by Group6.
The 'riscv-console-firmware.strip' is the OS of Group6, which is used as the firmware.
The folder 'Group5_cartridge' is the folder of our ported cartridge, which includes the source code of the cartridge. The user should make it in this directory before use.



--Prerequisites--
The riscv-simulator should be ready-to-use beforehand.



--Building the Project--
Step 1: Build the Project
cd Group5-Group6
cd Group5_cartridge
make

Step 2: Start the simulator
/code/runsim.sh

Step 3: Choose firmware
Click on the "firmware" button, choose 'riscv-console-firmware.strip'.

Step 4: Choose cartridge
Click on the "cartridge" button, choose either file in the 'Group5_cartridge/bin/' directory.

Step 5: Start Game
Click on the "PWR" button



--Expected Performance--
When the game starts:
A timer will automatically begin its counting down.
3 blocks appear on the screen, where the red one with the dark red border is the player, and the other 2 are the targets. 
The player should control the red block by a/w/d/x to "eat" the other 2 blocks to get points. 

During playing:
When a target is eaten, it will disappear and randomly appear at somewhere else.
Meanwhile, the player gets 1 point.
When 'CMD' is pressed, the game will pause, and show the remaining time and your current score.
(p.s. You can notice that... the player block looks different when you press different control buttons)

When time's up:
The game will stop and show your final score.
There will be a countdown timer to automatically restart the game.
You can also press 'RST' to manually restart.


--Existing Problem--
1. MediumSprite and LargeSprite: Not Available.
Group 6 didn't provide the applicable API for either medium or large sprite operations.
So we use small sprites to achieve the functions requiring different sprite sizes.

2. Color API: couold be more flexible
The APIs provided by Group 6 for color settings do not allow us to draw any pattern in the sprites. We can only assign one color to each sprite, and the whole sprite will show this color.
