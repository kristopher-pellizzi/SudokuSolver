import os
import sys
import re
import requests as reqs

def get_last_index(grids, lvl_name):
    l = [x for x in grids if x.startswith(lvl_name)]
    if len(l) == 0:
        return 0

    return max(map(int, map(lambda x: x.split(".")[0].split(lvl_name)[1], l)))


def download_new_grid(lvl, grids_folder, file_name, file_idx):
    url = "https://sudoku9x9.com/?level={0}".format(lvl)

    res = reqs.get(url).text

    m_orig = re.search("var\s+orig\s*=\s*new Array\((.*)\);", res)
    m_sol = re.search("var\s+solution\s*=\s*new Array\((.*)\);", res)
    if m_orig is None or m_sol is None:
        print(res)
        raise Exception("Could not find sudoku grid values")
    
    orig = m_orig.group(1)
    sol = m_sol.group(1)
    orig = list(map(int, map(lambda x: x.strip(), orig.split(","))))
    sol = list(map(int, map(lambda x: x.strip(), sol.split(","))))
    filename = "{0}{1}.txt".format(file_name, file_idx)

    with open(os.path.join(grids_folder, filename), "w") as f:
        for i in range(9):
            line = ""
            for j in range(9):
                idx = i * 9 + j
                val = 0
                if orig[idx] == 0:
                    val = 0
                else:
                    val = sol[idx]
                line = "{0}{1} ".format(line, val)

            line.strip()
            f.write(line)
            if i != 8:
                f.write("\n")
    
    print("Grid stored into {0}".format(filename))


def main():
    grids_folder = os.path.join(sys.path[0], 'grids')
    levels_map = {
        1: 'beginner',
        2: 'elementary',
        3: 'intermediate',
        4: 'expert',
        5: 'master'
    }
    
    last_lvl_idx = {
        1: 0,
        2: 0,
        3: 0,
        4: 0,
        5: 0
    }

    grids = os.listdir(grids_folder)

    for lvl, name in levels_map.items():
        last_lvl_idx = get_last_index(grids, name) + 1

        for i in range(10):
            download_new_grid(lvl, grids_folder, name, last_lvl_idx + i)
    
    print("Finished")

if __name__ == '__main__':
    main()