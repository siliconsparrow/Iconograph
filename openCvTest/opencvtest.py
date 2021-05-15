#!/usr/bin/env python3

''' OpenCV experiment '''

import numpy as np
import cv2
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib.gridspec as gridspec
from matplotlib.widgets import Slider

#def onclick(evt):
#    print('click at',evt.inaxes,evt.xdata,evt.ydata)

class ImgProcess:
    def __init__(self,filename):
        # Set default processing parameters
        self.cropRect = (40,50,250,450)
        self.edgeThreshLow = 100
        self.edgeThreshHigh = 100

        print('loading file')
        self.imgOrig = cv2.imread('AdamInHat-small.jpg')

        print('convert to RGB')
        self.imgOrig = cv2.cvtColor(self.imgOrig, cv2.COLOR_BGR2RGB) 

        # Start processing.
        self.grabCut()

    # Step 1 - Cut out the background.
    def grabCut(self):
        print('GrabCut')
        mask = np.zeros(self.imgOrig.shape[:2], np.uint8)
        bgdModel = np.zeros((1,65), np.float64)
        fgdModel = np.zeros((1,65), np.float64)
        cv2.grabCut(self.imgOrig, mask, self.cropRect, bgdModel, fgdModel, 5, cv2.GC_INIT_WITH_RECT)
        mask2 = np.where((mask==2)|(mask==0),0,1).astype('uint8')
        self.imgCropped = self.imgOrig * mask2[:,:,np.newaxis]

        self.edgeDetect()

    # Step 2 - Detect edges.
    def edgeDetect(self):
        #print('Adaptive Threshold')
        #img3 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)

        print('Edge detect')
        self.imgEdges = cv2.Canny(self.imgCropped, self.edgeThreshLow, self.edgeThreshHigh)

    # Save the result to disk.
    def save(self):
        cv2.imwrite('output.png', self.imgEdges)


class DraggableRectangle:
    def __init__(self, rect):
        self.rect = rect
        self.press = None

    def connect(self):
        """Connect to all the events we need."""
        self.cidpress = self.rect.figure.canvas.mpl_connect(
            'button_press_event', self.on_press)
        self.cidrelease = self.rect.figure.canvas.mpl_connect(
            'button_release_event', self.on_release)
        self.cidmotion = self.rect.figure.canvas.mpl_connect(
            'motion_notify_event', self.on_motion)

    def on_press(self, event):
        """Check whether mouse is over us; if so, store some data."""
        if event.inaxes != self.rect.axes:
            return
        contains, attrd = self.rect.contains(event)
        if not contains:
            return
        print('event contains', self.rect.xy)
        self.press = self.rect.xy, (event.xdata, event.ydata)

    def on_motion(self, event):
        """Move the rectangle if the mouse is over us."""
        if self.press is None or event.inaxes != self.rect.axes:
            return
        (x0, y0), (xpress, ypress) = self.press
        dx = event.xdata - xpress
        dy = event.ydata - ypress
        # print(f'x0={x0}, xpress={xpress}, event.xdata={event.xdata}, '
        #       f'dx={dx}, x0+dx={x0+dx}')
        self.rect.set_x(x0+dx)
        self.rect.set_y(y0+dy)

        self.rect.figure.canvas.draw()

    def on_release(self, event):
        """Clear button press information."""
        global fig,axes,img,bgdModel,fgdModel
        axes[1].imshow(cv2.grabCut(img,mask,self.rect,bgdModel,fgdModel,5,cv2.GC_INIT_WITH_RECT))
        fig.canvas.draw_idle()
        self.press = None
        self.rect.figure.canvas.draw()

    def disconnect(self):
        """Disconnect all callbacks."""
        self.rect.figure.canvas.mpl_disconnect(self.cidpress)
        self.rect.figure.canvas.mpl_disconnect(self.cidrelease)
        self.rect.figure.canvas.mpl_disconnect(self.cidmotion)

# Load the source image and process.
proc = ImgProcess('AdamInHat-small.jpg')

# Create window with side-by-side plots.
print('Display')
fig, axes = plt.subplots(1,3)
fig.canvas.set_window_title('Image Processing Test')
plt.setp(axes, xticks=[], yticks=[])

# Go fullscreen.
plt.get_current_fig_manager().window.showMaximized()

# Set the content of each plot.
axes[0].imshow(proc.imgOrig)
axes[0].set_title('Original')

axes[1].imshow(proc.imgCropped)
axes[1].set_title('Auto crop')

axes[2].imshow(proc.imgEdges, cmap='gray')
axes[2].set_title('Edge detect')

# Set up sliders for edge detect parameters
def thresholdUpdate(low,high):
    global fig,axes,proc
    if low >= 0:
        proc.edgeThreshLow = low;
    if high >= 0:
        proc.edgeThreshHigh = high
    proc.edgeDetect()
    axes[2].imshow(proc.imgEdges, cmap='gray')
    fig.canvas.draw_idle()
    
def slideUpdateLow(val):
    thresholdUpdate(val,-1)
def slideUpdateHigh(val):
    thresholdUpdate(-1, val)

boxThreshLow = plt.axes([0.20, 0.05, 0.65, 0.03], facecolor='yellow')
boxThreshHigh = plt.axes([0.20, 0.01, 0.65, 0.03], facecolor='yellow')
slideThreshLow = Slider(boxThreshLow, 'Low Threshold', 0, 255, valinit = proc.edgeThreshLow)
slideThreshLow.on_changed(slideUpdateLow)
slideThreshHigh = Slider(boxThreshHigh, 'High Threshold', 0, 255, valinit = proc.edgeThreshHigh)
slideThreshHigh.on_changed(slideUpdateHigh)

#def update(val):
#   axes[2].imshow(np.random.rand(3, 3))
#   fig.canvas.draw_idle()
#slider.on_changed(update)

# Set up mouse events
#connId = fig.canvas.mpl_connect('button_press_event', onclick)

# Set up draggable rectangle
rect = patches.Rectangle((40, 50), 250, 400, linewidth=4, edgecolor='r', facecolor='none')
axes[0].add_patch(rect)
#dr = DraggableRectangle(rect)
#dr.connect()

plt.show()



