
Launch Docker Container: 
Run the console launch script with the command: ./rvconsole.sh in the correct directory that contains riscv-cartridge rand iscv-firmware.

Makefile:
Once the container is launched you see the prompt, change directory into riscv-cartridge and make file by using the command make
Also change directory into riscv-firmware and make file by using the command make

Launch the Simulator:
Launch the simulator with the full path using the command /code/runsim.sh

Power on and Load to Firmware and Cartridge:
Pushing PWR button on the simulator. 
Hitting the Firmware button and navigating to the riscv-firmware/bin and loading the strip.
Then hitting the Cartridge button and navigating to the riscv-cartridge/bin and loading the strip. The Welcome with game rule message should appear on the screen.

Game Playing Instruction: 
Press the w, a, d, or x buttons to start the game.
It displays three bricks on the screen, the one in different colors is a large sprite that the player controls, other two in pink are target bricks (one is a small sprite, the other one is a medium sprite).
Press the w, a, d, or x buttons to control the large sprite brick to catch the other two bricks.
Catch as many target bricks as possible to get a higher score.
The game will end in 60 seconds, and it shows an end text including the final score and the next step.

