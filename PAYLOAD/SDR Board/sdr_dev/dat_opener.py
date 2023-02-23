import os
import scipy
import fnmatch

def main():

    file_list = fnmatch.filter(os.listdir("/home/analog/Documents/sdr_dev/data/"), "*.dat")
    
    print("file_list[0]")
    dat_file = scipy.fromfile(open(os.getcwd() + "/data/" + file_list[0]), dtype=scipy.float32)
    for i in tpr_file:
        print(i)

    print("file_list[1]")
    dat_file = scipy.fromfile(open(os.getcwd() + "/data/" + file_list[1]), dtype=scipy.float32)
    for i in tpr_file:
        print(i)

if __name__ == '__main__':
	main()