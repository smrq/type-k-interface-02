import json
from ezdxf.addons import r12writer

infile = 'keyboard_layout.json'
outfile_plate = 'case_plate.dxf'
outfile_back = 'case_back.dxf'

def main(infile, outfile_plate, outfile_back):
	raw = get_layout_raw(infile)
	layout = parse_layout_data(raw)
	generate_dxf(layout, outfile_plate, outfile_back)
	print(f'{infile} => {outfile_plate}')
	print(f'{infile} => {outfile_back}')

def get_layout_raw(filename):
	with open(filename, encoding='utf-8') as f:
		raw = json.load(f)
		return raw

def parse_layout_data(raw):
	result = []

	meta = None
	if not isinstance(raw[0], list):
		meta, *raw = raw

	y = 0
	for row in raw:
		x = 0
		w = 1
		h = 1
		for item in row:
			if isinstance(item, str):
				label = item.replace('\n', ' ')
				result.append({'x': x, 'y': y, 'w': w, 'h': h, 'label': label})
				x += w
				w = 1
				h = 1
			else:
				if 'x' in item:
					x += item['x']
				if 'y' in item:
					y += item['y']
				if 'w' in item:
					w = item['w']
				if 'h' in item:
					h = item['h']
		y += 1

	return result

def draw_item(dxf, item):
	# draw_rect_cutout(dxf, item, (u_to_mm(item['w']), u_to_mm(item['h'])))
	if item['label'] == 'X7':
		draw_ec11_cutout(dxf, item)
	elif item['label'] == 'OLED':
		draw_oled_cutout(dxf, item)
	elif item['w'] == 6.25:
		draw_mx_horiz_stab_cutout(dxf, item, 50)
	elif item['w'] >= 2:
		draw_mx_horiz_stab_cutout(dxf, item, 11.9)
	elif item['h'] >= 2:
		draw_mx_vert_stab_cutout(dxf, item, 11.9)
	else:
		draw_mx_cutout(dxf, item)

def draw_mx_cutout(dxf, item):
	draw_rect_cutout(dxf, item, (14, 14))

def get_stabilizer_shape(s):
	# Based on: https://github.com/swill/kad/blob/1d51e0f167e1c043916f309b98e5a6c5e30bb918/key.go#L289
	return [
		(s - 3.375, 2.3),
		(s - 3.375, 5.53),
		(s + 3.375, 5.53),
		(s + 3.375, 2.3),
		(s + 4.2, 2.3),
		(s + 4.2, -0.5),
		(s + 3.375, -0.5),
		(s + 3.375, -6.77),
		(s + 1.65, -6.77),
		(s + 1.65, -7.97),
		(s - 1.65, -7.97),
		(s - 1.65, -6.77),
		(s - 3.375, -6.77),
		(s - 3.375, -2.3),
		(7, -2.3),
		(7, -7),
		(-7, -7),
		(-7, -2.3),
		(-s + 3.375, -2.3),
		(-s + 3.375, -6.77),
		(-s + 1.65, -6.77),
		(-s + 1.65, -7.97),
		(-s - 1.65, -7.97),
		(-s - 1.65, -6.77),
		(-s - 3.375, -6.77),
		(-s - 3.375, -0.5),
		(-s - 4.2, -0.5),
		(-s - 4.2, 2.3),
		(-s - 3.375, 2.3),
		(-s - 3.375, 5.53),
		(-s + 3.375, 5.53),
		(-s + 3.375, 2.3),
		(-7, 2.3),
		(-7, 7),
		(7, 7),
		(7, 2.3),
		(s - 3.375, 2.3),
	]

def draw_mx_horiz_stab_cutout(dxf, item, s):
	center_x, center_y = get_center(item)
	dxf.add_polyline_2d([(u+center_x, v+center_y) for (u, v) in get_stabilizer_shape(s)])

def draw_mx_vert_stab_cutout(dxf, item, s):
	center_x, center_y = get_center(item)
	dxf.add_polyline_2d([(-v+center_x, u+center_y) for (u, v) in get_stabilizer_shape(s)])

def draw_ec11_cutout(dxf, item):
	draw_rect_cutout(dxf, item, (12, 12))

def draw_oled_cutout(dxf, item):
	draw_rect_cutout(dxf, item, (19.05*2, 14))

def draw_rect_cutout(dxf, item, cutout):
	cutout_w, cutout_h = cutout
	center_x, center_y = get_center(item)
	dxf.add_polyline_2d([(u+center_x, v+center_y) for (u, v) in [
		(-cutout_w/2, -cutout_h/2),
		(-cutout_w/2,  cutout_h/2),
		( cutout_w/2,  cutout_h/2),
		( cutout_w/2, -cutout_h/2),
		(-cutout_w/2, -cutout_h/2),
	]])

def draw_outer_screws(dxf, dimensions):
	w, h = dimensions
	dxf.add_circle(u_to_coords(0, 0), 2.2)
	dxf.add_circle(u_to_coords(w, 0), 2.2)
	dxf.add_circle(u_to_coords(w, h), 2.2)
	dxf.add_circle(u_to_coords(0, h), 2.2)

def draw_inner_screws(dxf):
	dxf.add_circle(u_to_coords(5, 3), 2.2)
	dxf.add_circle(u_to_coords(10, 3), 2.2)
	dxf.add_circle(u_to_coords(15.125, 3), 2.2)
	dxf.add_circle(u_to_coords(18.375, 3), 2.2)

def draw_outline(dxf, dimensions):
	w, h = dimensions
	dxf.add_line(u_to_coords(0, -0.5), u_to_coords(w, -0.5))
	dxf.add_line(u_to_coords(w+0.5, 0), u_to_coords(w+0.5, h))
	dxf.add_line(u_to_coords(w, h+0.5), u_to_coords(0, h+0.5))
	dxf.add_line(u_to_coords(-0.5, h), u_to_coords(-0.5, 0))
	dxf.add_arc(u_to_coords(w, 0), radius=u_to_mm(0.5), start=0, end=90)
	dxf.add_arc(u_to_coords(w, h), radius=u_to_mm(0.5), start=270, end=0)
	dxf.add_arc(u_to_coords(0, h), radius=u_to_mm(0.5), start=180, end=270)
	dxf.add_arc(u_to_coords(0, 0), radius=u_to_mm(0.5), start=90, end=180)

def get_center(item):
	center_x = u_to_mm(item['x'] + item['w']/2)
	center_y = -u_to_mm(item['y'] + item['h']/2)
	return (center_x, center_y)

def u_to_mm(n):
	return 19.05*n

def u_to_coords(x, y):
	return (19.05*x, -19.05*y)

def generate_dxf(layout, plate_filename, back_filename):
	dimensions = (22.5, 7.5)

	with r12writer(plate_filename) as dxf:
		draw_outline(dxf, dimensions)
		draw_outer_screws(dxf, dimensions)
		for item in layout:
			draw_item(dxf, item)

	with r12writer(back_filename) as dxf:
		draw_outline(dxf, dimensions)
		draw_outer_screws(dxf, dimensions)
		draw_inner_screws(dxf)

main(infile, outfile_plate, outfile_back)
