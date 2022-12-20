# list-alloc

Algorithm will not call to realloc since it is expensive operation and instead will add the data to the end of the new list, modify the external pointer and continue from that point