#!/usr/bin/python3
import json
import random

rows = 10
columns = 10
nTicks = 2
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
        
    nBots = 1
    bots = []
    for i in range(nBots):
        bots.append({
            "arucoId": i,
            "position": [0.2 + (tick/(nTicks-1))*0.6, 0.6],
            "forward": [0.02, -0.03],
            "right": [-0.01, 0.04]
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
