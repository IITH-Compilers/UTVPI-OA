line = input()
while (line != 'begin'):
    line = input()

line = input()

[a, b, _] = line.split()

m = int(a)
n = int(b)

print(m, n+1)

s = ''

for i in range(m):
   line = input()
   arr = line.split()
   s = '1 '
   c = arr[0]
   arr = arr[1:]
   arr.append(c)
   s += ' '.join(arr)
   print(s)
