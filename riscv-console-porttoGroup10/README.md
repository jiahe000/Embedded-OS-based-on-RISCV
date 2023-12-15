
Launch Docker Container: 
Run the console launch script with the command: ./rvconsole.sh in the correct directory that contains base_game.

Makefile:
Once the container is launched you see the prompt, change directory into base_game/cartridge and make file by using the command make

Launch the Simulator:
Launch the simulator with the full path using the command /code/runsim.sh

Power on and Load to Firmware and Cartridge:
Pushing PWR button on the simulator. 
Hitting the Firmware button and loading riscv-console-example.strip provided by group 10 (in the same directory as README). The OS STARTED message should appear on the screen.
Then hitting the Cartridge button and navigating to the base_game/cartridge/bin and loading the strip. The Welcome with game rule message should appear on the screen.

Game Playing Instruction: 
Press any of the w, a, d, and x buttons to start the game.
It displays Backgroud and three squares on the screen, the one in white is a large sprite that the player controls, other two (one medium sprite in pink and one small sprite in yellow) are target bricks.
Press the w, a, d, or x buttons to control the large sprite brick to catch the other two bricks.
The game will end in around 20 seconds, and it shows an end text Game is Over.
