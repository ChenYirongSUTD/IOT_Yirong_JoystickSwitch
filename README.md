# Directional Joystick On/Off switches!
Wouldn't it be cool if we could turn appliances on in a 'StarCraft deploy' fasion?!

## The analog joystick here is the main device. Whenever the user bumps the joystick in a certain direction and releases, it triggers to switch on the 'appliance' (simulated by the LED diodes) in the bumped direction.
This is achieved by:
1) Updating the status of the appliance on the database everytime the joystick is bumped
2) Reading from the database in fixed intervals, turning on/off based on the status read

There are many ways to turn on appliances, but rarely does it compare to the satisfaction of flicking a joystick in a chosen direction, to quite literally send out our instructions 😎
