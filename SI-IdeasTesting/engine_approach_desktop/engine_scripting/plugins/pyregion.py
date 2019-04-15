from libengine_scripting_lib import Region


class PyRegion(Region):
   def __init__(self):
      super(PyRegion, self).__init__()

   def on_enter(self):
      print("from PyRegion on_enter")

   def on_continuous(self):
      print("from PyRegion on_conti")

   def on_leave(self):
      print("from PyRegion on_leave")

if __name__ == '__main__':
    pass