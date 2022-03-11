import pcbnew

board = pcbnew.GetBoard()

indices = range(129, 256+1)
leds = [board.FindFootprintByReference('D%d' % i) for i in indices]

min_x = min([led.GetPosition().x for led in leds])
max_x = max([led.GetPosition().x for led in leds])
min_y = min([led.GetPosition().y for led in leds])
max_y = max([led.GetPosition().y for led in leds])

width = max_x - min_x
height = max_y - min_y

scale = width / 0xFF

for led in leds:
	pos = led.GetPosition();
	print('%s: %02x %02x' % (led.GetReference(), int((pos.x - min_x) // scale), int((pos.y - min_y) // scale)))

# exec(open('C:\\Code\\type-k-interface-02\\pcb\\find_leds.py').read())
