#!/usr/bin/env python3

from tkinter import *
import math

RodLen1 = 90.0
RodLen2 = 80.0
RodWidth = 10

# Class to manage the drawable area
class Graphics(Frame):
    def __init__(self, title, origin = (0, 0), scale = (1.0, -1.0)):
        super().__init__()
        self.master.title(title)
        self.pack(fill=BOTH, expand=1)
        self.canvas = Canvas(self)
        self.origin = origin
        self.scale = scale

    def transform(self, x):
        if type(x) is tuple:
            return (self.origin[0] + (x[0] * self.scale[0]),
                    self.origin[1] + (x[1] * self.scale[1]))
        else:
            return x * self.scale[0]

# Class to draw a line then move it around.
class GfxLine:
    def __init__(self, gfx, p1, p2, w):
        self.gfx = gfx
        self.line = self.gfx.canvas.create_line(p1[0], p1[1], p2[0], p2[1], width=w, capstyle=ROUND)
        self.gfx.canvas.pack(fill=BOTH, expand=1)

    def move(self, p1, p2):
        self.gfx.canvas.coords(self.line, p1[0], p1[1], p2[0], p2[1])

    def coords(self):
        return self.gfx.canvas.coords(self.line)

# Draws a line with thickness, position and angle.
class Rod:
    def __init__(self, gfx, length, width):
        self.gfx = gfx
        self.origin = (0,0)
        self.length = length
        self.angle = 0
        self.line = GfxLine(self.gfx, self.gfx.transform(self.origin), self.gfx.transform((1, 1)), self.gfx.transform(width))
        self.redraw()

    def redraw(self):
        endPt = self.getEndPos()
        #print("LINE:",self.origin,endPt)
        self.line.move(self.gfx.transform(self.origin), self.gfx.transform(endPt))

    def getEndPos(self):
        return (self.origin[0] + (self.length * math.cos(self.angle)),
                self.origin[1] + (self.length * math.sin(self.angle)))
    
    def setOrigin(self, origin):
        self.origin = origin
        self.redraw()
        
    def setAngle(self, angle):
        self.angle = angle
        self.redraw()

class Simulation:
    def __init__(self, len1, len2):
        self.gfx = Graphics('Iconograph Mechanical Simulation', (400,400), (2.0, -2.0))
        self.rod1 = Rod(self.gfx, len1, RodWidth)
        self.rod2 = Rod(self.gfx, len2, RodWidth)
        self.xh1 = GfxLine(self.gfx, self.gfx.transform((-20, 0)), self.gfx.transform((20, 0)), self.gfx.transform(1))
        self.xh2 = GfxLine(self.gfx, self.gfx.transform((0, -20)), self.gfx.transform((0, 20)), self.gfx.transform(1))
        GfxLine(self.gfx, self.gfx.transform((-200, 0)), self.gfx.transform((200, 0)), self.gfx.transform(1))

    # When given the adjacent and opposite, calculate the length
    # of the hypotenuse.
    def lengthOfHypotenuse(self, x, y):
        return math.sqrt((x*x)+(y*y))
    
    # When given the adjacent and opposite, calculate theta.
    def calcTheta(self, x, y):
        return math.atan(y / x)
    
    # When given the lengths of three sides of a triangle, use cosine
    # rule to calculate the angle opposite c.
    def calcOppositeAngle(self, a, b, c):
        return math.acos((a**2 + b**2 - c**2)/(2 * a * b))

    # Convert radians to degrees
    def fmtAngle(self, x):
        return str(round((360 * x) / (2 * math.pi), 3)) + u"\u00b0"

    def calcAngles(self, x, y):
        d = self.lengthOfHypotenuse(x, y)
        print("d=",str(round(d, 3)))
        angle0 = self.calcOppositeAngle(d, self.rod1.length, self.rod2.length)
        print("angle0=",self.fmtAngle(angle0))
        angle1 = (math.pi) - self.calcOppositeAngle(self.rod1.length, self.rod2.length, d)
        print("angle1=",self.fmtAngle(angle1))
        angle2 = self.calcTheta(-x, y)
        print("angle2=",self.fmtAngle(angle2))
        baseAngle = math.pi - angle2 - angle0
        print("baseAngle=",self.fmtAngle(baseAngle))
        return (baseAngle,angle1)

    def setAt(self, x, y):
        angles = self.calcAngles(x, y)
        self.xh1.move(self.gfx.transform((x-20, y)), self.gfx.transform((x+20, y)))
        self.xh2.move(self.gfx.transform((x, y-20)), self.gfx.transform((x, y+20)))
        self.rod1.setAngle(angles[0])
        self.rod2.setOrigin(self.rod1.getEndPos())
        self.rod2.setAngle(angles[0] + angles[1])

class App:
    def __init__(self):
        self.root = Tk()
        self.root.geometry("800x700+100+100")
        self.sim = Simulation(RodLen1, RodLen2)
        self.x = -100
        self.y = 60
        self.th = 0
        self.r = 30
        #self.sim.setAt(self.x, self.y)
        self.root.after(250, self.tick)
        x = -65
        y = 70
        print("X:{0},Y:{1}".format(x,y))
        self.sim.setAt(x, y)
        self.root.mainloop()

    def tick(self):
        x = self.x + self.r * math.cos(self.th)
        y = self.y + self.r * math.sin(self.th)
        print("X:{0},Y:{1}".format(x,y))
        self.sim.setAt(x, y)
        self.th = self.th + 0.1
        self.root.after(33, self.tick)

app = App()
