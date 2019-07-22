from libPySI import PySiRegion


class PyRegion(PySiRegion):
   def __init__(self):
      super(PyRegion, self).__init__()

      self.name = "PyRegion"

   def __repr__(self):
      return "Name: {}".format(self.name)

   def on_enter(self):
      print("from PyRegion on_enter")

   def on_continuous(self):
      print("from PyRegion on_conti")

   def on_leave(self):
      print("from PyRegion on_leave")

if __name__ == '__main__':
    pass