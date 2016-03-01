from subprocess import call, check_output, CalledProcessError
import sys
import random

def main():
    check_bubbleSort()
    check_quickSort()
    check_mergeSort()
    check_inplace_qs()
    print

def listify_output(s):
    ''' Turns raw output from sorter program into list of ints. '''
    return map(int, filter(isInt, s.split()))

def check_mergeSort():
    print("Checking merge sort...")
    for i in range(NUM_TESTS):
        nums = generate_nums(i);
        try:
            raw_out = check_output(["./sorter", "-m", "out"])
        except:
            print("Student program crashed...")
            return
        lst_out = listify_output(raw_out)
        if (sorted(lst_out) == lst_out):
            sys.stdout.write(".")
        else:
            print("Student merge sort failed on test case:")
            print(nums)
            print("Student output: "), lst_out
    print
    return

def check_quickSort():
    print("Checking quicksort...")
    for i in range(NUM_TESTS):
        nums = generate_nums(i);
        try:
            raw_out = check_output(["./sorter", "-q", "out"])
        except:
            print("Student program crashed...")
            return
        lst_out = listify_output(raw_out)
        if (sorted(lst_out) == lst_out):
            sys.stdout.write(".")
        else:
            print("Student quickSort failed on test case:")
            print(nums)
            print("Student output: "), lst_out
    print
    return

def check_bubbleSort():
    print("Checking bubble sort...")
    for i in range(NUM_TESTS):
        nums = generate_nums(i);
        try:
            raw_out = check_output(["./sorter", "-b", "out"])
        except:
            print("Student program crashed...")
            return
        lst_out = listify_output(raw_out)
        if (sorted(lst_out) == lst_out):
            sys.stdout.write(".")
        else:
            print("Student bubbleSort failed on test case:")
            print(nums)
            print("Student output: "), lst_out
    print
    return

def check_inplace_qs():
    print("Checking in-place quicksort...")
    for i in range(NUM_TESTS):
        nums = generate_nums(i);
        try:
            raw_out = check_output(["./sorter", "-qi", "out"])
        except:
            print("Student program crashed...")
            return
        lst_out = listify_output(raw_out)
        if (sorted(lst_out) == lst_out):
            sys.stdout.write(".")
        else:
            print("Student in-place quicksort failed on test case:")
            print(nums);
            print("Student output: "), lst_out
    print
    return

def generate_nums(n):
    lst = [random.randint(MIN_VAL, MAX_VAL) for i in range(n)]
    f = open("out", 'w')
    write_out(lst, f)
    f.close()
    return lst

def write_out(nums, f):
    for num in nums:
        f.write("%d\n" % num)
    return

def isInt(x):
    try:
        int(x)
    except ValueError:
        return False
    return True

if __name__ == '__main__':
    NUM_TESTS = 50
    MIN_VAL = -1000
    MAX_VAL = 1000
    main()
