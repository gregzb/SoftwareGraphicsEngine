# basebase = """push
# """
# base = """rotate
# y 5
# rotate
# z 10
# rotate
# x 15
# move
# 250 250 0
# sphere
# 0 0 0 200
# move
# -250 -250 0
# save
# {}.png
# clear
# """

# final = basebase

# for i in range(19):
#     final += base.format(str(i).zfill(3))

# with open('script', 'w') as f:
#     f.write(final)

basebase = """push
movescreen
250 250 0
"""
base = """rotate
x 5
rotate
y 8
rotate
z 2
torus
0 0 0 50 200
save
{}.png
clear
"""

final = basebase

for i in range(37):
    final += base.format(str(i).zfill(3))

with open('script', 'w') as f:
    f.write(final)