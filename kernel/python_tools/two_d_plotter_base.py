"""
Base class for 2D plotting utilities. The aim of this and
the derived classes is to become a thin layer on top
of matplotlib rich functionality so that it is easier to use
"""

import matplotlib.pyplot as plt



class TwoDPlotterBase(object):

    def __init__(self, **kwargs):

        self.__plot_title = "No Title"
        if 'plot_title' in kwargs.keys():
            self.__plot_title = kwargs['plot_title']

        self.__y_axis_title = "No y-axis Title"
        if 'ylabel' in kwargs.keys():
            self.__y_axis_title = kwargs['ylabel']

        self.__x_axis_title = "No x-axis Title"
        if 'xlabel' in kwargs.keys():
            self.__x_axis_title = kwargs['xlabel']

    @property
    def xlabel(self):
        return self.__x_axis_title

    @xlabel.setter
    def xlabel(self, value):
        self.__x_axis_title = value

    @property
    def ylabel(self):
        return self.__y_axis_title

    @ylabel.setter
    def ylabel(self, value):
        self.__y_axis_title = value

    def configure_plots(self, **kwrags):

        """
        Configure the plots as needed.
        """

        plt.ylabel(self.__y_axis_title)
        plt.xlabel(self.__x_axis_title)

        if self.__plot_title != "No Title":
            plt.title(self.__plot_title)

        if 'show_grid' in kwrags.keys():
            plt.grid(True)

        if 'show_legend' in kwrags.keys():
            plt.legend()

        if 'axis_limits' in kwrags.keys():
            plt.xlim(kwrags['axis_limits'][0])
            plt.ylim(kwrags['axis_limits'][1])

        return plt

    def show_plots(self, **kwargs):
        plt = self.configure_plots(**kwargs)
        plt.show()