import os
import sys
import subprocess as subp


def launch_grids(grids_folder, lvl, logs_folder):
    solver_path = os.path.join(sys.path[0], "solver")
    l = os.listdir(grids_folder)
    grids = [x for x in l if x.startswith(lvl)]

    for grid in grids:
        grid_path = os.path.join(grids_folder, grid)
        log_path = os.path.join(logs_folder, "{0}.log".format(grid.split(".")[0]))
        cmd = "{0} -p {1} | tee {2}".format(solver_path, grid_path, log_path)

        p = subp.Popen(cmd, stdout = subp.PIPE, shell = True)
        p.communicate()


def main():
    logs_folder = os.path.join(sys.path[0], "logs")
    grids_folder = os.path.join(sys.path[0], "grids")
    levels = ["beginner", "elementary", "intermediate", "expert", "master"]

    if not os.path.exists(logs_folder):
        os.mkdir(logs_folder)

    for lvl in levels:
        launch_grids(grids_folder, lvl, logs_folder)


if __name__ == '__main__':
    main()