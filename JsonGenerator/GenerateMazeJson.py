#!/usr/bin/python3
import json
import random
import math

def hex_to_signed(source):
    """Convert a string hex value to a signed hexidecimal value.

    This assumes that source is the proper length, and the sign bit
    is the first bit in the first byte of the correct length.

    hex_to_signed("F") should return -1.
    hex_to_signed("0F") should return 15.
    """
    if not isinstance(source, str):
        raise ValueError("string type required")
    if 0 == len(source):
        raise valueError("string is empty")
    sign_bit_mask = 1 << (len(source)*4-1)
    other_bits_mask = sign_bit_mask - 1
    value = int(source, 16)
    return -(value & sign_bit_mask) | (value & other_bits_mask)

def botColor(id):
	if i % 3 == 0:
		return hex_to_signed('ffff0000')
	elif i % 3 == 1:
		return hex_to_signed('ff00ff00')
	else:
		return hex_to_signed('ff0000ff')

rows = 10
columns = 10
nTicks = 90
ticks = []
for tick in range(nTicks):
    maze = []
    for y in range(rows):
        row = []
        for x in range(columns):
            row.append({
                "type": random.randint(0, 1),
                "orientation": random.randint(0,3) * 90
            })
        maze.append(row)

    actionTiles = []
    nActionTiles = 5
    for i in range(nActionTiles):
        actionTiles.append({
            "x": random.randint(0,9),
            "y": random.randint(0,9),
            "direction": random.choice(["left", "right", "up", "down"]),
            "laneId": random.randint(0,9)
        })
        
    nBots = 3
    bots = []
    for i in range(nBots):
        bots.append({
            "arucoId": i,
            "position": [0.2 + (tick/(nTicks-1))*0.6, 0.6],
            "forward": [0.02, -0.03],
            "right": [-0.01, 0.04],
			"score": (i+1) * 10,
			"color": botColor(i)
        })

    ticks.append({
        "gameId": 1,
        "gameTick": tick,
        "rows": rows,
        "columns": columns,
        "data": maze,
        "actionTiles": actionTiles,
        "bots": bots
    })

for i, tick in enumerate(ticks, start=0):
    with open("tick" + str(i) + ".json", 'w') as f:
        json.dump(tick, f)
        f.write("\n")
