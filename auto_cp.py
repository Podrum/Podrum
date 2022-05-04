from ast import Try
import shutil
from time import sleep

while True:
    try:
        shutil.copyfile(r'C:\Users\nguyenduck\Desktop\Podrum\build\Debug\PodrumR3.exe', r'C:\Users\nguyenduck\Desktop\Podrum\PodrumR3.exe')
        shutil.copyfile(r'C:\Users\nguyenduck\Desktop\Podrum\build\Debug\PodrumR3.pdb', r'C:\Users\nguyenduck\Desktop\Podrum\PodrumR3.pdb')
    except: pass
    sleep(5)