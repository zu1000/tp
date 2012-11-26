#!/usr/bin/python

import wx

class MyFrame(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, -1, title, size = (970, 720),
                          style=wx.DEFAULT_FRAME_STYLE |
                                wx.NO_FULL_REPAINT_ON_RESIZE)
        self.Center()

class MyApp(wx.App):
    def OnInit(self):
        frame = MyFrame(None, 'hello world')
        frame.Show()
        return True

if __name__ == '__main__':
    app = MyApp(False)
    app.MainLoop()
if __name__ == '__main__':
    print 'TODO'
