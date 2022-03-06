import pcbnew

SCALE = 1000000

board = pcbnew.GetBoard()

led_to_switch = [
	16,15,14,13,12,11,10,9,8,24,7,6,5,21,4,3,2,18,1,
	17,34,35,19,20,37,38,22,23,40,41,25,26,27,28,29,30,31,32,
	64,48,47,62,46,45,60,44,43,42,57,56,39,55,54,53,36,52,51,50,33,
	49,66,67,68,84,69,70,71,72,88,73,58,59,75,76,61,77,78,79,63,
	80,96,95,94,91,74,89,104,103,87,86,85,100,99,83,82,65,
	81,98,115,116,101,102,118,119,120,105,90,107,108,92,93,110,111,112,
	128,127,126,109,125,124,123,122,106,121,117,114,113,97
]

for led_idx in range(1, 128+1):
	switch_idx = led_to_switch[led_idx-1]

	switch = board.FindFootprintByReference('SW%d' % switch_idx)
	diode = board.FindFootprintByReference('D%d' % switch_idx)
	led = board.FindFootprintByReference('D%d' % (128+led_idx))

	pos = switch.GetPosition();

	print('Placing %s and %s relative to %s' % (diode.GetReference(), led.GetReference(), switch.GetReference()))

	diode.SetPosition(pcbnew.wxPoint(pos.x - (6.35*SCALE), pos.y - (1.905*SCALE)))
	diode.SetOrientation(0)

	led.SetPosition(pcbnew.wxPoint(pos.x-(2.54*SCALE), pos.y + ((5.08+5.05)*SCALE)))
	led.SetOrientation(0)

# exec(open('C:\\Code\\type-k-interface-02\\pcb\\place_diodes.py').read())