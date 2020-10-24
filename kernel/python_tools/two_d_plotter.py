import matplotlib.pyplot as plt

from two_d_plotter_base import TwoDPlotterBase

class TwoDPlotter(TwoDPlotterBase):

    def __init__(self, **kwargs):
        TwoDPlotterBase.__init__(self, **kwargs)

    def plot(self, x, y, label=None):
        plt.plot(x, y, label=label)
