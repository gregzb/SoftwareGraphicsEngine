import os
from PIL import Image

from pathlib import Path

for path in Path('.').rglob('*.mtl'):
    #print(path.parent)
    print(path)
    f_path = str(path)
    lines = []
    with open(f_path, 'r') as f:
        lines = f.read().strip().split('\n')

    for i in range(len(lines)):
        if lines[i].startswith('map_'):
            split_words = lines[i].split(' ')
            initial = split_words[0]
            joined = ' '.join(split_words[1:])
            if not os.path.exists(joined):
                print("bad path, skipping: ", joined)
                #exit()
            else:
                print("good path: ", joined)

                im = Image.open(joined)
                new_file = Path(joined).stem + ".ppm"
                im.save(str(path.parent) + "/" + new_file)
                lines[i] = initial + " " + new_file

    with open(f_path, 'w') as f:
        f.write('\n'.join(lines))
            #print(os.path.normpath(joined))
            #pa = Path(joined)
            #print(joined, pa.name)
            #im = Image.open(joined)
            #im.save("")
            # 
    print(lines)
