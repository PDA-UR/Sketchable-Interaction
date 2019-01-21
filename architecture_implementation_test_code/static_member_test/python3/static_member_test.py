
class Test:
  i = 3

  def __init__(self):
    pass

t = Test()

t.i = 4

print(Test.i, t.i) # prints 3 4

Test.i = 5

print(Test.i, t.i) # prints 5 4